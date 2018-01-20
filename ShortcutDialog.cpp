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
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ShortcutDialog::ShortcutDialog(QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , shortcut_(nullptr)
{
   ui_.setupUi(this);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ShortcutDialog::keyPressEvent(QKeyEvent *event)
{
   event->ignore();
   qint32 key = event->key();
   if (key == Qt::Key_Control || key == Qt::Key_Shift || key == Qt::Key_Meta || key == Qt::Key_Alt || 
      key == Qt::Key_unknown) 
      return;
   shortcut_ = std::make_shared<Shortcut>(event->modifiers(), event->nativeVirtualKey(),
      event->nativeScanCode());
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


