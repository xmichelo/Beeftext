/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preferences manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_PREFERENCES_MANAGER_H
#define BEEFTEXT_PREFERENCES_MANAGER_H


#include "Shortcut.h"


//**********************************************************************************************************************
/// \brief Preferences manager class
//**********************************************************************************************************************
class PreferencesManager: public QObject
{
   Q_OBJECT
public: // static member functions
   static PreferencesManager& instance(); ///< Return the only allowed instance of the class

public: // member functions
   PreferencesManager(PreferencesManager const&) = delete; ///< Disabled copy constructor
	PreferencesManager(PreferencesManager&&) = delete; ///< Disabled move constructor
	~PreferencesManager() = default; ///< Default destructor
	PreferencesManager& operator=(PreferencesManager const&) = delete; ///< Disabled assignment operator
	PreferencesManager& operator=(PreferencesManager&&) = delete; ///< Disabled move assignment operator
   void reset(); ///< Reset the preferences to their default values
   void resetWarnings() const; ///< Reset the warnings
   QString getInstalledApplicationPath() const; ///< Retrieve the path of the installed application
   void setAlreadyLaunched() const; ///< Set the value for the 'First Launch' preference to false
   bool alreadyLaunched() const; ///< Test whether this is the first time the application is launched
   void setFileMarkedForDeletionOnStartup(QString const& path) const; ///< Get the path of the file marked for deletion on startup
   QString fileMarkedForDeletionOnStartup() const; ///< Set the path of the file marked for deletion on startup
   void clearFileMarkedForDeletionOnStartup() const; ///< Reset the file marked for deletion on startup
   void setMainWindowGeometry(QByteArray const& array) const; ///< Set the geometry of the main window
   QByteArray mainWindowGeometry() const; ///< Get the geometry of the main window
   QLocale locale() const; ///< Get the locale
   void setLocale(QLocale const& locale) const; ///< Set the locale
   void setLastUpdateCheckDateTime(QDateTime const& dateTime) const; ///< Set the last time the application checked for update
   QDateTime lastUpdateCheckDateTime() const; ///< Get the date time of the last update check
   void setAutoStartAtLogin(bool value) const; ///< Set the value for the 'Autostart at login' preference
   bool autoStartAtLogin() const; ///< Get the value for the 'Autostart at login' preference
   void setPlaySoundOnCombo(bool value) const; ///< Set the value for the 'Play sound on combo' preference
   bool playSoundOnCombo() const; ///< Set the value for the 'Play sound on combo' preference
   void setAutoCheckForUpdates(bool value); ///< Set the value for the 'Auto check for updates preference
   bool autoCheckForUpdates() const; ///< Set the value for the 'Auto check for updates preference
   void setUseClipboardForComboSubstitution(bool value) const; ///< Set the value for the 'Use clipboard for combo substitution' preference
   bool useClipboardForComboSubstitution() const; ///< Get the value for the 'Use clipboard for combo substitution' preference
   void setUseCustomTheme(bool value) const; ///< Set the value for the 'Use custom theme' preference
   bool useCustomTheme() const; ///< Get the value for the 'Use custom theme' preference
   void setUseAutomaticSubstitution(bool value); ///< Set the value for the 'Use automatic substitution' preference
   bool useAutomaticSubstitution() const; ///< Get the value for the 'Use automatic substitution' preference
   void setWarnAboutShortComboKeywords(bool value) const; ///< Set the value for the 'Warn about short combo keyword' preference
   bool warnAboutShortComboKeywords() const; ///< Get the value for the 'Warn about short combo keyword' preference
   void setComboListFolderPath(QString const& value) const; ///< Set the path of the folder for saving the combo list
   QString comboListFolderPath() const; ///< Get the path of the folder for saving the combo list
   static QString defaultComboListFolderPath(); ///< Get the default combo list folder path
   void setComboTriggerShortcut(SpShortcut const& shortcut); ///< Set the combo trigger shortcut
   SpShortcut comboTriggerShortcut() const; ///< Retrieve the combo trigger shortcut
   void setAutoBackup(bool value) const; ///< Set the value for the 'Auto backup' preference
   bool autoBackup() const; ///< Get the value for the 'Auto backup' preference
   QString lastComboImportExportPath() const; ///< Retrieve the path of the last imported and exported path
   void setLastComboImportExportPath(QString const& path) const; ///< Retrieve the path of the last imported and exported path
   static SpShortcut defaultComboTriggerShortcut(); ///< Reset the combo trigger shortcut to its default value

signals:
   void autoCheckForUpdatesChanged(bool value); // Signal emitted when the 'Auto check for updates' preference value changed

private: // member functions
   PreferencesManager(); ///< Default constructor
   void cacheComboTriggerShortcut(); ///< Read the combo trigger shortcut and cache it for faster access
   void applyCustomThemePreference() const; ///< Apply the preference for the custom theme
   void applyAutoStartPreference() const; ///< Apply the preference for the auto-start
   void applyLocalePreference() const; ///< Apply the preference for the locale
   bool registerApplicationForAutoStart() const; ///< Register the application to be automatically started at login
   static void unregisterApplicationFromAutoStart(); ///< Unregister the application to be automatically started at login

   template <typename T> T readSettings(QString const& key, T const& defaultValue = T()) const; ///< Read a value of a given type read from the settings
   
private: // data members
   std::unique_ptr<QSettings> settings_; ///< The Qt settings instance
   bool cachedUseAutomaticSubstitution_; ///< Cached value for the 'use automatic substitution' preference value
   SpShortcut cachedComboTriggerShortcut_; ///< Cached value for the 'combo trigger shortcut' preference
};


#endif // #ifndef BEEFTEXT_PREFERENCES_MANAGER_H