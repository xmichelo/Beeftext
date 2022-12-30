/// \file
/// \author 
///
/// \brief Implementation of the advanced preferences pane.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PrefPaneAdvanced.h"
#include "Combo/ComboManager.h"
#include "Backup/BackupRestoreDialog.h"
#include "Backup/BackupManager.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"
#include <XMiLib/Exception.h>


//****************************************************************************************************************************************************
///\ param[in] parent The parent widget of the pane.
//****************************************************************************************************************************************************
PrefPaneAdvanced::PrefPaneAdvanced(QWidget *parent)
    : PrefPane(parent)
    , prefs_(PreferencesManager::instance()) {
    ui_.setupUi(this);
    ui_.spinDelayBetweenKeystrokes->setRange(PreferencesManager::minDelayBetweenKeystrokesMs(), PreferencesManager::maxDelayBetweenKeystrokesMs());
    if (isInPortableMode())
        ui_.frameComboListFolder->setVisible(false);

    // We update the GUI when the combo list is saved to properly enable/disable the 'Restore Backup' button
    connect(&ComboManager::instance(), &ComboManager::comboListWasSaved, this, &PrefPaneAdvanced::updateGui);

    connect(ui_.buttonChangeComboListFolder, &QPushButton::clicked, this, &PrefPaneAdvanced::onChangeComboListFolder);
    connect(ui_.buttonChangeCustomBackupLocation, &QPushButton::clicked, this, &PrefPaneAdvanced::onChangeCustomBackupLocation);
    connect(ui_.buttonChangeCustomPowershellVersion, &QPushButton::clicked, this, &PrefPaneAdvanced::onChangeCustomPowershellVersion);
    connect(ui_.buttonChangeComboListFolder, &QPushButton::clicked, this, &PrefPaneAdvanced::onChangeComboListFolder);
    connect(ui_.buttonExcludedApplications, &QPushButton::clicked, this, &PrefPaneAdvanced::onEditExcludedApplications);
    connect(ui_.buttonOpenComboListFolder, &QPushButton::clicked, this, &PrefPaneAdvanced::onOpenComboListFolder);
    connect(ui_.buttonResetComboListFolder, &QPushButton::clicked, this, &PrefPaneAdvanced::onResetComboListFolder);
    connect(ui_.buttonRestoreBackup, &QPushButton::clicked, this, &PrefPaneAdvanced::onRestoreBackup);
    connect(ui_.buttonSensitiveApplications, &QPushButton::clicked, this, &PrefPaneAdvanced::onEditSensitiveApplications);
    connect(ui_.checkAutoBackup, &QCheckBox::toggled, this, &PrefPaneAdvanced::onCheckAutoBackup);
    connect(ui_.checkRestoreClipboardAfterSubstitution, &QCheckBox::toggled, this, &PrefPaneAdvanced::onCheckRestoreClipboardAfterSubstitution);
    connect(ui_.checkUseCustomBackupLocation, &QCheckBox::toggled, this, &PrefPaneAdvanced::onCheckUseCustomBackupLocation);
    connect(ui_.checkUseCustomPowershellVersion, &QCheckBox::toggled, this, &PrefPaneAdvanced::onCheckUseCustomPowerShellVersion);
    connect(ui_.checkUseLegacyCopyPaste, &QCheckBox::toggled, this, &PrefPaneAdvanced::onCheckUseLegacyCopyPaste);
    connect(ui_.checkUseShiftInsertForPasting, &QCheckBox::toggled, this, &PrefPaneAdvanced::onCheckUseShiftInsertForPasting);
    connect(ui_.checkWriteDebugLogFile, &QCheckBox::toggled, this, &PrefPaneAdvanced::onCheckWriteDebugLogFile);
    connect(ui_.spinDelayBetweenKeystrokes, &QSpinBox::valueChanged, this, &PrefPaneAdvanced::onSpinDelayBetweenKeystrokesChanged);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneAdvanced::load() const {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
    QSignalBlocker blocker(ui_.spinDelayBetweenKeystrokes);
    ui_.spinDelayBetweenKeystrokes->setValue(prefs_.delayBetweenKeystrokesMs());
    ui_.editComboListFolder->setText(QDir::toNativeSeparators(prefs_.comboListFolderPath()));
    blocker = QSignalBlocker(ui_.checkWriteDebugLogFile);
    ui_.checkWriteDebugLogFile->setChecked(prefs_.writeDebugLogFile());
    blocker = QSignalBlocker(ui_.checkUseLegacyCopyPaste);
    ui_.checkUseLegacyCopyPaste->setChecked(prefs_.useLegacyCopyPaste());
    blocker = QSignalBlocker(ui_.checkRestoreClipboardAfterSubstitution);
    ui_.checkRestoreClipboardAfterSubstitution->setChecked(prefs_.restoreClipboardAfterSubstitution());
    blocker = QSignalBlocker(ui_.checkUseShiftInsertForPasting);
    ui_.checkUseShiftInsertForPasting->setChecked(prefs_.useShiftInsertForPasting());
    blocker = QSignalBlocker(ui_.checkUseCustomPowershellVersion);
    ui_.checkUseCustomPowershellVersion->setChecked(prefs_.useCustomPowershellVersion());
    blocker = QSignalBlocker(ui_.editCustomPowerShellPath);
    ui_.editCustomPowerShellPath->setText(QDir::toNativeSeparators(prefs_.customPowershellPath()));
    ui_.checkAutoBackup->setChecked(prefs_.autoBackup());
    blocker = QSignalBlocker(ui_.checkUseCustomBackupLocation);
    ui_.checkUseCustomBackupLocation->setChecked(prefs_.useCustomBackupLocation());
    ui_.editCustomBackupLocation->setText(QDir::toNativeSeparators(prefs_.customBackupLocation()));
#pragma clang diagnostic pop

    this->updateGui();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneAdvanced::updateGui() const {
    bool const customPowershell = ui_.checkUseCustomPowershellVersion->isChecked();
    ui_.editCustomPowerShellPath->setEnabled(customPowershell);
    ui_.buttonChangeCustomPowershellVersion->setEnabled(customPowershell);
    ui_.buttonRestoreBackup->setEnabled(BackupManager::instance().backupFileCount());
    QWidgetList widgets = { ui_.editCustomBackupLocation, ui_.buttonChangeCustomBackupLocation };
    for (QWidget *widget: widgets)
        widget->setEnabled(prefs_.useCustomBackupLocation());
}


//****************************************************************************************************************************************************
/// \param[in] value The new value.
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onSpinDelayBetweenKeystrokesChanged(int value) const {
    prefs_.setDelayBetweenKeystrokesMs(value);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onChangeComboListFolder() {
    QString const errorTitle = tr("Error");
    QString const errorMsg = tr("The location of the combo list folder could not be changed.");
    QFile oldFile = QDir(prefs_.comboListFolderPath()).absoluteFilePath(ComboList::defaultFileName);
    try {
        QString const path = QFileDialog::getExistingDirectory(this, tr("Select folder"), prefs_.comboListFolderPath());
        if (path.trimmed().isEmpty())
            return;
        QDir const dir(path);
        if (!dir.exists(ComboList::defaultFileName)) {
            if (!prefs_.setComboListFolderPath(path))
                throw xmilib::Exception(errorMsg);
            ui_.editComboListFolder->setText(QDir::toNativeSeparators(path));
            oldFile.remove();
            return;
        }

        switch (threeOptionsDialog(this, QMessageBox::Warning, tr("Overwrite"), tr("The selected folder "
                                                                                   "already contains a combo list file do you want to overwrite this list or to replace the current combo list "
                                                                                   "With the content of this file?"), tr("Overwrite file"), tr("Replace Current List"), tr("Cancel"), 0, 2)) {
        case 1: // replace current combos
            if (!ComboManager::instance().restoreBackup(dir.absoluteFilePath(ComboList::defaultFileName)))
                throw xmilib::Exception(errorMsg);
            [[fallthrough]];
        case 0: // overwrite file
            if (!prefs_.setComboListFolderPath(path))
                throw xmilib::Exception(errorMsg);
            ui_.editComboListFolder->setText(QDir::toNativeSeparators(path));
            oldFile.remove();
            break;
        case 2:
        default:
            return;
        }
    }
    catch (xmilib::Exception const &e) {
        QMessageBox::critical(this, errorTitle, e.qwhat());
    }
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onResetComboListFolder() {
    if (QMessageBox::Yes != QMessageBox::question(this, tr("Reset Folder"), tr("Reset the combo list folder?")))
        return;
    //previousComboListPath_ = prefs_.comboListFolderPath();
    QString const path = PreferencesManager::defaultComboListFolderPath();
    if (!prefs_.setComboListFolderPath(path)) {
        QMessageBox::critical(this, tr("Error"), tr("The location of the combo list folder could not be reset."));
        return;
    }
    ui_.editComboListFolder->setText(QDir::toNativeSeparators(path));
    ComboManager::instance().saveComboListToFile();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onOpenComboListFolder() const {
    QString const path = QDir::fromNativeSeparators(ui_.editComboListFolder->text());
    if (QDir(path).exists())
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


//****************************************************************************************************************************************************
/// \param[in] checked Is the check box checked?
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onCheckWriteDebugLogFile(bool checked) const {
    prefs_.setWriteDebugLogFile(checked);
    xmilib::DebugLog &log = globals::debugLog();
    if (checked)
        log.enableLoggingToFile(globals::logFilePath());
    else
        log.disableLoggingToFile();
}


//****************************************************************************************************************************************************
/// \param[in] checked Is the check box checked?
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onCheckUseLegacyCopyPaste(bool checked) const {
    prefs_.setUseLegacyCopyPaste(checked);
}


//****************************************************************************************************************************************************
/// \param[in] checked The value for the 'Restore clipboard after substitution' check box.
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onCheckRestoreClipboardAfterSubstitution(bool checked) const {
    prefs_.setRestoreClipboardAfterSubstitution(checked);
}


//****************************************************************************************************************************************************
/// \param[in] checked Is the check box checked?
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onCheckUseShiftInsertForPasting(bool checked) const {
    prefs_.setUseShiftInsertForPasting(checked);
}


//****************************************************************************************************************************************************
/// \param[in] checked Is the check box checked.
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onCheckUseCustomPowerShellVersion(bool checked) {
    if (checked) {
        QString const path = prefs_.customPowershellPath();
        QFileInfo const fi(path);
        if (path.isEmpty() || (!fi.exists()) || (!fi.isExecutable())) {
            this->onChangeCustomPowershellVersion();
            if ((path.isEmpty() || (!fi.exists()) || (!fi.isExecutable()))) {
                QSignalBlocker blocker(ui_.checkUseCustomPowershellVersion);
                ui_.checkUseCustomPowershellVersion->setChecked(false);
            }
        }
    }
    prefs_.setUseCustomPowershellVersion(ui_.checkUseCustomPowershellVersion->isChecked());
    this->updateGui();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onChangeCustomPowershellVersion() {
    QString const path = QFileDialog::getOpenFileName(this, tr("Select PowerShell executable"), QString(), tr("Executable files (*.exe);;All files (*.*)"));
    if (path.isEmpty())
        return;
    prefs_.setCustomPowershellPath(path);
    QSignalBlocker blocker(ui_.editCustomPowerShellPath);
    ui_.editCustomPowerShellPath->setText(QDir::toNativeSeparators(path));
}


//****************************************************************************************************************************************************
/// \param[in] value Is the radio button checked?
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onCheckAutoBackup(bool value) {
    if (!value) {
        if (!this->promptForAndRemoveAutoBackups()) // did the user cancel?
        {
            QSignalBlocker const blocker(ui_.checkAutoBackup);
            ui_.checkAutoBackup->setChecked(true);
        }
    }
    prefs_.setAutoBackup(value);
    this->updateGui();
}


//****************************************************************************************************************************************************
/// \param[in] value The check state of the box.
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onCheckUseCustomBackupLocation(bool value) const {
    prefs_.setUseCustomBackupLocation(value);
    this->updateGui();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onChangeCustomBackupLocation() {
    QString const path = QFileDialog::getExistingDirectory(this, tr("Custom backup location"), prefs_.customBackupLocation());
    if (path.isEmpty())
        return;
    ui_.editCustomBackupLocation->setText(QDir::toNativeSeparators(path));
    prefs_.setCustomBackupLocation(path);

}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onRestoreBackup() {
    BackupRestoreDialog::run(this);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onEditSensitiveApplications() {
    globals::sensitiveApplications().runDialog(this);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneAdvanced::onEditExcludedApplications() {
    globals::excludedApplications().runDialog(this);
}


//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void PrefPaneAdvanced::changeEvent(QEvent *event) {
    if (QEvent::LanguageChange == event->type())
        ui_.retranslateUi(this);
    PrefPane::changeEvent(event);
}


//****************************************************************************************************************************************************
/// \return true if the used picked Yes or No
/// \return false if the user selected Cancel
//****************************************************************************************************************************************************
bool PrefPaneAdvanced::promptForAndRemoveAutoBackups() {
    BackupManager const &backupManager = BackupManager::instance();
    qint32 const reply = QMessageBox::question(this, tr("Delete Backup Files?"), tr("Do you want to delete all the "
                                                                                    "backup files?"), QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel), QMessageBox::No);
    if (QMessageBox::Cancel == reply)
        return false;
    if (QMessageBox::Yes == reply)
        backupManager.removeAllBackups();
    return true;
}
