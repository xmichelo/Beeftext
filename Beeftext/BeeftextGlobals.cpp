/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of project global variables
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"
#include "BeeftextConstants.h"
#include "Preferences/PreferencesManager.h"


namespace globals {


//**********************************************************************************************************************
/// \return The application debug log
//**********************************************************************************************************************
xmilib::DebugLog& debugLog()
{
   static xmilib::DebugLog log;
   return log;
}


//**********************************************************************************************************************
/// \return The location of the local storage folder for the application
//**********************************************************************************************************************
QString appDataDir()
{
   return isInPortableMode() ? portableModeDataFolderPath() :
      QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}


//**********************************************************************************************************************
/// \return The path of the folder containing the application provided translations.
//**********************************************************************************************************************
QString translationRootFolderPath()
{
   return QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("Translations");
}


//**********************************************************************************************************************
/// \return The path of the folder containing the user-provided translations.
//**********************************************************************************************************************
QString userTranslationRootFolderPath()
{
   return QDir(appDataDir()).absoluteFilePath("Translations");
}


//**********************************************************************************************************************
/// \return The absolute path of the log file
//**********************************************************************************************************************
QString logFilePath()
{
   return QDir(appDataDir()).absoluteFilePath("log.txt");
}


//**********************************************************************************************************************
/// \return The path of the backup folder
//**********************************************************************************************************************
QString backupFolderPath()
{
   PreferencesManager const& prefs = PreferencesManager::instance();
   QString defaultPath = defaultBackupFolderPath();
   if (!prefs.useCustomBackupLocation())
      return defaultPath;
   QString const customPath = PreferencesManager::instance().customBackupLocation();
   return customPath.isEmpty() ? defaultPath : customPath;
}


//**********************************************************************************************************************
/// \return The default path of the backup folder
//**********************************************************************************************************************
QString defaultBackupFolderPath()
{
   return QDir(appDataDir()).absoluteFilePath("Backup");
}


//**********************************************************************************************************************
/// \return The absolute path of the folder where the user data is stored when the that is used when the application is 
/// in portable mode
//**********************************************************************************************************************
QString portableModeDataFolderPath()
{
   QDir const appDir(QCoreApplication::applicationDirPath());
   return usePortableAppsFolderLayout() ? appDir.absoluteFilePath("../../Data/settings") 
      : appDir.absoluteFilePath("Data");
}

//**********************************************************************************************************************
/// \return The path of the settings file that is used when the application is in portable mode
//**********************************************************************************************************************
QString portableModeSettingsFilePath()
{
   return QDir(portableModeDataFolderPath()).absoluteFilePath("Settings.ini");
}


//**********************************************************************************************************************
/// \return The path of the JSON file containing the list of sensitive applications
//**********************************************************************************************************************
QString sensitiveApplicationsFilePath()
{
   return QDir(appDataDir()).absoluteFilePath("sensitiveApps.json");
}


//**********************************************************************************************************************
/// \return The path of the JSON file containing the list of application excluded from emoji substitution.
//**********************************************************************************************************************
QString emojiExcludedAppsFilePath()
{
   return QDir(appDataDir()).absoluteFilePath("emojiExcludedApps.json");
}

//**********************************************************************************************************************
/// \return the color of disabled items in tables and list views.
//**********************************************************************************************************************
QColor disabledTextColorInTablesAndLists()
{
   PreferencesManager const& prefs = PreferencesManager::instance();
   return (prefs.useCustomTheme() && (ETheme::Dark ==prefs.theme())) ? QColor(0x55, 0x55, 0x55)
      : QColor(0xa0, 0xa0, 0xa0);
}


//**********************************************************************************************************************
/// \return The filter for the backup files dialog.
//**********************************************************************************************************************
QString backupFileDialogFilter()
{
   return QObject::tr("Beeftext backup files (*.%1);;All files (*.*)").arg(constants::backupFileExtension);
}


//**********************************************************************************************************************
/// \return The filter for the JSON files dialog.
//**********************************************************************************************************************
QString jsonFileDialogFilter()
{
   return QObject::tr("JSON files (*.json);;All files (*.*)");
}


//**********************************************************************************************************************
/// \return The filter for the JSON & CSV files dialog.
//**********************************************************************************************************************
QString jsonCsvFileDialogFilter()
{
   return QObject::tr("JSON & CSV files (*.json *.csv);;JSON files (*.json);;"
      "CSV files (*.csv);;All files (*.*)");
}


//**********************************************************************************************************************
/// \return The filter for the CSV files dialog.
//**********************************************************************************************************************
QString csvFileDialogFilter()
{
   return QObject::tr("CSV files (*.csv);;All files (*.*)");
}


} // namespace globals
