/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of preferences dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferencesDialog.h"
#include "BeeftextGlobals.h"
#include <XMiLib/XMiLibConstants.h>


namespace {


QString const kExportFileName = "BeeftextPrefs.json"; ///< The default file name for export/import of preferences.


}


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//****************************************************************************************************************************************************
PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent, xmilib::constants::kDefaultDialogFlags), prefs_(PreferencesManager::instance()) {
    ui_.setupUi(this);

    connect(ui_.buttonResetWarnings, &QPushButton::clicked, this, &PreferencesDialog::onResetWarnings);
    connect(ui_.buttonImport, &QPushButton::clicked, this, &PreferencesDialog::onImport);
    connect(ui_.buttonExport, &QPushButton::clicked, this, &PreferencesDialog::onExport);
    connect(ui_.buttonDefaults, &QPushButton::clicked, this, &PreferencesDialog::onResetToDefaultValues);
    connect(ui_.buttonClose, &QPushButton::clicked, this, &PreferencesDialog::onClose);

    panes_ = { ui_.paneBehavior, ui_.paneCombos, ui_.paneEmojis, ui_.paneAppearance, ui_.paneAdvanced };
    this->load();
    ui_.tabPreferences->setCurrentIndex(0);
    QSize const size = QDialog::sizeHint();
    this->setMinimumSize(size);
    this->resize(size);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PreferencesDialog::load() const {
    for (PrefPane const *pane: panes_)
        pane->load();
}


//****************************************************************************************************************************************************
/// \return true if and only if the settings are consistent.
//****************************************************************************************************************************************************
bool PreferencesDialog::validateInput() {
    for (PrefPane *pane: panes_)
        if (!pane->validateInput())
            return false;
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void PreferencesDialog::changeEvent(QEvent *event) {
    if (QEvent::LanguageChange == event->type())
        ui_.retranslateUi(this);
    QDialog::changeEvent(event);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PreferencesDialog::onResetToDefaultValues() {
    if (QMessageBox::Yes != QMessageBox::question(this, tr("Reset Preferences"), tr("Are you sure you want to reset "
                                                                                    "the preferences to their default values?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
        return;

    previousComboListPath_ = prefs_.comboListFolderPath();
    prefs_.reset();
    this->load();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesDialog::onResetWarnings() {
    if (QMessageBox::Yes == QMessageBox::question(this, tr("Reset Warnings"), tr("Are you sure you want to reset "
                                                                                 "all warnings?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
        prefs_.resetWarnings();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PreferencesDialog::onClose() {
    if (this->validateInput())
        this->close();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesDialog::onExport() {
    QString const path = QFileDialog::getSaveFileName(this, tr("Export Preferences"), QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).absoluteFilePath(kExportFileName), globals::jsonFileDialogFilter());
    if (path.isEmpty())
        return;
    if (!prefs_.save(path))
        QMessageBox::critical(this, tr("Error"), tr("An error occurred while exporting the preferences."));
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesDialog::onImport() {
    QString const path = QFileDialog::getOpenFileName(this, tr("Import Preferences"), QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).absoluteFilePath(kExportFileName), globals::jsonFileDialogFilter());
    if (path.isEmpty())
        return;
    if (!prefs_.load(path))
        QMessageBox::critical(this, tr("Error"), tr("An error occurred while importing the preferences."));
    this->load();
}
