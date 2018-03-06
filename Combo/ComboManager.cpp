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
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


bool isBeeftextTheForegroundApplication(); ///< Check whether Beeftext is the foreground application


//**********************************************************************************************************************
/// \return true if and only if Beeftext is the application currently in the foreground
//**********************************************************************************************************************
bool isBeeftextTheForegroundApplication()
{
   DWORD processId = 0;
   GetWindowThreadProcessId(GetForegroundWindow(), &processId);
   return  qApp->applicationPid() == processId;
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
// 
//**********************************************************************************************************************
ComboManager::~ComboManager()
{
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
   : QObject()
   , sound_(std::make_unique<QSound>(":/MainWindow/Resources/Notification.wav"))
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
      return;
   if (!this->loadComboListFromFile(&errMsg))
      QMessageBox::critical(nullptr, tr("Error"), errMsg);
}


//**********************************************************************************************************************
/// \return A reference to the combo group attached to the combo list
//**********************************************************************************************************************
ComboGroupList& ComboManager::groupListRef()
{
   return comboList_.groupListRef();
}


//**********************************************************************************************************************
/// \return A constant reference to the combo group attached to the combo list
//**********************************************************************************************************************
ComboGroupList const& ComboManager::groupListRef() const
{
   return comboList_.groupListRef();
}


//**********************************************************************************************************************
/// \param[out] outErrorMsg If not null the function returns false, this variable will contain a description 
/// of the error.
//**********************************************************************************************************************
bool ComboManager::loadComboListFromFile(QString* outErrorMsg)
{
   bool inOlderFormat = false;
   QString const& path = QDir(PreferencesManager::instance().comboListFolderPath())
      .absoluteFilePath(ComboList::defaultFileName);
   if (!comboList_.load(path, &inOlderFormat, outErrorMsg))
      return false;
   if (inOlderFormat)
      if (!comboList_.save(path))
         globals::debugLog().addWarning("Could not upgrade the combo list file to the newest format version.");
      else
         globals::debugLog().addInfo("The combo list file was upgraded to the latest format version.");
   return true;
}


//**********************************************************************************************************************
/// \param[out] outErrorMsg If not null the function returns false, this variable will contain a description 
/// of the error.
/// \return true if and only if the operation completed successfully
//**********************************************************************************************************************
bool ComboManager::saveComboListToFile(QString* outErrorMsg) const
{
   return comboList_.save(QDir(PreferencesManager::instance().comboListFolderPath())
      .absoluteFilePath(ComboList::defaultFileName), outErrorMsg);
}


//**********************************************************************************************************************
/// \return true if and only if the combo manager is enabled
//**********************************************************************************************************************
bool ComboManager::isEnabled() const
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
bool ComboManager::setEnabled(bool enable)
{
   bool result = this->isEnabled();
   if (enable)
      this->enable();
   else
      this->disable();
   return result;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboManager::checkAndPerformSubstitution()
{
   VecSPCombo::const_iterator const it = std::find_if(comboList_.begin(), comboList_.end(),
      [&](SPCombo const combo) -> bool { return combo->isEnabled() && (combo->keyword() == currentText_); });
   if (comboList_.end() == it)
      return;

   if (!isBeeftextTheForegroundApplication()) // in Beeftext windows, substitution is disabled
   {
      (*it)->performSubstitution();
      if (PreferencesManager::instance().playSoundOnCombo())
         sound_->play();
   }
   this->onComboBreakerTyped();
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

