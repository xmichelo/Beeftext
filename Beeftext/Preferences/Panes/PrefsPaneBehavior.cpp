/// \file
/// \author 
///
/// \brief Implementation of behavior preference pane.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PrefsPaneBehavior.h"
#include "Combo/ComboManager.h"
#include "BeeftextConstants.h"
#include "BeeftextUtils.h"


namespace {


qint32 kUpdateCheckStatusLabelTimeoutMs = 3000; ///< The delay after which the update check status label is cleared


}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the pane.
//**********************************************************************************************************************
PrefsPaneBehavior::PrefsPaneBehavior(QWidget* parent)
   : PrefsPane(parent)
   , prefs_(PreferencesManager::instance())
{
   ui_.setupUi(this);
   this->updateCheckStatusTimer_.setSingleShot(true);
   connect(&updateCheckStatusTimer_, &QTimer::timeout, [&]() { ui_.labelUpdateCheckStatus->setText(QString()); });
   ui_.labelUpdateCheckStatus->setText(QString());
   if (isInPortableMode())
      ui_.checkAutoStart->setVisible(false);

   // signal mappings for the 'Check now' button
   UpdateManager& updateManager = UpdateManager::instance();
   connect(ui_.buttonCheckNow, &QPushButton::clicked, &updateManager, &UpdateManager::checkForUpdate);
   connect(&updateManager, &UpdateManager::startedUpdateCheck, this, &PrefsPaneBehavior::onUpdateCheckStarted);
   connect(&updateManager, &UpdateManager::finishedUpdateCheck, this, &PrefsPaneBehavior::onUpdateCheckFinished);
   connect(&updateManager, &UpdateManager::updateIsAvailable, this, &PrefsPaneBehavior::onUpdateIsAvailable);
   connect(&updateManager, &UpdateManager::noUpdateIsAvailable, this, &PrefsPaneBehavior::onNoUpdateIsAvailable);
   connect(&updateManager, &UpdateManager::updateCheckFailed, this, &PrefsPaneBehavior::onUpdateCheckFailed);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneBehavior::load() const
{
   QSignalBlocker blocker(ui_.checkAutoCheckForUpdates);
   ui_.checkAutoCheckForUpdates->setChecked(prefs_.autoCheckForUpdates());
   blocker = QSignalBlocker(ui_.checkAutoStart);
   ui_.checkAutoStart->setChecked(prefs_.autoStartAtLogin());
   blocker = QSignalBlocker(ui_.checkPlaySoundOnCombo);
   ui_.checkPlaySoundOnCombo->setChecked(prefs_.playSoundOnCombo());
   blocker = QSignalBlocker(ui_.checkUseCustomSound);
   ui_.checkUseCustomSound->setChecked(prefs_.useCustomSound());
   ui_.editCustomSound->setText(QDir::toNativeSeparators(prefs_.customSoundPath()));
   SpShortcut const shortcut = prefs_.appEnableDisableShortcut();
   blocker = QSignalBlocker(ui_.checkAppEnableDisable);
   ui_.checkAppEnableDisable->setChecked(prefs_.enableAppEnableDisableShortcut());
   ui_.editAppEnableDisableShortcut->setText(shortcut ? shortcut->toString() : "");
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneBehavior::updateGui() const
{
      ui_.frameCustomSound->setEnabled(ui_.checkPlaySoundOnCombo->isChecked());
      bool const useCustomSound = ui_.checkUseCustomSound->isChecked();
      QWidgetList widgets = { ui_.editCustomSound, ui_.buttonChangeCustomSound, ui_.buttonPlay };
      for (QWidget* const widget: widgets)
         widget->setEnabled(useCustomSound);
      ui_.frameAppEnableDisableShortcut->setEnabled(ui_.checkAppEnableDisable->isChecked());
}


//**********************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
//**********************************************************************************************************************
void PrefsPaneBehavior::onUpdateIsAvailable(SpLatestVersionInfo const& latestVersionInfo)
{
   this->setUpdateCheckStatus(latestVersionInfo ? tr("%1 v%2.%3 is available.").arg(constants::kApplicationName)
      .arg(latestVersionInfo->versionMajor()).arg(latestVersionInfo->versionMinor())
      : tr("A new version is available."));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneBehavior::onNoUpdateIsAvailable()
{
   this->setUpdateCheckStatus(tr("The software is up to date."));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneBehavior::onUpdateCheckStarted() const
{
   ui_.buttonCheckNow->setEnabled(false);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneBehavior::onUpdateCheckFinished() const
{
   ui_.buttonCheckNow->setEnabled(true);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneBehavior::onUpdateCheckFailed()
{
   this->setUpdateCheckStatus(tr("Update check failed."));
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked
//**********************************************************************************************************************
void PrefsPaneBehavior::onCheckAutoCheckForUpdates(bool checked) const
{
   prefs_.setAutoCheckForUpdates(checked);
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked
//**********************************************************************************************************************
void PrefsPaneBehavior::onCheckAutoStart(bool checked) const
{
   if (!isInPortableMode())
      prefs_.setAutoStartAtLogin(checked);
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked
//**********************************************************************************************************************
void PrefsPaneBehavior::onCheckPlaySoundOnCombo(bool checked) const
{
   prefs_.setPlaySoundOnCombo(checked);
   this->updateGui();
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked?
//**********************************************************************************************************************
void PrefsPaneBehavior::onCheckUseCustomSound(bool checked) const
{
   prefs_.setUseCustomSound(checked);
   ComboManager::instance().loadSoundFromPreferences();
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneBehavior::onChangeCustomSound() const
{
   QString const oldPath = prefs_.customSoundPath();
   QString const path = QFileDialog::getOpenFileName(nullptr, QObject::tr("Select custom sound file"),
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
void PrefsPaneBehavior::onPlaySoundButton() const
{
   if (QFileInfo(prefs_.customSoundPath()).exists())
      ComboManager::instance().playSound();
}



//**********************************************************************************************************************
/// \param[in] checked Is the check box checked?
//**********************************************************************************************************************
void PrefsPaneBehavior::onCheckEnableAppEnableDisableShortcut(bool checked) const
{
   prefs_.setEnableAppEnableDisableShortcut(checked);
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneBehavior::onChangeAppEnableDisableShortcut() const
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
void PrefsPaneBehavior::onResetAppEnableDisableShortcut() const
{
   SpShortcut const shortcut = PreferencesManager::defaultAppEnableDisableShortcut();
   prefs_.setAppEnableDisableShortcut(shortcut);
   ui_.editAppEnableDisableShortcut->setText(shortcut ? shortcut->toString() : "");
}


//**********************************************************************************************************************
/// \param[in] status The status message
//**********************************************************************************************************************
void PrefsPaneBehavior::setUpdateCheckStatus(QString const& status)
{
   updateCheckStatusTimer_.stop();
   ui_.labelUpdateCheckStatus->setText(status);
   updateCheckStatusTimer_.start(kUpdateCheckStatusLabelTimeoutMs);
}


//**********************************************************************************************************************
/// \return true if and only if the input is validated.
//**********************************************************************************************************************
bool PrefsPaneBehavior::validateInput()
{
   if (ui_.checkPlaySoundOnCombo->isChecked() && ui_.checkUseCustomSound->isChecked())
   {
      QFileInfo const fi(ui_.editCustomSound->text());
      if ((!fi.exists()) || (!fi.isFile()) || (!fi.isReadable()))
      {
         QMessageBox::critical(this, tr("Error"), tr("The path of the custom sound file is invalid."));
         return false;
      }
   }
   return true;
}


