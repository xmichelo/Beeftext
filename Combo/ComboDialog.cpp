/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of dialog for creating/editing a combo
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  

#include "stdafx.h"
#include "ComboDialog.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


//**********************************************************************************************************************
/// \param[in] Combo The combo
/// \param[in] title The title to display in the dialog's title bar
/// \param[in] parent The parent widget of the dialog
/// \return true if and only if the user validated the dialog
//**********************************************************************************************************************
bool ComboDialog::run(SPCombo combo, QString const& title, QWidget* parent)
{
   return QDialog::Accepted == ComboDialog(combo, title, parent).exec();
}

//**********************************************************************************************************************
/// \param[in] combo The combo
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ComboDialog::ComboDialog(SPCombo combo, QString const& title, QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , combo_(combo)
{
   if (!combo)
      throw xmilib::Exception("%1(): combo is null.");
   ui_.setupUi(this);
   this->setWindowTitle(title);
   ui_.editName->setText(combo->name());
   ui_.editCombo->setText(combo->comboText());
   ui_.editSubstitution->setPlainText(combo->substitutionText());
}


//**********************************************************************************************************************
/// \return The combo
//**********************************************************************************************************************
SPCombo ComboDialog::combo() const
{
   return combo_;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::onActionOk()
{
   Q_ASSERT(combo_);
   combo_->setName(ui_.editName->text().trimmed());
   combo_->setComboText(ui_.editCombo->text().trimmed());
   combo_->setSubstitutionText(ui_.editSubstitution->toPlainText());
   this->accept();
}
