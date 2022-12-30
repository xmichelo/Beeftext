/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of the 'About' dialog class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "AboutDialog.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"
#include <XMiLib/XMiLibConstants.h>


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//****************************************************************************************************************************************************
AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent, xmilib::constants::kDefaultDialogFlags)
    , ui_() {
    ui_.setupUi(this);
    connect(ui_.buttonClose, &QPushButton::clicked, this, &AboutDialog::accept);
    this->completeText();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void AboutDialog::completeText() const {
    QString const hexColor = colorToHex(constants::blueBeeftextColor, false);
    ui_.labelText->setText(ui_.labelText->text().arg(constants::kVersionNumber.major()).arg(constants::kVersionNumber.minor())
        .arg(isInPortableMode() ? tr("Portable Edition") : "").arg(QString(__DATE__).right(4))
        .arg(hexColor));
    ui_.labelTranslators->setText(ui_.labelTranslators->text().arg(hexColor));
    ui_.labelBuildInfo->setText(globals::getBuildInfo());
}


//****************************************************************************************************************************************************
/// \param[in] event The event
//****************************************************************************************************************************************************
void AboutDialog::changeEvent(QEvent *event) {
    if (QEvent::LanguageChange == event->type()) {
        ui_.retranslateUi(this);
        this->completeText();
    }
    QDialog::changeEvent(event);
}
