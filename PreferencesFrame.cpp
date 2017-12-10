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
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::loadPreferences()
{
   ui_.checkPlaySoundOnCombo->setChecked(prefs_.getPlaySoundOnCombo());
   ui_.checkAutoCheckForUpdates->setChecked(prefs_.getAutoCheckForUpdates());

   ui_.checkAutoStart->blockSignals(true);
   ui_.checkAutoStart->setChecked(prefs_.getAutoStartAtLogin());
   ui_.checkAutoStart->blockSignals(false);
   this->applyAutoStartPreference(); // we ensure autostart is properly setup
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::applyAutoStartPreference()
{
   if (prefs_.getAutoStartAtLogin())
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
   qDebug() << QString("%1()").arg(__FUNCTION__);
   prefs_.setAutoCheckForUpdates(ui_.checkAutoCheckForUpdates->isChecked());
}


