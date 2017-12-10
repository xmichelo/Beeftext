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
   QString const kKeyFileMarkedForDeletion = "markedForDeletion"; ///< The path of the file marked for deletion on next application startup
   QString const kKeyGeometry = "Geometry"; ///< The settings key for storing the geometry
   QString const kKeyAppExePath = "AppExePath"; ///< The settings key for the application executable path
   QString const kKeyPlaySoundOnCombo = "PlaySoundOnCombo"; ///< The settings key for the 'Play sound on combo' preference
   QString const kKeyAutoStartAtLogin = "AutoStartAtLogin"; ///< The settings key for the 'Autostart at login' preference
   QString const kKeyAutoCheckForUpdates = "AutoCheckForUpdate"; ///< The settings key for the 'Autostart at login' preference
  
   bool const kDefaultValuePlaySoundOnCombo = true; ///< The default value for the 'Play sound on combo' preference
   bool const kDefaultValueAutoStartAtLogin = false; ///< The default value for the 'Autostart at login' preference
   bool const kDefaultValueAutoCheckForUpdates = true; ///< The default value for the 'Auto check for update preference
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
   this->setAutoCheckForUpdates(kDefaultValueAutoCheckForUpdates);
   this->setAutoStartAtLogin(kDefaultValueAutoStartAtLogin); // we do not actually touch the registry here
}


//**********************************************************************************************************************
/// The value for this preference is set by the NSIS installer. 
//**********************************************************************************************************************
QString PreferencesManager::getInstalledApplicationPath() const
{
   if (!settings_.contains(kKeyAppExePath))
      return QString();
   return QDir::fromNativeSeparators(settings_.value(kKeyAppExePath).toString());
}


//**********************************************************************************************************************
/// \param[in] path The path of the file to delete on next application startup
//**********************************************************************************************************************
void PreferencesManager::setFileMarkedForDeletionOnStartup(QString const& path)
{
   settings_.setValue(kKeyFileMarkedForDeletion, path);
}


//**********************************************************************************************************************
/// \return The path of the file marked for deletion on next application startup
//**********************************************************************************************************************
QString PreferencesManager::getFileMarkedForDeletionOnStartup() const
{
   return settings_.value(kKeyFileMarkedForDeletion).toString();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesManager::clearFileMarkedForDeletionOnStartup()
{
   settings_.remove(kKeyFileMarkedForDeletion);
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
void PreferencesManager::setAutoStartAtLogin(bool value)
{
   settings_.setValue(kKeyAutoStartAtLogin, value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::getAutoStartAtLogin() const
{
   return qvariant_cast<bool>(settings_.value(kKeyAutoStartAtLogin, kDefaultValueAutoStartAtLogin));
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
   return qvariant_cast<bool>(settings_.value(kKeyPlaySoundOnCombo, kDefaultValuePlaySoundOnCombo));
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setAutoCheckForUpdates(bool value)
{
   settings_.setValue(kKeyAutoCheckForUpdates, value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::getAutoCheckForUpdates() const
{
   return qvariant_cast<bool>(settings_.value(kKeyAutoCheckForUpdates, kDefaultValueAutoCheckForUpdates));
}


