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
}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
PreferencesDialog::PreferencesDialog(QWidget* parent)
   : QDialog(parent, xmilib::constants::kDefaultDialogFlags),
     ui_(),
     prefs_(PreferencesManager::instance()),
     triggerShortcut_(nullptr)
{
   ui_.setupUi(this);
   this->updateCheckStatusTimer_.setSingleShot(true);
   connect(&updateCheckStatusTimer_, &QTimer::timeout, [&]() { ui_.labelUpdateCheckStatus->setText(QString()); });
   ui_.labelUpdateCheckStatus->setText(QString());
   ui_.checkAutoStart->setText(tr("&Automatically start %1 at login").arg(constants::kApplicationName));
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

   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::loadPreferences()
{
   ui_.checkAutoCheckForUpdates->setChecked(prefs_.autoCheckForUpdates());
   ui_.checkAutoStart->setChecked(prefs_.autoStartAtLogin());
   ui_.checkPlaySoundOnCombo->setChecked(prefs_.playSoundOnCombo());
   if (prefs_.useAutomaticSubstitution())
      ui_.radioComboTriggerAuto->setChecked(true);
   else
      ui_.radioComboTriggerManual->setChecked(true);
   ui_.checkEnableEmoji->setChecked(prefs_.emojiShortcodesEnabled());
   ui_.editEmojiLeftDelimiter->setText(prefs_.emojiLeftDelimiter());
   ui_.checkEmojiRightDelimiter->setChecked(prefs_.useEmojiRightDelimiter());
   ui_.editEmojiRightDelimiter->setText(prefs_.emojiRightDelimiter());
   I18nManager::selectLocaleInCombo(prefs_.locale(), *ui_.comboLocale);
   ui_.checkUseClipboardForComboSubstitution->setChecked(prefs_.useClipboardForComboSubstitution());
   ui_.spinDelayBetweenKeystrokes->setValue(prefs_.delayBetweenKeystrokesMs());
   ui_.checkUseCustomTheme->setChecked(prefs_.useCustomTheme());
   triggerShortcut_ = prefs_.comboTriggerShortcut();
   ui_.editShortcut->setText(triggerShortcut_ ? triggerShortcut_->toString() : "");
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(prefs_.comboListFolderPath()));
   ui_.checkAutoBackup->setChecked(prefs_.autoBackup());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::savePreferences()
{
   bool const autoBackup = ui_.checkAutoBackup->isChecked();
   if (prefs_.autoBackup() && !autoBackup && BackupManager::instance().backupFileCount())
      if (!this->promptForAndRemoveAutoBackups())
         return; // the user canceled

   prefs_.setAutoCheckForUpdates(ui_.checkAutoCheckForUpdates->isChecked());
   if (!isInPortableMode())
      prefs_.setAutoStartAtLogin(ui_.checkAutoStart->isChecked());
   prefs_.setPlaySoundOnCombo(ui_.checkPlaySoundOnCombo->isChecked());
   prefs_.setUseAutomaticSubstitution(ui_.radioComboTriggerAuto->isChecked());
   prefs_.setComboTriggerShortcut(triggerShortcut_ ? triggerShortcut_ :
      PreferencesManager::defaultComboTriggerShortcut());
   prefs_.setEmojiShortcodeEnabled(ui_.checkEnableEmoji->isChecked());
   prefs_.setEmojiLeftDelimiter(ui_.editEmojiLeftDelimiter->text());
   prefs_.setUseEmojiRightDelimiter(ui_.checkEmojiRightDelimiter->isChecked());
   prefs_.setEmojiRightDelimiter(ui_.editEmojiRightDelimiter->text());
   prefs_.setLocale(I18nManager::instance().getSelectedLocaleInCombo(*ui_.comboLocale));
   prefs_.setUseCustomTheme(ui_.checkUseCustomTheme->isChecked());
   prefs_.setUseClipboardForComboSubstitution(ui_.checkUseClipboardForComboSubstitution->isChecked());
   prefs_.setDelayBetweenKeystrokesMs(ui_.spinDelayBetweenKeystrokes->value());
   if (!isInPortableMode())
   {
      if (this->validateComboListFolderPath())
         prefs_.setComboListFolderPath(QDir::fromNativeSeparators(ui_.editComboListFolder->text()));
   }
   prefs_.setAutoBackup(autoBackup);
}


//**********************************************************************************************************************
/// \return true if and only if the combo list folder is valid (i.e. it exists and we could save the combo list there)
//**********************************************************************************************************************
bool PreferencesDialog::validateComboListFolderPath()
{
   if (isInPortableMode())
      return true;
   QString const& path = QDir(QDir::fromNativeSeparators(ui_.editComboListFolder->text()))
      .absoluteFilePath(ComboList::defaultFileName);
   bool const result = ComboManager::instance().comboListRef().save(path, true);
   if (!result)
   {
      globals::debugLog().addError(QString("The combo list folder could not be changed to '%1'")
         .arg(QDir::toNativeSeparators(path)));
      QMessageBox::critical(this, tr("Error"), tr("The combo list folder could not be changed. "
         "The other settings were saved successfully."));
   }
   return result;
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
      ui_.editShortcut->setText(shortcut ? shortcut->toString() : "");
   }
   QDialog::changeEvent(event);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onActionResetToDefaultValues()
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
void PreferencesDialog::onActionChangeComboListFolder()
{
   QString const previousPath = QDir::fromNativeSeparators(ui_.editComboListFolder->text());
   QString const path = QFileDialog::getExistingDirectory(this, tr("Select folder"), previousPath);
   if (path.trimmed().isEmpty())
      return;
   previousComboListPath_ = prefs_.comboListFolderPath();
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(path));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onActionResetComboListFolder()
{
   previousComboListPath_ = prefs_.comboListFolderPath();
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(PreferencesManager::defaultComboListFolderPath()));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onActionChangeShortcut()
{
   ShortcutDialog dlg(triggerShortcut_);
   if (Accepted != dlg.exec())
      return;
   triggerShortcut_ = dlg.shortcut();
   ui_.editShortcut->setText(triggerShortcut_->toString());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onActionResetComboTriggerShortcut()
{
   triggerShortcut_ = PreferencesManager::defaultComboTriggerShortcut();
   ui_.editShortcut->setText(triggerShortcut_ ? triggerShortcut_->toString() : "");
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onActionRestoreBackup()
{
   BackupRestoreDialog::run(this);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onActionResetWarnings()
{
   if (QMessageBox::Yes == QMessageBox::question(this, tr("Reset Warnings"), tr("Are you sure you want to reset "
      "all warnings?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
      prefs_.resetWarnings();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onActionOk()
{
   this->savePreferences();
   this->accept();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::onActionApply()
{
   this->savePreferences();
   this->updateGui();
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
void PreferencesDialog::onActionEditSensitiveApplications()
{
   SensitiveApplicationManager::instance().runDialog(this);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesDialog::onActionEditEmojiExcludedApplications()
{
   EmojiManager::instance().runDialog(this);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesDialog::updateGui() const
{
   bool const manualTrigger = ui_.radioComboTriggerManual->isChecked();
   ui_.editShortcut->setEnabled(manualTrigger);
   ui_.buttonChangeShortcut->setEnabled(manualTrigger);
   ui_.buttonResetComboTriggerShortcut->setEnabled(manualTrigger);
   ui_.buttonRestoreBackup->setEnabled(BackupManager::instance().backupFileCount());
   bool const useClipboard = ui_.checkUseClipboardForComboSubstitution->isChecked();
   ui_.buttonSensitiveApplications->setEnabled(useClipboard);
   bool const emojiEnabled = ui_.checkEnableEmoji->isChecked();
   ui_.buttonEmojiExcludedApps->setEnabled(emojiEnabled);
   ui_.labelEmojiLeftDelimiter->setEnabled(emojiEnabled);
   ui_.editEmojiLeftDelimiter->setEnabled(emojiEnabled);
   ui_.checkEmojiRightDelimiter->setEnabled(emojiEnabled);
   bool const useEmojiRightDelimiter = emojiEnabled && ui_.checkEmojiRightDelimiter->isChecked();
   ui_.editEmojiRightDelimiter->setEnabled(useEmojiRightDelimiter);
}
