/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of dialog for creating/editing a combo


#include "stdafx.h"
#include "ComboDialog.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ComboDialog::ComboDialog(SPCombo combo, QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , combo_(combo)
{
   if (!combo)
      throw xmilib::Exception("%1(): combo is null.");
   ui_.setupUi(this);
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
