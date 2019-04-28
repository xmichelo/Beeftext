/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboManager.h"
#include "InputManager.h"
#include "PreferencesManager.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include "Backup/BackupManager.h"
#include "EmojiManager.h"


using namespace xmilib;


bool isBeeftextTheForegroundApplication(); ///< Check whether Beeftext is the foreground application


//**********************************************************************************************************************
/// \return true if and only if Beeftext is the application currently in the foreground
//**********************************************************************************************************************
bool isBeeftextTheForegroundApplication()
{
   DWORD processId = 0;
   GetWindowThreadProcessId(GetForegroundWindow(), &processId);
   return QCoreApplication::applicationPid() == processId;
}


//**********************************************************************************************************************
/// \return A reference to the only allowed instance of the class
//**********************************************************************************************************************
ComboManager& ComboManager::instance()
{
   static ComboManager instance;
   return instance;
}


//**********************************************************************************************************************
/// \return A mutable reference to the combo list
//**********************************************************************************************************************
ComboList& ComboManager::comboListRef()
{
   return comboList_;
}


//**********************************************************************************************************************
/// \return A constant reference to the combo list
//**********************************************************************************************************************
ComboList const& ComboManager::comboListRef() const
{
   return comboList_;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
ComboManager::ComboManager()
   : sound_(std::make_unique<QSound>(":/MainWindow/Resources/Notification.wav"))
{
   // We used queued connections to minimize the time spent in the keyboard hook
   InputManager& inputManager = InputManager::instance();
   connect(&inputManager, &InputManager::comboBreakerTyped, this, &ComboManager::onComboBreakerTyped,
      Qt::QueuedConnection);
   connect(&inputManager, &InputManager::characterTyped, this, &ComboManager::onCharacterTyped,
      Qt::QueuedConnection);
   connect(&inputManager, &InputManager::backspaceTyped, this, &ComboManager::onBackspaceTyped,
      Qt::QueuedConnection);
   connect(&inputManager, &InputManager::substitutionShortcutTriggered, this,
      &ComboManager::onSubstitutionTriggerShortcut, Qt::QueuedConnection);
   QString errMsg;

   if (!QFileInfo(QDir(PreferencesManager::instance().comboListFolderPath())
      .absoluteFilePath(ComboList::defaultFileName)).exists()) // we avoid displaying an error on first launch
   {
      comboList_.ensureCorrectGrouping();
      return;
   }
   if (!this->loadComboListFromFile(&errMsg))
      QMessageBox::critical(nullptr, tr("Error"), errMsg);
}


//**********************************************************************************************************************
/// \return A reference to the combo group attached to the combo list
//**********************************************************************************************************************
GroupList& ComboManager::groupListRef()
{
   return comboList_.groupListRef();
}


//**********************************************************************************************************************
/// \return A constant reference to the combo group attached to the combo list
//**********************************************************************************************************************
GroupList const& ComboManager::groupListRef() const
{
   return comboList_.groupListRef();
}


//**********************************************************************************************************************
/// \param[out] outErrorMsg If not null the function returns false, this variable will contain a description 
/// of the error.
//**********************************************************************************************************************
bool ComboManager::loadComboListFromFile(QString* outErrorMsg)
{
   BackupManager::instance().cleanup();
   bool inOlderFormat = false;
   QString const& path = QDir(PreferencesManager::instance().comboListFolderPath())
      .absoluteFilePath(ComboList::defaultFileName);
   if (!comboList_.load(path, &inOlderFormat, outErrorMsg))
      return false;
   bool wasInvalid = false;
   comboList_.ensureCorrectGrouping(&wasInvalid);
   if (inOlderFormat || wasInvalid)
   {
      if (!this->saveComboListToFile(outErrorMsg))
         globals::debugLog().addWarning(inOlderFormat ?
            "Could not upgrade the combo list file to the newest format version." :
            "Could not save the combo list file after fixing the grouping of combos.");
      else
         globals::debugLog().addInfo(inOlderFormat ? "The combo list file was upgraded to the latest format version." :
            "The combo list file was successfully saved after fixing the the grouping of combos.");
   }
   emit comboListWasLoaded();
   return true;
}


//**********************************************************************************************************************
/// \param[out] outErrorMsg If not null the function returns false, this variable will contain a description 
/// of the error.
/// \return true if and only if the operation completed successfully
//**********************************************************************************************************************
bool ComboManager::saveComboListToFile(QString* outErrorMsg) const
{
   PreferencesManager& prefs = PreferencesManager::instance();
   QString const filePath = QDir(prefs.comboListFolderPath()).absoluteFilePath(ComboList::defaultFileName);
   if (prefs.autoBackup())
      BackupManager::instance().archive(filePath);
   bool const result = comboList_.save(filePath, true, outErrorMsg);
   if (result)
   emit comboListWasSaved();
   return result;
}


//**********************************************************************************************************************
/// \param[in] backupFilePath The path of the backup file
/// \return true if the backup was correctly restored
//**********************************************************************************************************************
bool ComboManager::restoreBackup(QString const& backupFilePath)
{
   bool inOlderFormat = false;
   QString outErrorMsg;
   if (!comboList_.load(backupFilePath, &inOlderFormat, &outErrorMsg))
      return false;
   comboList_.ensureCorrectGrouping();
   emit backupWasRestored();
   return this->saveComboListToFile();
}


//**********************************************************************************************************************
/// \return true if and only if the combo manager is enabled
//**********************************************************************************************************************
bool ComboManager::isEnabled()
{
   return InputManager::instance().isKeyboardHookEnable();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboManager::enable()
{
   InputManager& inputManager = InputManager::instance();
   inputManager.enableKeyboardHook();
#ifdef NDEBUG
   inputManager.enableMouseHook();
#endif
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboManager::disable()
{
   InputManager& inputManager = InputManager::instance();
   inputManager.disableKeyboardHook();
#ifdef NDEBUG
   inputManager.disableMouseHook();
#endif
}


//**********************************************************************************************************************
/// return The previous state if the combo manager
//**********************************************************************************************************************
bool ComboManager::setEnabled(bool enabled)
{
   bool const result = isEnabled();
   if (enabled)
      enable();
   else
      disable();
   return result;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboManager::checkAndPerformSubstitution()
{
   if (!this->checkAndPerformComboSubstitution())
      this->checkAndPerformEmojiSubstitution();
}


//**********************************************************************************************************************
/// \return true if a substitution was available and it was performed or cancelled because Beeftext is the active 
/// application.
//**********************************************************************************************************************
bool ComboManager::checkAndPerformComboSubstitution()
{
   VecSpCombo result;
   for (SpCombo const& combo: comboList_)
      if (combo && combo->isEnabled() && combo->matchesForInput(currentText_))
         result.push_back(combo);
   VecSpCombo::const_iterator const it = std::find_if(comboList_.begin(), comboList_.end(),
      [&](SpCombo const& combo) -> bool
      {
         return combo && combo->isEnabled() && combo->matchesForInput(currentText_);
      });

   if (result.empty())
      return false;

   SpCombo const combo = result[result.size() > 1 ? rng_.get() % result.size() : 0];
   if ((!isBeeftextTheForegroundApplication()) &&
      (combo->performSubstitution() && PreferencesManager::instance().playSoundOnCombo()))
      sound_->play(); // in Beeftext windows, substitution is disabled
   this->onComboBreakerTyped();
   return true;
}


//**********************************************************************************************************************
/// \return true if a substitution was available and it was performed or cancelled because Beeftext is the active 
/// application.
//**********************************************************************************************************************
bool ComboManager::checkAndPerformEmojiSubstitution()
{
   qint32 const textSize = currentText_.size();
   PreferencesManager& prefs = PreferencesManager::instance();
   if (!prefs.emojiShortcodesEnabled())
      return false;
   QString const leftDelimiter = prefs.emojiLeftDelimiter();
   bool const useEmojiRightDelimiter = prefs.useEmojiRightDelimiter();
   QString const rightDelimiter = useEmojiRightDelimiter ? prefs.emojiRightDelimiter() : QString();
   qint32 const delimsSize = leftDelimiter.size() + rightDelimiter.size();
   if ((textSize <= delimsSize) || (!currentText_.startsWith(leftDelimiter)) || 
      (!currentText_.endsWith(rightDelimiter)))
      return false;
   QString const keyword = currentText_.mid(leftDelimiter.size(), currentText_.size() - delimsSize);
   EmojiManager& emojisManager = EmojiManager::instance();
   QString emoji = emojisManager.emoji(keyword);
   if (emoji.isEmpty())
      return false;
   bool result = false;
   if ((!isBeeftextTheForegroundApplication()) &&
      !EmojiManager::instance().isExcludedApplication(getActiveExecutableFileName()))
   {
      performTextSubstitution(currentText_.size(), emoji, -1);
      if (PreferencesManager::instance().playSoundOnCombo())
         sound_->play();
      result = true;
   }
   this->onComboBreakerTyped();
   return result;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboManager::onComboBreakerTyped()
{
   currentText_ = QString();
}


//**********************************************************************************************************************
/// \param[in] c The character that was typed
//**********************************************************************************************************************
void ComboManager::onCharacterTyped(QChar c)
{
   currentText_.append(c);
   if (!PreferencesManager::instance().useAutomaticSubstitution())
      return;
   this->checkAndPerformSubstitution();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboManager::onBackspaceTyped()
{
   currentText_.chop(1);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboManager::onSubstitutionTriggerShortcut()
{
   if (!PreferencesManager::instance().useAutomaticSubstitution())
      this->checkAndPerformSubstitution();
}
