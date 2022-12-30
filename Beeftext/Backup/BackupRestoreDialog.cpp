/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of backup restoration dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BackupRestoreDialog.h"
#include "BackupManager.h"
#include "BeeftextGlobals.h"
#include "Combo/ComboManager.h"
#include <XMiLib/Exception.h>
#include <XMiLib/XMiLibConstants.h>


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
/// \return true if and only if a backup was restored
//****************************************************************************************************************************************************
bool BackupRestoreDialog::run(QWidget *parent) {
    return Accepted == BackupRestoreDialog(parent).exec();
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//****************************************************************************************************************************************************
BackupRestoreDialog::BackupRestoreDialog(QWidget *parent)
    : QDialog(parent, xmilib::constants::kDefaultDialogFlags), ui_() {
    ui_.setupUi(this);

    connect(ui_.buttonRestore, &QPushButton::clicked, this, &BackupRestoreDialog::onButtonRestore);
    connect(ui_.buttonCancel, &QPushButton::clicked, this, &BackupRestoreDialog::reject);

    this->fillCombo();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void BackupRestoreDialog::onButtonRestore() {
    try {
        QString const path = ui_.comboBackup->currentData().toString();
        if (path.isEmpty())
            throw xmilib::Exception("Invalid backup file path.");
        QFile const file(path);
        if (!file.exists())
            throw xmilib::Exception("Backup file does not exist.");
        if (!ComboManager::instance().restoreBackup(path))
            throw xmilib::Exception("Could not restore backup file.");
        this->accept();
    }
    catch (xmilib::Exception const &e) {
        globals::debugLog().addError(QString("Internal error: %1(): %2").arg(__FUNCTION__).arg(e.qwhat()));
        QMessageBox::critical(this, tr("Error"), tr("The backup could not be restored."));
    }
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void BackupRestoreDialog::fillCombo() const {
    ui_.comboBackup->clear();
    QStringList backups = BackupManager::orderedBackupFilePaths();
    std::sort(backups.begin(), backups.end(), [](QString const &lhs, QString const &rhs) -> bool { return lhs > rhs; });
    for (QString const &path: backups)
        ui_.comboBackup->addItem(QDateTime::fromString(QFileInfo(path).fileName().left(18), "yyyyMMdd'_'HHmmsszzz")
            .toString("yyyy-MM-dd, HH:mm:ss"), path);
    ui_.comboBackup->setCurrentIndex(0);
}
