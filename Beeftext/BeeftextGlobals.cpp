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
/// \return The path of the folder containing all the translation files
//**********************************************************************************************************************
QString translationRootFolderPath()
{
   return QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("Translations");
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
   return QDir(appDataDir()).absoluteFilePath("sensitiveApplications.json");
}


//**********************************************************************************************************************
/// \return The path of the JSON file containing the list of application excluded from emoji substitution.
//**********************************************************************************************************************
QString emojiExceptionsFilePath()
{
   return QDir(appDataDir()).absoluteFilePath("emojiExceptions.json");
}


} // namespace globals
