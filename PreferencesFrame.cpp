/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preferences frame
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferencesFrame.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"
#include "MainWindow.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the frame
//**********************************************************************************************************************
PreferencesFrame::PreferencesFrame(QWidget* parent)
   : QFrame(parent)
   , prefs_(PreferencesManager::instance())
{
   ui_.setupUi(this);
   ui_.checkAutoStart->setText(tr("&Automatically start %1 at login").arg(constants::kApplicationName));
   loadPreferences();

   MainWindow* mainWindow = static_cast<MainWindow*>(this->window());
   connect(ui_.buttonCheckNow, &QPushButton::clicked, mainWindow, &MainWindow::launchCheckForUpdate);
   connect(mainWindow, &MainWindow::startedCheckingForUpdate, [&]() { ui_.buttonCheckNow->setEnabled(false); });
   connect(mainWindow, &MainWindow::finishedCheckingForUpdate, [&]() { ui_.buttonCheckNow->setEnabled(true); });
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::loadPreferences()
{
   ui_.checkPlaySoundOnCombo->setChecked(prefs_.playSoundOnCombo());
   ui_.checkAutoCheckForUpdates->setChecked(prefs_.autoCheckForUpdates());

   ui_.checkAutoStart->blockSignals(true);
   ui_.checkAutoStart->setChecked(prefs_.autoStartAtLogin());
   ui_.checkAutoStart->blockSignals(false);
   this->applyAutoStartPreference(); // we ensure autostart is properly setup
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
void PreferencesFrame::onActionResetToDefaultValues()
{
   if (QMessageBox::Yes != QMessageBox::question(this, tr("Reset Preferences"), tr("Are you sure you want to reset "
      "the preferences to their default values."), QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
      return;
   prefs_.reset();
   this->loadPreferences();
   this->applyAutoStartPreference();
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
