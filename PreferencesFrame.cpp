/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preferences frame
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferencesFrame.h"
#include "UpdateManager.h"
#include "InputManager.h"
#include "I18nManager.h"
#include "Combo/ComboManager.h"
#include "ShortcutDialog.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"
#include "MainWindow.h"


using namespace xmilib;


namespace {
   qint32 kUpdateCheckStatusLabelTimeoutMs = 3000; ///< The delay after which the update check status label is cleared
}

//**********************************************************************************************************************
/// \param[in] parent The parent widget of the frame
//**********************************************************************************************************************
PreferencesFrame::PreferencesFrame(QWidget* parent)
   : QFrame(parent)
   , prefs_(PreferencesManager::instance())
{
   ui_.setupUi(this);
   this->updateCheckStatusTimer_.setSingleShot(true);
   connect(&updateCheckStatusTimer_, &QTimer::timeout, [&]() { ui_.labelUpdateCheckStatus->setText(QString()); });
   ui_.labelUpdateCheckStatus->setText(QString());
   ui_.checkAutoStart->setText(tr("&Automatically start %1 at login").arg(constants::kApplicationName));
   this->loadPreferences();
   this->applyAutoStartPreference(); // we ensure autostart is properly setup
   this->applyThemePreference(); // we apply the custom theme if the user selected it
   I18nManager::fillLocaleCombo(*ui_.comboLocale);
   I18nManager::selectLocaleInCombo(I18nManager::instance().locale(), *ui_.comboLocale);
   if (isInPortableMode())
   {
      QWidgetList widgets = { ui_.checkAutoStart, ui_.frameComboListFolder };
      for (QWidget* widget : widgets)
         widget->setVisible(false);
   }

   // signal mappings for the 'Check now' button
   UpdateManager& updateManager = UpdateManager::instance();
   connect(ui_.buttonCheckNow, &QPushButton::clicked, &updateManager, &UpdateManager::checkForUpdate);
   connect(&updateManager, &UpdateManager::startedUpdateCheck, [&]() { ui_.buttonCheckNow->setEnabled(false); });
   connect(&updateManager, &UpdateManager::finishedUpdateCheck, [&]() { ui_.buttonCheckNow->setEnabled(true); });
   connect(&updateManager, &UpdateManager::updateIsAvailable, this, &PreferencesFrame::onUpdateIsAvailable);
   connect(&updateManager, &UpdateManager::noUpdateIsAvailable, this, &PreferencesFrame::onNoUpdateIsAvailable);
   connect(&updateManager, &UpdateManager::updateCheckFailed, this, &PreferencesFrame::onUpdateCheckFailed);

   this->updateGuiState();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::loadPreferences() const
{
   QSignalBlocker blockers[] =  { QSignalBlocker(ui_.checkPlaySoundOnCombo),
      QSignalBlocker(ui_.checkAutoCheckForUpdates), QSignalBlocker(ui_.checkUseClipboardForComboSubstitution),
      QSignalBlocker(ui_.checkAutoStart), QSignalBlocker(ui_.radioComboTriggerAuto), 
      QSignalBlocker(ui_.radioComboTriggerManual), QSignalBlocker(ui_.checkAutoBackup) }; // Temporarily Block signals emission by the controls
   ui_.checkPlaySoundOnCombo->setChecked(prefs_.playSoundOnCombo());
   ui_.checkAutoCheckForUpdates->setChecked(prefs_.autoCheckForUpdates());
   ui_.checkUseClipboardForComboSubstitution->setChecked(prefs_.useClipboardForComboSubstitution());
   ui_.checkAutoStart->setChecked(prefs_.autoStartAtLogin());
   ui_.checkUseCustomTheme->setChecked(prefs_.useCustomTheme());
   if (prefs_.useAutomaticSubstitution())
      ui_.radioComboTriggerAuto->setChecked(true);
   else
      ui_.radioComboTriggerManual->setChecked(true);
   SPShortcut const shortcut = InputManager::instance().comboTriggerShortcut();
   ui_.editShortcut->setText(shortcut ? shortcut->toString() : "");
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(prefs_.comboListFolderPath()));
   ui_.checkAutoBackup->setChecked(prefs_.autoBackup());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::applyAutoStartPreference() const
{
   if (prefs_.autoStartAtLogin())
   {
      if (!registerApplicationForAutoStart())
         globals::debugLog().addWarning("Could not register the application for automatic startup on login.");
   }
   else
      unregisterApplicationFromAutoStart();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::applyThemePreference() const
{
   qApp->setStyleSheet(prefs_.useCustomTheme() ? constants::kStyleSheet : QString());
}


//**********************************************************************************************************************
/// \param[in] folderPath The new path of the folder
/// \param[in] previousPath The path of the previous folder, to restore in case the new one does not work
//**********************************************************************************************************************
void PreferencesFrame::applyComboListFolderPreference(QString const& folderPath, QString const& previousPath)
{
   if (isInPortableMode())
      return;
   PreferencesManager& prefs = PreferencesManager::instance();
   prefs.setComboListFolderPath(folderPath);
   QString err;
   if (!ComboManager::instance().saveComboListToFile(&err))
   {
      prefs.setComboListFolderPath(previousPath);
      globals::debugLog().addError(QString("The combo list folder could not be changed to '%1'").arg(folderPath));
      QMessageBox::critical(this, tr("Error"), tr("The combo list folder could not be changed."));
      return;
   }
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(folderPath));
}


//**********************************************************************************************************************
/// \param[in] status The status message
//**********************************************************************************************************************
void PreferencesFrame::setUpdateCheckStatus(QString const& status)
{
   updateCheckStatusTimer_.stop();
   ui_.labelUpdateCheckStatus->setText(status);
   updateCheckStatusTimer_.start(kUpdateCheckStatusLabelTimeoutMs);
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void PreferencesFrame::changeEvent(QEvent *event)
{
   if (QEvent::LanguageChange == event->type())
   {
      ui_.retranslateUi(this);
      SPShortcut const shortcut = InputManager::instance().comboTriggerShortcut();
      ui_.editShortcut->setText(shortcut ? shortcut->toString() : "");
   }
   QFrame::changeEvent(event);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::updateGuiState() const
{
   bool const manualTrigger = !prefs_.useAutomaticSubstitution();
   ui_.editShortcut->setEnabled(manualTrigger);
   ui_.buttonChangeShortcut->setEnabled(manualTrigger);
   ui_.buttonResetComboTriggerShortcut->setEnabled(manualTrigger);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onActionResetToDefaultValues()
{
   if (QMessageBox::Yes != QMessageBox::question(this, tr("Reset Preferences"), tr("Are you sure you want to reset "
      "the preferences to their default values?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
      return;
   PreferencesManager& prefs = PreferencesManager::instance();
   QString const oldComboListFolderPath = prefs.comboListFolderPath();
   prefs_.reset();
   InputManager::instance().setComboTriggerShortcut(prefs_.comboTriggerShortcut());
   this->loadPreferences();
   this->applyAutoStartPreference();
   this->applyThemePreference();
   this->applyComboListFolderPreference(prefs.comboListFolderPath(), oldComboListFolderPath);
   InputManager::instance().setComboTriggerShortcut(PreferencesManager::defaultComboTriggerShortcut());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onActionOpenLogFile()
{
   openLogFile();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onActionChangeComboListFolder()
{
   QString const path = QFileDialog::getExistingDirectory(this, tr("Select folder"), 
      QDir::fromNativeSeparators(ui_.editComboListFolder->text()));
   if (path.trimmed().isEmpty())
      return;
   this->applyComboListFolderPreference(path, PreferencesManager::instance().comboListFolderPath());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onActionResetComboListFolder()
{
   PreferencesManager& prefs = PreferencesManager::instance();
   this->applyComboListFolderPreference(PreferencesManager::defaultComboListFolderPath(), prefs. comboListFolderPath());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onActionChangeShortcut() const
{
   ShortcutDialog dlg(InputManager::instance().comboTriggerShortcut());
   if (QDialog::Accepted != dlg.exec())
      return;
   SPShortcut const shortcut = dlg.shortcut();
   ui_.editShortcut->setText(shortcut->toString());
   InputManager::instance().setComboTriggerShortcut(shortcut);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onActionResetComboTriggerShortcut() const
{
   SPShortcut const shortcut = PreferencesManager::defaultComboTriggerShortcut();
   InputManager::instance().setComboTriggerShortcut(shortcut);
   ui_.editShortcut->setText(shortcut ? shortcut->toString() : "");
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onPlaySoundOnComboCheckChanged() const
{
   prefs_.setPlaySoundOnCombo(ui_.checkPlaySoundOnCombo->isChecked());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onAutoStartCheckChanged() const
{
   if (isInPortableMode())
      return;
   prefs_.setAutoStartAtLogin(ui_.checkAutoStart->isChecked());
   this->applyAutoStartPreference();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onAutoCheckForUpdatesCheckChanged() const
{
   prefs_.setAutoCheckForUpdates(ui_.checkAutoCheckForUpdates->isChecked());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onUseCustomThemeCheckChanged() const
{
   prefs_.setUseCustomTheme(ui_.checkUseCustomTheme->isChecked());
   this->applyThemePreference();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onRadioAutomaticComboTriggerChecked(bool checked) const
{
   prefs_.setUseAutomaticSubstitution(ui_.radioComboTriggerAuto->isChecked());
   this->updateGuiState();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onUseClipboardForComboSubstitutionCheckChanged() const
{
   prefs_.setUseClipboardForComboSubstitution(ui_.checkUseClipboardForComboSubstitution->isChecked());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onAutoBackupCheckChanged() const
{
   prefs_.setAutoBackup(ui_.checkAutoBackup->isChecked());
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesFrame::onLocaleChanged() const
{
   QLocale const locale = I18nManager::getSelectedLocaleInCombo(*ui_.comboLocale);
   prefs_.setLocale(locale);
   I18nManager::instance().setLocale(locale);
}


//**********************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
//**********************************************************************************************************************
void PreferencesFrame::onUpdateIsAvailable(SPLatestVersionInfo const& latestVersionInfo)
{
   this->setUpdateCheckStatus(latestVersionInfo ? tr("%1 v%2.%3 is available.").arg(constants::kApplicationName)
      .arg(latestVersionInfo->versionMajor()).arg(latestVersionInfo->versionMinor()) 
      : tr("A new version is available."));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onNoUpdateIsAvailable()
{
   this->setUpdateCheckStatus(tr("The software is up to date."));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onUpdateCheckFailed()
{
   this->setUpdateCheckStatus(tr("Update check failed."));
}

