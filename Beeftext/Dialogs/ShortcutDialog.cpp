/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of shortcut dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ShortcutDialog.h"
#include "InputManager.h"
#include "Preferences/PreferencesManager.h"
#include <XMiLib/XMiLibConstants.h>


//**********************************************************************************************************************
/// \param[in] shortcut The shortcut
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ShortcutDialog::ShortcutDialog(SpShortcut const& shortcut, QWidget* parent)
   : QDialog(parent, xmilib::constants::kDefaultDialogFlags),
     ui_(),
     shortcut_(shortcut ? shortcut : PreferencesManager::defaultComboTriggerShortcut())
{
   ui_.setupUi(this);
   this->resize(QDialog::sizeHint());
   this->updateGui();
   InputManager& inputManager = InputManager::instance();
   shortcutsWereEnabled_ = inputManager.isShortcutProcessingEnabled();
   inputManager.setShortcutsProcessingEnabled(false);
   connect(&InputManager::instance(), &InputManager::shortcutPressed, this, &ShortcutDialog::onShortcutPressed);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
ShortcutDialog::~ShortcutDialog()
{
   InputManager::instance().setShortcutsProcessingEnabled(shortcutsWereEnabled_);
}


//**********************************************************************************************************************
/// \return The shortcut
//**********************************************************************************************************************
SpShortcut ShortcutDialog::shortcut() const
{
   return shortcut_;
}


//**********************************************************************************************************************
/// \param[in] shortcut The shortcut
//**********************************************************************************************************************
void ShortcutDialog::onShortcutPressed(SpShortcut const& shortcut)
{
   if ((!shortcut) || (!shortcut->isValid()))
      return;
   shortcut_ = shortcut;
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ShortcutDialog::updateGui() const
{
   ui_.editShortcut->setText(shortcut_ ? shortcut_->toString() : "");
   ui_.buttonOK->setEnabled(shortcut_.get());
}
