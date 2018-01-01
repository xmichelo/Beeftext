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
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"
#include "MainWindow.h"
#include <XMiLib/Exception.h>


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
   loadPreferences();
   this->applyAutoStartPreference(); // we ensure autostart is properly setup
   this->applyThemePreference(); // we apply the custom theme if the user selected it

   // signal mappings for the 'Check now' button
   UpdateManager& updateManager = UpdateManager::instance();
   connect(ui_.buttonCheckNow, &QPushButton::clicked, &updateManager, &UpdateManager::checkForUpdate);
   connect(&updateManager, &UpdateManager::startedUpdateCheck, [&]() { ui_.buttonCheckNow->setEnabled(false); });
   connect(&updateManager, &UpdateManager::finishedUpdateCheck, [&]() { ui_.buttonCheckNow->setEnabled(true); });
   connect(&updateManager, &UpdateManager::updateIsAvailable, this, &PreferencesFrame::onUpdateIsAvailable);
   connect(&updateManager, &UpdateManager::noUpdateIsAvailable, this, &PreferencesFrame::onNoUpdateIsAvailable);
   connect(&updateManager, &UpdateManager::updateCheckFailed, this, &PreferencesFrame::onUpdateCheckFailed);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::loadPreferences()
{
   QSignalBlocker blockers[] =  { QSignalBlocker(ui_.checkPlaySoundOnCombo),
      QSignalBlocker(ui_.checkAutoCheckForUpdates), QSignalBlocker(ui_.checkUseClipboardForComboSubstitution),
      QSignalBlocker(ui_.checkAutoStart) }; // Temporarily Block signals emission by the controls
   ui_.checkPlaySoundOnCombo->setChecked(prefs_.playSoundOnCombo());
   ui_.checkAutoCheckForUpdates->setChecked(prefs_.autoCheckForUpdates());
   ui_.checkUseClipboardForComboSubstitution->setChecked(prefs_.useClipboardForComboSubstitution());
   ui_.checkAutoStart->setChecked(prefs_.autoStartAtLogin());
   ui_.checkUseCustomTheme->setChecked(prefs_.useCustomTheme());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::applyAutoStartPreference()
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
void PreferencesFrame::applyThemePreference()
{
   qApp->setStyleSheet(prefs_.useCustomTheme() ? constants::kStyleSheet : QString());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::setUpdateCheckStatus(QString const& status)
{
   updateCheckStatusTimer_.stop();
   ui_.labelUpdateCheckStatus->setText(status);
   updateCheckStatusTimer_.start(kUpdateCheckStatusLabelTimeoutMs);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onActionResetToDefaultValues()
{
   if (QMessageBox::Yes != QMessageBox::question(this, tr("Reset Preferences"), tr("Are you sure you want to reset "
      "the preferences to their default values."), QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
      return;
   prefs_.reset();
   this->loadPreferences();
   this->applyAutoStartPreference();
   this->applyThemePreference();
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
void PreferencesFrame::onPlaySoundOnComboCheckChanged()
{
   prefs_.setPlaySoundOnCombo(ui_.checkPlaySoundOnCombo->isChecked());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onAutoStartCheckChanged()
{
   prefs_.setAutoStartAtLogin(ui_.checkAutoStart->isChecked());
   this->applyAutoStartPreference();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onAutoCheckForUpdatesCheckChanged()
{
   prefs_.setAutoCheckForUpdates(ui_.checkAutoCheckForUpdates->isChecked());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onUseCustomThemeCheckChanged()
{
   prefs_.setUseCustomTheme(ui_.checkUseCustomTheme->isChecked());
   this->applyThemePreference();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onUseClipboardForComboSubstitutionCheckChanged()
{
   prefs_.setUseClipboardForComboSubstitution(ui_.checkUseClipboardForComboSubstitution->isChecked());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onUpdateIsAvailable(SPLatestVersionInfo latestVersionInfo)
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

