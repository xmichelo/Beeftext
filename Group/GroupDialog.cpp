/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo group dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "GroupDialog.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


//**********************************************************************************************************************
/// \param[in,out] group The group
/// \param[in] title The title for the dialog
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
bool GroupDialog::run(SPGroup& group, QString const& title, QWidget* parent)
{
   return QDialog::Accepted == GroupDialog(group, title, parent).exec();
}


//**********************************************************************************************************************
/// \param[in,out] group The group
/// \param[in] title The title for the dialog
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
GroupDialog::GroupDialog(SPGroup& group, QString const& title, QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , group_(group)
{
   if (!group_)
      throw xmilib::Exception("%1(): combo is null.");
   ui_.setupUi(this);
   ui_.editName->setText(group->name());
   ui_.editDescription->setPlainText(group->description());
   this->setWindowTitle(title);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void GroupDialog::onActionOk()
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
void GroupDialog::onNameChanged()
{
   this->updateGui();
}


//**********************************************************************************************************************
/// \return true if and only if the user input is acceptable
//**********************************************************************************************************************
bool GroupDialog::isInputValid()
{
   return (!ui_.editName->text().trimmed().isEmpty());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void GroupDialog::updateGui()
{
   bool const valid = this->isInputValid();
   ui_.actionOk->setEnabled(valid);
   ui_.buttonOk->setEnabled(valid);
}


