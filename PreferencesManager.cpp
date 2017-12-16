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
   QString const kKeyAlreadyLaunched = "AlreadyLaunched"; ///< The settings key for the "Already launched" indicator
   QString const kKeyFileMarkedForDeletion = "markedForDeletion"; ///< The path of the file marked for deletion on next application startup
   QString const kKeyGeometry = "Geometry"; ///< The settings key for storing the geometry
   QString const kKeyAppExePath = "AppExePath"; ///< The settings key for the application executable path
   QString const kKeyPlaySoundOnCombo = "PlaySoundOnCombo"; ///< The settings key for the 'Play sound on combo' preference
   QString const kKeyAutoStartAtLogin = "AutoStartAtLogin"; ///< The settings key for the 'Autostart at login' preference
   QString const kKeyAutoCheckForUpdates = "AutoCheckForUpdate"; ///< The settings key for the 'Autostart at login' preference
   QString const kKeyUseClipboardForComboSubstitution = "UseClipboardForComboSubstitution"; ///< The setting key for the 'Use clipboard for combo substitution' preference
   bool const kDefaultValuePlaySoundOnCombo = true; ///< The default value for the 'Play sound on combo' preference
   bool const kDefaultValueAutoStartAtLogin = false; ///< The default value for the 'Autostart at login' preference
   bool const kDefaultValueAutoCheckForUpdates = true; ///< The default value for the 'Auto check for update preference
   bool const kDefaultvalueUseClipboardForComboSubstitution = true; ///< The default value for the 'Use clipboard for combo substitution' preference
}


//**********************************************************************************************************************
/// This function returns the default value if the key does not exist in the settings OR if the value is not of the
/// expected data type.
///
/// \param[in] key The key to read from
/// \param[in] default value The default value to use if the key does not exist or its value is not of the right type
/// \return The read value
//**********************************************************************************************************************
template <typename T> T PreferencesManager::readSettings(QString const& key, T const& defaultValue) const
{
   if (!settings_.contains(key))
      return defaultValue;
   QVariant v = settings_.value(key, defaultValue);
   return v.canConvert<T>() ? qvariant_cast<T>(v) : defaultValue;
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
   this->setUseClipboardForComboSubstitution(kDefaultvalueUseClipboardForComboSubstitution);
}


//**********************************************************************************************************************
/// The value for this preference is set by the NSIS installer. 
//**********************************************************************************************************************
QString PreferencesManager::getInstalledApplicationPath() const
{
   if (!settings_.contains(kKeyAppExePath))
      return QString();
   return QDir::fromNativeSeparators(this->readSettings<QString>(kKeyAppExePath));
}


//**********************************************************************************************************************
/// Set the settings value indicating that the application has been launched in the past
//**********************************************************************************************************************
void PreferencesManager::setAlreadyLaunched()
{
   settings_.setValue(kKeyAlreadyLaunched, 1);
}


//**********************************************************************************************************************
/// Check whether the application has ever been launched or not
//**********************************************************************************************************************
bool PreferencesManager::alreadyLaunched() const
{
   return this->readSettings<bool>(kKeyAlreadyLaunched, false);
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
QString PreferencesManager::fileMarkedForDeletionOnStartup() const
{
   return readSettings<QString>(kKeyFileMarkedForDeletion);
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
bool PreferencesManager::autoStartAtLogin() const
{
   return this->readSettings<bool>(kKeyAutoStartAtLogin, kDefaultValueAutoStartAtLogin);
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
bool PreferencesManager::playSoundOnCombo() const
{
   return this->readSettings<bool>(kKeyPlaySoundOnCombo, kDefaultValuePlaySoundOnCombo);
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
bool PreferencesManager::autoCheckForUpdates() const
{
   return this->readSettings<bool>(kKeyAutoCheckForUpdates, kDefaultValueAutoCheckForUpdates);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setUseClipboardForComboSubstitution(bool value)
{
   settings_.setValue(kKeyUseClipboardForComboSubstitution, value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::useClipboardForComboSubstitution() const
{
   return this->readSettings<bool>(kKeyUseClipboardForComboSubstitution, kDefaultvalueUseClipboardForComboSubstitution);
}



