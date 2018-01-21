/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of shortcut dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ShortcutDialog.h"
#include "BeeftextConstants.h"


//**********************************************************************************************************************
/// \param[in] shortcut The shortcut
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ShortcutDialog::ShortcutDialog(SPShortcut shortcut, QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , shortcut_(shortcut)
{
   ui_.setupUi(this);
   this->updateGui();
}


//**********************************************************************************************************************
/// \return The shortcut
//**********************************************************************************************************************
SPShortcut ShortcutDialog::shortcut() const
{
   return shortcut_;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ShortcutDialog::keyPressEvent(QKeyEvent *event)
{
   event->ignore(); // we intercept all key events
   SPShortcut shortcut = std::make_shared<Shortcut>(event->modifiers(), event->nativeVirtualKey(),
      event->nativeScanCode());
   if (!shortcut->isValid())
      return;
   shortcut_ = shortcut;
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ShortcutDialog::updateGui()
{
   ui_.editShortcut->setText(shortcut_ ? shortcut_->toString() : "");
   ui_.buttonOK->setEnabled(shortcut_.get());
}


