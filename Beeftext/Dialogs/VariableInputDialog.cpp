/// \file
/// \author 
///
/// \brief Implementation of dialog class for interactively providing the value of a variable
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "VariableInputDialog.h"
#include <XMiLib/XMiLibConstants.h>


//****************************************************************************************************************************************************
/// \param[in] description The description message displayed in the dialog
//****************************************************************************************************************************************************
VariableInputDialog::VariableInputDialog(QString const &description)
    : QInputDialog(nullptr, xmilib::constants::kDefaultDialogFlags) {
    this->setInputMode(TextInput);
    this->setLabelText(description);
    this->show();
}


//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void VariableInputDialog::showEvent(QShowEvent *event) {
    this->raise();
    this->activateWindow();
    QDialog::showEvent(event);
}


//****************************************************************************************************************************************************
/// \param[in] description The description message displayed in the dialog
/// \param[out] outUserInput The user input
//****************************************************************************************************************************************************
bool VariableInputDialog::run(QString const &description, QString &outUserInput) {
    VariableInputDialog dlg(description);
    if (Accepted != dlg.exec())
        return false;
    outUserInput = dlg.textValue();
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] event The event
//****************************************************************************************************************************************************
void VariableInputDialog::changeEvent(QEvent *event) {
    if ((event->type() == QEvent::ActivationChange) && !this->isActiveWindow())
        this->reject(); // when the dialog looses the focus, we dismiss is because we don't know where the input
    // focus can be now.
    QWidget::changeEvent(event);
}
