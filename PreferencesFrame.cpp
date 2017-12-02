/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preferences frame
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferencesFrame.h"


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the frame
//**********************************************************************************************************************
PreferencesFrame::PreferencesFrame(QWidget* parent)
   : QFrame(parent)
   , prefs_(PreferencesManager::instance())
{
   ui_.setupUi(this);
   loadPreferences();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::loadPreferences()
{
   qDebug() << QString("%1()").arg(__FUNCTION__);
   ui_.checkPlaySoundOnCombo->setChecked(prefs_.getPlaySoundOnCombo());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onActionResetToDefaultValues()
{
   qDebug() << QString("%1()").arg(__FUNCTION__);
   prefs_.reset();
   this->loadPreferences();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesFrame::onPlaySoundOnComboCheckChanged()
{
   qDebug() << QString("%1()").arg(__FUNCTION__);
   prefs_.setPlaySoundOnCombo(ui_.checkPlaySoundOnCombo->isChecked());
}

