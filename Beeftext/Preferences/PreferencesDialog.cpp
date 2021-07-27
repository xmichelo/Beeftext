/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of preferences dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferencesDialog.h"
#include "Combo/ComboManager.h"
#include "Backup/BackupManager.h"
#include "Backup/BackupRestoreDialog.h"
#include "SensitiveApplicationManager.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <XMiLib/XMiLibConstants.h>


namespace {


QString const kExportFileName = "BeeftextPrefs.json"; ///< The default file name for export/import of preferences.


}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
PreferencesDialog::PreferencesDialog(QWidget* parent)
   : QDialog(parent, xmilib::constants::kDefaultDialogFlags),
     prefs_(PreferencesManager::instance())
{
   ui_.setupUi(this);
   ui_.spinDelayBetweenKeystrokes->setRange(PreferencesManager::minDelayBetweenKeystrokesMs(),
      PreferencesManager::maxDelayBetweenKeystrokesMs());
   panes_ = { ui_.paneBehavior, ui_.paneCombos, ui_.paneEmojis, ui_.paneAppearance };

   this->load();

   if (isInPortableMode())
      ui_.frameComboListFolder->setVisible(false);

   // We update the GUI when the combo list is saved to proper enable/disable the 'Restore Backup' button
   connect(&ComboManager::instance(), &ComboManager::comboListWasSaved, this, &PreferencesDialog::updateGui);

   ui_.tabPreferences->setCurrentIndex(0);
   QSize const size = QDialog::sizeHint();
   this->setMinimumSize(size);
   this->resize(size);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::load() const
{
   // ReSharper disable CppEntityAssignedButNoRead
   // ReSharper disable CppAssignedValueIsNeverUsed
   for (PrefsPane* pane: panes_)
      pane->load();

   QSignalBlocker blocker(nullptr);
   blocker = QSignalBlocker(ui_.spinDelayBetweenKeystrokes);
   ui_.spinDelayBetweenKeystrokes->setValue(prefs_.delayBetweenKeystrokesMs());
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(prefs_.comboListFolderPath()));
   ui_.checkAutoBackup->setChecked(prefs_.autoBackup());
   blocker = QSignalBlocker(ui_.checkUseCustomBackupLocation);
   ui_.checkUseCustomBackupLocation->setChecked(prefs_.useCustomBackupLocation());
   ui_.editCustomBackupLocation->setText(QDir::toNativeSeparators(prefs_.customBackupLocation()));
   blocker = QSignalBlocker(ui_.checkWriteDebugLogFile);
   ui_.checkWriteDebugLogFile->setChecked(prefs_.writeDebugLogFile());
   blocker = QSignalBlocker(ui_.checkUseLegacyCopyPaste);
   ui_.checkUseLegacyCopyPaste->setChecked(prefs_.useLegacyCopyPaste());
   blocker = QSignalBlocker(ui_.checkUseShiftInsertForPasting);
   ui_.checkUseShiftInsertForPasting->setChecked(prefs_.useShiftInsertForPasting());
   blocker = QSignalBlocker(ui_.checkUseCustomPowershellVersion);
   ui_.checkUseCustomPowershellVersion->setChecked(prefs_.useCustomPowershellVersion());
   blocker = QSignalBlocker(ui_.editCustomPowerShellPath);
   ui_.editCustomPowerShellPath->setText(QDir::toNativeSeparators(prefs_.customPowershellPath()));
   this->updateGui();
   // ReSharper restore CppAssignedValueIsNeverUsed
   // ReSharper restore CppEntityAssignedButNoRead
}


//**********************************************************************************************************************
/// \return true if the used picked Yes or No
/// \return false if the user selected Cancel
//**********************************************************************************************************************
bool PreferencesDialog::promptForAndRemoveAutoBackups()
{
   BackupManager& backupManager = BackupManager::instance();
   qint32 const reply = QMessageBox::question(this, tr("Delete Backup Files?"), tr("Do you want to delete all the "
         "backup files?"), QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel),
      QMessageBox::No);
   if (QMessageBox::Cancel == reply)
      return false;
   if (QMessageBox::Yes == reply)
      backupManager.removeAllBackups();
   return true;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::updateGui() const
{
   ui_.buttonRestoreBackup->setEnabled(BackupManager::instance().backupFileCount());

   QWidgetList widgets = { ui_.editCustomBackupLocation, ui_.buttonChangeCustomBackupLocation };
   for (QWidget* widget: widgets)
      widget->setEnabled(prefs_.useCustomBackupLocation());

   bool const customPowershell = ui_.checkUseCustomPowershellVersion->isChecked();
   ui_.editCustomPowerShellPath->setEnabled(customPowershell);
   ui_.buttonChangeCustomPowershellVersion->setEnabled(customPowershell);

}


//**********************************************************************************************************************
/// \return true if and only if the settings are consistant
//**********************************************************************************************************************
bool PreferencesDialog::validateInput()
{
   for (PrefsPane* pane: panes_)
      if (!pane->validateInput())
         return false;
   return true;
}


//**********************************************************************************************************************
/// \param[in] value The new value.
//**********************************************************************************************************************
void PreferencesDialog::onSpinDelayBetweenKeystrokesChanged(int value) const
{
   prefs_.setDelayBetweenKeystrokesMs(value);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onChangeComboListFolder()
{
   QString const path = QFileDialog::getExistingDirectory(this, tr("Select folder"), prefs_.comboListFolderPath());
   if (path.trimmed().isEmpty())
      return;
   if (!prefs_.setComboListFolderPath(path))
   {
      QMessageBox::critical(this, tr("Error"), tr("The location of the combo list folder could not be changed."));
      return;
   }
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(path));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onResetComboListFolder()
{
   if (QMessageBox::Yes != QMessageBox::question(this, tr("Reset Folder"), tr("Reset the combo list folder?")))
      return;
   previousComboListPath_ = prefs_.comboListFolderPath();
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(PreferencesManager::defaultComboListFolderPath()));
   ComboManager::instance().saveComboListToFile();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onOpenComboListFolder() const
{
   QString const path = QDir::fromNativeSeparators(ui_.editComboListFolder->text());
   if (QDir(path).exists())
      QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


//**********************************************************************************************************************
/// \param[in] value Is the radio button checked?
//**********************************************************************************************************************
void PreferencesDialog::onCheckAutoBackup(bool value)
{
   if (!value)
   {
      if (!this->promptForAndRemoveAutoBackups()) // did the user cancel?
      {
         QSignalBlocker const blocker(ui_.checkAutoBackup);
         ui_.checkAutoBackup->setChecked(true);
      }
   }
   prefs_.setAutoBackup(value);
   this->updateGui();
}


//**********************************************************************************************************************
/// \param[in] value The check state of the box.
//**********************************************************************************************************************
void PreferencesDialog::onCheckUseCustomBackupLocation(bool value) const
{
   prefs_.setUseCustomBackupLocation(value);
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onChangeCustomBackupLocation()
{
   QString const path = QFileDialog::getExistingDirectory(this, tr("Custom backup location"), 
      prefs_.customBackupLocation());
   if (path.isEmpty())
      return;
   ui_.editCustomBackupLocation->setText(QDir::toNativeSeparators(path));
   prefs_.setCustomBackupLocation(path);

}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onRestoreBackup()
{
   BackupRestoreDialog::run(this);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onResetToDefaultValues()
{
   if (QMessageBox::Yes != QMessageBox::question(this, tr("Reset Preferences"), tr("Are you sure you want to reset "
      "the preferences to their default values?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
      return;

   previousComboListPath_ = prefs_.comboListFolderPath();
   prefs_.reset();
   this->load();
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onResetWarnings()
{
   if (QMessageBox::Yes == QMessageBox::question(this, tr("Reset Warnings"), tr("Are you sure you want to reset "
      "all warnings?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
      prefs_.resetWarnings();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onClose()
{
   if (this->validateInput())
      this->close();
}

//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onEditSensitiveApplications()
{
   SensitiveApplicationManager::instance().runDialog(this);
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked?
//**********************************************************************************************************************
void PreferencesDialog::onCheckWriteDebugLogFile(bool checked) const
{
   prefs_.setWriteDebugLogFile(checked);
   xmilib::DebugLog& log = globals::debugLog();
   if (checked)
      log.enableLoggingToFile(globals::logFilePath());
   else
      log.disableLoggingToFile();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onCheckUseLegacyCopyPaste() const
{
   prefs_.setUseLegacyCopyPaste(ui_.checkUseLegacyCopyPaste->isChecked());
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onCheckUseShiftInsertForPasting() const
{
   prefs_.setUseShiftInsertForPasting(ui_.checkUseShiftInsertForPasting->isChecked());
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked.
//**********************************************************************************************************************
void PreferencesDialog::onCheckUseCustomPowerShellVersion(bool checked)
{
   if (checked)
   {
      QString const path = prefs_.customPowershellPath();
      QFileInfo const fi(path);
      if (path.isEmpty() || (!fi.exists()) || (!fi.isExecutable()))
      {
         this->onChangeCustomPowershellVersion();
         if ((path.isEmpty() || (!fi.exists()) || (!fi.isExecutable())))
         {
            QSignalBlocker blocker(ui_.checkUseCustomPowershellVersion);
            ui_.checkUseCustomPowershellVersion->setChecked(false);
         }
      }
   }
   prefs_.setUseCustomPowershellVersion(ui_.checkUseCustomPowershellVersion->isChecked());
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onChangeCustomPowershellVersion()
{
   QString const path = QFileDialog::getOpenFileName(this, tr("Select PowerShell executable"), QString(),
      tr("Executable files (*.exe);;All files (*.*)"));
   if (path.isEmpty())
      return;
   prefs_.setCustomPowershellPath(path);
   QSignalBlocker blocker(ui_.editCustomPowerShellPath);
   ui_.editCustomPowerShellPath->setText(QDir::toNativeSeparators(path));
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onExport()
{
   QString const path = QFileDialog::getSaveFileName(this, tr("Export Preferences"), 
      QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).absoluteFilePath(kExportFileName), 
      constants::jsonFileDialogFilter());
   if (path.isEmpty())
      return;
   if (!prefs_.save(path))
      QMessageBox::critical(this, tr("Error"), tr("An error occurred while exporting the preferences."));
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onImport()
{
   QString const path = QFileDialog::getOpenFileName(this, tr("Import Preferences"),
      QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).absoluteFilePath(kExportFileName),
      constants::jsonFileDialogFilter());
   if (path.isEmpty())
      return;
   if (!prefs_.load(path))
      QMessageBox::critical(this, tr("Error"), tr("An error occurred while importing the preferences."));
   this->load();
}
