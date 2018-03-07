/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo group dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboGroupDialog.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


//**********************************************************************************************************************
/// \param[in,out] group The group
/// \param[in] title The title for the dialog
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
bool ComboGroupDialog::run(SPComboGroup& group, QString const& title, QWidget* parent)
{
   return QDialog::Accepted == ComboGroupDialog(group, title, parent).exec();
}


//**********************************************************************************************************************
/// \param[in,out] group The group
/// \param[in] title The title for the dialog
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ComboGroupDialog::ComboGroupDialog(SPComboGroup& group, QString const& title, QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , group_(group)
{
   if (!group_)
      throw xmilib::Exception("%1(): combo is null.");
   ui_.setupUi(this);
   this->setWindowTitle(title);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupDialog::onActionOk()
{
   if (!this->isInputValid())
      return;
   group_->setName(ui_.editName->text());
   group_->setDescription(ui_.editDescription->toPlainText());
   this->accept();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupDialog::onNameChanged()
{
   this->updateGui();
}


//**********************************************************************************************************************
/// \return true if and only if the user input is acceptable
//**********************************************************************************************************************
bool ComboGroupDialog::isInputValid()
{
   return (!ui_.editName->text().trimmed().isEmpty());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupDialog::updateGui()
{
   bool const valid = this->isInputValid();
   ui_.actionOk->setEnabled(valid);
   ui_.buttonOk->setEnabled(valid);
}


