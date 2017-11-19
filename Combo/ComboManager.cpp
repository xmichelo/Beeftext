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
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {
   QString const kComboListFileName = "comboList.json"; ///< The name of the default combo list file
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
ComboList& ComboManager::getComboListRef()
{
   return comboList_;
}


//**********************************************************************************************************************
/// \return A constant reference to the combo list
//**********************************************************************************************************************
ComboList const& ComboManager::getComboListRef() const
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
   InputManager& inputManager = InputManager::instance();
   connect(&inputManager, &InputManager::comboBreakerTyped, this, &ComboManager::onComboBreakerTyped);
   connect(&inputManager, &InputManager::characterTyped, this, &ComboManager::onCharacterTyped);
   connect(&inputManager, &InputManager::backspaceTyped, this, &ComboManager::onBackspaceTyped);

   QString errMsg;
   if (!this->loadComboListFromFile(&errMsg))
      QMessageBox::critical(nullptr, tr("Error"), errMsg);
}


//**********************************************************************************************************************
/// \param[out] outErrorMsg If not null the function returns false, this variable will contain a description 
/// of the error.
//**********************************************************************************************************************
bool ComboManager::loadComboListFromFile(QString* outErrorMsg)
{
   comboList_.clear();
   QFile file(QDir(globals::getAppDataDir()).absoluteFilePath(kComboListFileName));
   if (!file.exists())
   {
      globals::debugLog().addWarning(tr("No combo list file could be found."));
      return true;
   }
   try
   {
      if (!file.open(QIODevice::ReadOnly))
         throw xmilib::Exception(tr("A combo file exists but it cannot be opened."));
      QString errMsg;
      if (!comboList_.readFromJsonDocument(QJsonDocument::fromJson(file.readAll()), &errMsg))
         throw xmilib::Exception(tr("The content of the combo list file is invalid:\n%1.").arg(errMsg));
      return true;
   }
   catch (xmilib::Exception const& e)
   {
   	if (outErrorMsg)
         *outErrorMsg = e.qwhat();
      return false;
   }
}

//**********************************************************************************************************************
/// \param[out] outErrorMsg If not null the function returns false, this variable will contain a description 
/// of the error.
/// \return true if and only if the operation completed successfully
//**********************************************************************************************************************
bool ComboManager::saveComboListToFile(QString* outErrorMsg)
{
   try
   {
      QFile file(QDir(globals::getAppDataDir()).absoluteFilePath(kComboListFileName));
      if (!file.open(QIODevice::WriteOnly))
         throw xmilib::Exception(tr("The combo list could not be saved."));
      file.write(comboList_.toJsonDocument().toJson());
      return true;
   }
   catch (xmilib::Exception const& e)
   {
      if (outErrorMsg)
         *outErrorMsg = e.qwhat();
      return false;
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboManager::tempAddTestCombos()
{
   comboList_.append(Combo::create("Personal Email", "xxem", "johndoe@gmail.com"));
   comboList_.append(Combo::create("Personal Signature","xxsig", 
      "Regards.\n\n-- \nJohn Doe\n\"johndoe@gmail.com\"\n"));
   comboList_.append(Combo::create("Personal Name" ,"xxname", "John Doe"));
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
   VecSPCombo::const_iterator it = std::find_if(comboList_.begin(), comboList_.end(),
      [&](SPCombo const combo) -> bool { return combo->isEnabled() && (combo->comboText() == currentText_); });
   if (comboList_.end() == it)
      return;
   (*it)->performSubstitution();
   sound_->play();
   this->onComboBreakerTyped();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboManager::onBackspaceTyped()
{
   currentText_.chop(1);
}
