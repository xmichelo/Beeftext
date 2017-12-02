/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of preferences manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferencesManager.h"
#include "BeeftextConstants.h"


namespace {
   QString const kKeyGeometry = "Geometry"; ///< The settings key for storing the geometry
   QString const kKeyPlaySoundOnCombo = "PlaySoundOnCombo"; ///< The settings key for the 'Play sound on combo' preference
   
   bool const kDefaultValuePlaySoundOnCombo = true; ///< The default value for the 'Play sound on combo' preference
}


//**********************************************************************************************************************
/// \return The only allowed instance of the class
//**********************************************************************************************************************
PreferencesManager& PreferencesManager::instance()
{
   static PreferencesManager prefManager;
   return prefManager;
}


//**********************************************************************************************************************
/// \note We set the organization and application names manually in case we want to use the preferences before the call 
/// to QApplication::SetOrganizationName() and QApplication::SetApplicationName()
//**********************************************************************************************************************
PreferencesManager::PreferencesManager()
   : settings_(constants::kOrganizationName, constants::kApplicationName)
{

}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesManager::reset()
{
   this->setPlaySoundOnCombo(kDefaultValuePlaySoundOnCombo);
}


//**********************************************************************************************************************
/// \param[in] array The geometry as a byte array
//**********************************************************************************************************************
void PreferencesManager::setMainWindowGeometry(QByteArray const& array)
{
   settings_.setValue(kKeyGeometry, array);
}


//**********************************************************************************************************************
/// \return The geometry of the main window as a byte array
//**********************************************************************************************************************
QByteArray PreferencesManager::mainWindowGeometry() const
{
   return settings_.value(kKeyGeometry).toByteArray();
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setPlaySoundOnCombo(bool value)
{
   settings_.setValue(kKeyPlaySoundOnCombo, value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::getPlaySoundOnCombo() const
{
   return qvariant_cast<bool>(settings_.value(kKeyPlaySoundOnCombo, true));
}


