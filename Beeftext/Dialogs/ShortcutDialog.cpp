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
#include <XMiLib/XMiLibConstants.h>


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
/// \return The shortcut typed by the user.
/// \return A null pointer if the user cancelled the dialog.
//**********************************************************************************************************************
SpShortcut ShortcutDialog::run(QWidget* parent)
{
   ShortcutDialog dlg(nullptr, parent);
   return (Accepted == dlg.exec()) ? dlg.shortcut() : nullptr;
}


//**********************************************************************************************************************
/// \param[in,out] inOutShortcut On function entry this value holds the shortcut to be displayed by default in the
/// dialog. On exit this variable contains the shortcut select by the user if he accepted the dialog. If the dialog
/// was cancelled, this content of this variable on exit is undetermined.
/// \param[in] parent The parent widget of the dialog.
/// \return true if and only if the user accepted the dialog.
//**********************************************************************************************************************
bool ShortcutDialog::run(SpShortcut& inOutShortcut, QWidget* parent)
{
   ShortcutDialog dlg(inOutShortcut, parent);
   if (Accepted != dlg.exec())
      return false;
   inOutShortcut = dlg.shortcut();
   return true;
}


//**********************************************************************************************************************
/// \param[in] shortcut The shortcut
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ShortcutDialog::ShortcutDialog(SpShortcut const& shortcut, QWidget* parent)
   : QDialog(parent, xmilib::constants::kDefaultDialogFlags)
   , ui_()
   , shortcut_(shortcut)
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
