/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of project global variables
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_GLOBALS_H
#define BEEFTEXT_GLOBALS_H


#include <XMiLib/DebugLog/DebugLogWindow.h>


namespace globals {


xmilib::DebugLog& debugLog(); ///< Returns a reference to the debug log
QString appDataDir(); ///< Returns the location of the local storage folder for the application
QString translationRootFolderPath(); ///< Returns the location of the root translation folder
QString logFilePath(); ///< Returns the path of the log file
QString backupFolderPath(); ///< Return the backup folder path
QString portableModeDataFolderPath(); ///< Returns the path of the user data folder when the application is run in portable mode
QString portableModeSettingsFilePath(); ///< Returns the path of the settings file when the application is run in portable mode


} // namespace globals


#endif // #ifndef BEEFTEXT_GLOBALS_H