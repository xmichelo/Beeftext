/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of preferences dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferencesDialog.h"
#include "ShortcutDialog.h"
#include "Combo/ComboManager.h"
#include "I18nManager.h"
#include "Backup/BackupManager.h"
#include "Backup/BackupRestoreDialog.h"
#include "Update/UpdateManager.h"
#include "EmojiManager.h"
#include "SensitiveApplicationManager.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <XMiLib/XMiLibConstants.h>


namespace {


qint32 kUpdateCheckStatusLabelTimeoutMs = 3000; ///< The delay after which the update check status label is cleared
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
   this->updateCheckStatusTimer_.setSingleShot(true);
   connect(&updateCheckStatusTimer_, &QTimer::timeout, [&]() { ui_.labelUpdateCheckStatus->setText(QString()); });
   ui_.labelUpdateCheckStatus->setText(QString());
   ui_.spinDelayBetweenKeystrokes->setRange(PreferencesManager::minDelayBetweenKeystrokesMs(), 
      PreferencesManager::maxDelayBetweenKeystrokesMs());
   I18nManager::instance().fillLocaleCombo(*ui_.comboLocale);
   this->loadPreferences();
   if (isInPortableMode())
   {
      QWidgetList widgets = {ui_.checkAutoStart, ui_.frameComboListFolder};
      for (QWidget* widget: widgets)
         widget->setVisible(false);
   }

   // signal mappings for the 'Check now' button
   UpdateManager& updateManager = UpdateManager::instance();
   connect(ui_.buttonCheckNow, &QPushButton::clicked, &updateManager, &UpdateManager::checkForUpdate);
   connect(&updateManager, &UpdateManager::startedUpdateCheck, this, &PreferencesDialog::onUpdateCheckStarted);
   connect(&updateManager, &UpdateManager::finishedUpdateCheck, this, &PreferencesDialog::onUpdateCheckFinished);
   connect(&updateManager, &UpdateManager::updateIsAvailable, this, &PreferencesDialog::onUpdateIsAvailable);
   connect(&updateManager, &UpdateManager::noUpdateIsAvailable, this, &PreferencesDialog::onNoUpdateIsAvailable);
   connect(&updateManager, &UpdateManager::updateCheckFailed, this, &PreferencesDialog::onUpdateCheckFailed);

   // We update the GUI when the combo list is saved to proper enable/disable the 'Restore Backup' button
   connect(&ComboManager::instance(), &ComboManager::comboListWasSaved, this, &PreferencesDialog::updateGui);

   ui_.tabPreferences->setCurrentIndex(0);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::loadPreferences() const
{
   // ReSharper disable CppEntityAssignedButNoRead
   // ReSharper disable CppAssignedValueIsNeverUsed
   QSignalBlocker blocker(ui_.checkAutoCheckForUpdates);
   ui_.checkAutoCheckForUpdates->setChecked(prefs_.autoCheckForUpdates());
   blocker = QSignalBlocker(ui_.checkAutoStart);
   ui_.checkAutoStart->setChecked(prefs_.autoStartAtLogin());
   blocker = QSignalBlocker(ui_.checkPlaySoundOnCombo);
   SpShortcut shortcut = prefs_.appEnableDisableShortcut();
   blocker = QSignalBlocker(ui_.CheckAppEnableDisable);
   ui_.CheckAppEnableDisable->setChecked(prefs_.enableAppEnableDisableShortcut());
   ui_.editAppEnableDisableShortcut->setText(shortcut ? shortcut->toString() : "");
   ui_.checkPlaySoundOnCombo->setChecked(prefs_.playSoundOnCombo());
   blocker = QSignalBlocker(ui_.radioComboTriggerAuto);
   if (prefs_.useAutomaticSubstitution())
      ui_.radioComboTriggerAuto->setChecked(true);
   else
      ui_.radioComboTriggerManual->setChecked(true);
   shortcut = prefs_.comboTriggerShortcut();
   ui_.editComboTriggerShortcut->setText(shortcut ? shortcut->toString() : "");
   blocker = QSignalBlocker(ui_.checkEnableComboPicker);
   ui_.checkEnableComboPicker->setChecked(prefs_.comboPickerEnabled());
   shortcut = prefs_.comboPickerShortcut();
   ui_.editComboPickerShortcut->setText(shortcut ? shortcut->toString() : "");
   blocker = QSignalBlocker(ui_.checkEnableEmoji);
   ui_.checkEnableEmoji->setChecked(prefs_.emojiShortcodesEnabled());
   blocker = QSignalBlocker(ui_.editEmojiLeftDelimiter);
   ui_.editEmojiLeftDelimiter->setText(prefs_.emojiLeftDelimiter());
   blocker = QSignalBlocker(ui_.editEmojiRightDelimiter);
   ui_.editEmojiRightDelimiter->setText(prefs_.emojiRightDelimiter());
   blocker = QSignalBlocker(ui_.comboLocale);
   this->onRefreshLanguageList();
   blocker = QSignalBlocker(ui_.checkUseCustomTheme);
   ui_.checkUseCustomTheme->setChecked(prefs_.useCustomTheme());
   blocker = QSignalBlocker(ui_.spinDelayBetweenKeystrokes);
   ui_.spinDelayBetweenKeystrokes->setValue(prefs_.delayBetweenKeystrokesMs());
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(prefs_.comboListFolderPath()));
   ui_.checkAutoBackup->setChecked(prefs_.autoBackup());
   blocker = QSignalBlocker(ui_.checkWriteDebugLogFile);
   ui_.checkWriteDebugLogFile->setChecked(prefs_.writeDebugLogFile());
   blocker = QSignalBlocker(ui_.checkUseCustomSound);
   ui_.checkUseCustomSound->setChecked(prefs_.useCustomSound());
   ui_.editCustomSound->setText(QDir::toNativeSeparators(prefs_.customSoundPath()));
   this->updateGui();
   // ReSharper restore CppAssignedValueIsNeverUsed
   // ReSharper restore CppEntityAssignedButNoRead
}


//**********************************************************************************************************************
/// \param[in] status The status message
//**********************************************************************************************************************
void PreferencesDialog::setUpdateCheckStatus(QString const& status)
{
   updateCheckStatusTimer_.stop();
   ui_.labelUpdateCheckStatus->setText(status);
   updateCheckStatusTimer_.start(kUpdateCheckStatusLabelTimeoutMs);
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
/// \param[in] event The event
//**********************************************************************************************************************
void PreferencesDialog::changeEvent(QEvent* event)
{
   if (QEvent::LanguageChange == event->type())
   {
      ui_.retranslateUi(this);
      SpShortcut const shortcut = prefs_.comboTriggerShortcut();
      ui_.editComboTriggerShortcut->setText(shortcut ? shortcut->toString() : "");
   }
   QDialog::changeEvent(event);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::updateGui() const
{
   ui_.buttonRestoreBackup->setEnabled(BackupManager::instance().backupFileCount());

   QWidgetList widgets = { ui_.editComboTriggerShortcut, ui_.buttonChangeComboTriggerShortcut, 
      ui_.buttonResetComboTriggerShortcut };
   for (QWidget* const widget: widgets)
      widget->setEnabled(ui_.radioComboTriggerManual->isChecked());

   widgets = { ui_.buttonEmojiExcludedApps, ui_.labelEmojiLeftDelimiter, ui_.editEmojiLeftDelimiter,
      ui_.labelEmojiRightDelimiter, ui_.editEmojiRightDelimiter };
   for (QWidget* const widget: widgets)
      widget->setEnabled(ui_.checkEnableEmoji->isChecked());
   
   widgets = { ui_.labelComboPickerShortcut, ui_.editComboPickerShortcut, ui_.buttonChangeComboPickerShortcut,
      ui_.buttonResetComboPickerShortcut };
   for (QWidget* const widget: widgets)
      widget->setEnabled(ui_.checkEnableComboPicker->isChecked());

   ui_.frameCustomSound->setEnabled(ui_.checkPlaySoundOnCombo->isChecked());

   ui_.frameAppEnableDisableShortcut->setEnabled(ui_.CheckAppEnableDisable->isChecked());

   bool const useCustomSound = ui_.checkUseCustomSound->isChecked();
   widgets = { ui_.editCustomSound, ui_.buttonChangeCustomSound, ui_.buttonPlay };
   for (QWidget* const widget: widgets)
      widget->setEnabled(useCustomSound);
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked
//**********************************************************************************************************************
void PreferencesDialog::onCheckAutoCheckForUpdates(bool checked) const
{
   prefs_.setAutoCheckForUpdates(checked);
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked
//**********************************************************************************************************************
void PreferencesDialog::onCheckAutoStart(bool checked) const
{
   if (!isInPortableMode())
      prefs_.setAutoStartAtLogin(checked);
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked
//**********************************************************************************************************************
void PreferencesDialog::onCheckPlaySoundOnCombo(bool checked) const
{
   prefs_.setPlaySoundOnCombo(checked);
   this->updateGui();
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked?
//**********************************************************************************************************************
void PreferencesDialog::onCheckUseCustomSound(bool checked) const
{
   prefs_.setUseCustomSound(checked);
   ComboManager::instance().loadSoundFromPreferences();
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onChangeCustomSound() const
{
   QString const oldPath = prefs_.customSoundPath();
   QString const path = QFileDialog::getOpenFileName(nullptr, tr("Select custom sound file"),   
      oldPath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) : oldPath,
      tr("WAV files (*.wav);;All files (*.*)"));
   if (path.isEmpty())
      return;
   ui_.editCustomSound->setText(QDir::toNativeSeparators(path));
   prefs_.setCustomSoundPath(path);
   ComboManager::instance().loadSoundFromPreferences();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onPlaySoundButton() const
{
   if (QFileInfo(prefs_.customSoundPath()).exists())
      ComboManager::instance().playSound();
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked
//**********************************************************************************************************************
void PreferencesDialog::onRadioAutomaticComboTrigger(bool checked) const
{
   prefs_.setUseAutomaticSubstitution(checked);
   this->updateGui();
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked?
//**********************************************************************************************************************
void PreferencesDialog::onCheckEnableAppEnableDisableShortcut(bool checked) const
{
   prefs_.setEnableAppEnableDisableShortcut(checked);
   this->updateGui();
}


//**********************************************************************************************************************
/// \param[in] shortcut The shortcut.
/// \return true if and only if the user accepted the dialog
//**********************************************************************************************************************
bool runShortcutDialog(SpShortcut& shortcut)
{
   ShortcutDialog dlg(shortcut);
   if (QDialog::Accepted != dlg.exec())
      return false;
   shortcut = dlg.shortcut();
   return true;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onChangeAppEnableDisableShortcut() const
{
   SpShortcut shortcut = prefs_.appEnableDisableShortcut();
   if (!runShortcutDialog(shortcut))
      return;
   prefs_.setAppEnableDisableShortcut(shortcut);
   ui_.editAppEnableDisableShortcut->setText(shortcut ? shortcut->toString() : ""); 
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onResetAppEnableDisableShortcut() const
{
   SpShortcut const shortcut = PreferencesManager::defaultAppEnableDisableShortcut();
   prefs_.setAppEnableDisableShortcut(shortcut);
   ui_.editAppEnableDisableShortcut->setText(shortcut ? shortcut->toString() : ""); 
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onChangeComboTriggerShortcut() const
{
   SpShortcut shortcut = prefs_.comboTriggerShortcut();
   if (!runShortcutDialog(shortcut))
      return;
   prefs_.setComboTriggerShortcut(shortcut);
   ui_.editComboTriggerShortcut->setText(shortcut ? shortcut->toString() : "");   
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onResetComboTriggerShortcut() const
{
   SpShortcut const shortcut = PreferencesManager::defaultComboTriggerShortcut();
   prefs_.setComboTriggerShortcut(shortcut);
   ui_.editComboTriggerShortcut->setText(shortcut ? shortcut->toString() : "");   
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked
//**********************************************************************************************************************
void PreferencesDialog::onCheckEnableComboPicker(bool checked) const
{
   prefs_.setComboPickerEnabled(checked);
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onChangeComboPickerShortcut() const
{
   SpShortcut shortcut = prefs_.comboPickerShortcut();
   if (!runShortcutDialog(shortcut))
      return;
   prefs_.setComboPickerShortcut(shortcut);
   ui_.editComboPickerShortcut->setText(shortcut ? shortcut->toString() : "");
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onResetComboPickerShortcut() const
{
   SpShortcut const shortcut = PreferencesManager::defaultComboPickerShortcut();
   prefs_.setComboPickerShortcut(shortcut);
   ui_.editComboPickerShortcut->setText(shortcut ? shortcut->toString() : "");
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked?
//**********************************************************************************************************************
void PreferencesDialog::onCheckEnableEmojiShortcodes(bool checked) const
{
   prefs_.setEmojiShortcodeEnabled(checked);
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onEditEmojiExcludedApplications()
{
   EmojiManager::instance().runDialog(this);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onEmojiLeftDelimiterChanged(QString const&) const
{
   prefs_.setEmojiLeftDelimiter(ui_.editEmojiLeftDelimiter->text());
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onEmojiRightDelimiterChanged(QString const&) const
{
   prefs_.setEmojiRightDelimiter(ui_.editEmojiRightDelimiter->text());
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onComboLanguageValueChanged(int) const
{
   prefs_.setLocale(I18nManager::instance().getSelectedLocaleInCombo(*ui_.comboLocale));
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked?
//**********************************************************************************************************************
void PreferencesDialog::onCheckUseCustomTheme(bool checked) const
{
   prefs_.setUseCustomTheme(checked);
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
   QString const previousPath = QDir::fromNativeSeparators(ui_.editComboListFolder->text());
   QString const path = QFileDialog::getExistingDirectory(this, tr("Select folder"), previousPath);
   if (path.trimmed().isEmpty())
      return;
   previousComboListPath_ = prefs_.comboListFolderPath();
   prefs_.setComboListFolderPath(path);
   if (!ComboManager::instance().saveComboListToFile())
   {
      QMessageBox::critical(this, tr("Error"), tr("The location of the combo list folder could not be changed."));
      prefs_.setComboListFolderPath(previousPath);
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
   this->loadPreferences();
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
   this->close();
}


//**********************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
//**********************************************************************************************************************
void PreferencesDialog::onUpdateIsAvailable(SpLatestVersionInfo const& latestVersionInfo)
{
   this->setUpdateCheckStatus(latestVersionInfo ? tr("%1 v%2.%3 is available.").arg(constants::kApplicationName)
      .arg(latestVersionInfo->versionMajor()).arg(latestVersionInfo->versionMinor())
      : tr("A new version is available."));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onNoUpdateIsAvailable()
{
   this->setUpdateCheckStatus(tr("The software is up to date."));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onUpdateCheckStarted() const
{
   ui_.buttonCheckNow->setEnabled(false);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onUpdateCheckFinished() const
{
   ui_.buttonCheckNow->setEnabled(true);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onUpdateCheckFailed()
{
   this->setUpdateCheckStatus(tr("Update check failed."));
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
void PreferencesDialog::onOpenTranslationFolder()
{
   QString const path = globals::userTranslationRootFolderPath();
   if (QDir(path).exists())
      QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onRefreshLanguageList() const
{
   I18nManager& i18NManager = I18nManager::instance();
   QLocale const currentLocale = I18nManager::locale();
   i18NManager.refreshSupportedLocalesList();
   i18NManager.fillLocaleCombo(*ui_.comboLocale);
   QLocale const validatedLocale = i18NManager.validateLocale(currentLocale);
   I18nManager::selectLocaleInCombo(validatedLocale, *ui_.comboLocale);
   i18NManager.setLocale(validatedLocale);
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
   this->loadPreferences();
}
