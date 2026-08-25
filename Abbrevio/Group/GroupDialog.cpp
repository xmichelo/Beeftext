/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo group dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "GroupDialog.h"
#include <XMiLib/Exception.h>
#include <XMiLib/XMiLibConstants.h>


//****************************************************************************************************************************************************
/// \param[in,out] group The group
/// \param[in] title The title for the dialog
/// \param[in] parent The parent widget of the dialog
//****************************************************************************************************************************************************
bool GroupDialog::run(SpGroup const &group, QString const &title, QWidget *parent) {
    return Accepted == GroupDialog(group, title, parent).exec();
}


//****************************************************************************************************************************************************
/// \param[in,out] group The group
/// \param[in] title The title for the dialog
/// \param[in] parent The parent widget of the dialog
//****************************************************************************************************************************************************
GroupDialog::GroupDialog(SpGroup const &group, QString const &title, QWidget *parent)
    : QDialog(parent, xmilib::constants::kDefaultDialogFlags), ui_(), group_(group) {
    if (!group_)
        throw xmilib::Exception("%1(): combo is null.");
    ui_.setupUi(this);

    connect(ui_.buttonOk, &QPushButton::clicked, this, &GroupDialog::onOK);
    connect(ui_.buttonCancel, &QPushButton::clicked, this, &GroupDialog::reject);
    connect(ui_.editName, &QLineEdit::textChanged, this, &GroupDialog::onNameChanged);

    ui_.editName->setText(group->name());
    ui_.editDescription->setPlainText(group->description());
    this->setWindowTitle(title);
    this->updateGui();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupDialog::onOK() {
    if (!this->isInputValid())
        return;
    group_->setName(ui_.editName->text());
    group_->setDescription(ui_.editDescription->toPlainText());
    this->accept();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupDialog::onNameChanged() const {
    this->updateGui();
}


//****************************************************************************************************************************************************
/// \return true if and only if the user input is acceptable
//****************************************************************************************************************************************************
bool GroupDialog::isInputValid() const {
    return (!ui_.editName->text().trimmed().isEmpty());
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupDialog::updateGui() const {
    ui_.buttonOk->setEnabled(this->isInputValid());
}
