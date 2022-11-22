/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of project global variables
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_GLOBALS_H
#define BEEFTEXT_GLOBALS_H


#include "ProcessListManager.h"
#include <XMiLib/DebugLog/DebugLogWindow.h>


namespace globals {


xmilib::DebugLog &debugLog(); ///< Returns a reference to the debug log
xmilib::DebugLogWindow &debugLogWindow(); ///< Returns a reference to the debug log window.
ProcessListManager &sensitiveApplications(); ///< Return the list of sensitive applications.
ProcessListManager &excludedApplications(); ///< Return the list of excluded applications.
QString getBuildInfo(); ///< Returns a string containing information about the current build.
QString appDataDir(); ///< Returns the location of the local storage folder for the application
QString translationRootFolderPath(); ///< Returns the location of the root folder for application provided translations.
QString userTranslationRootFolderPath(); ///< Returns the location of the root folder of the user provided translations.
QString logFilePath(); ///< Returns the path of the log file
QString backupFolderPath(); ///< Return the path of the backup folder.
QString defaultBackupFolderPath(); ///< Return the default path of the backup folder.
QString portableModeDataFolderPath(); ///< Returns the path of the user data folder when the application is run in portable mode
QString portableModeSettingsFilePath(); ///< Returns the path of the settings file when the application is run in portable mode
QString emojiExcludedAppsFilePath(); ///< Return the path of the JSON file containing the list of emoji exceptions
QColor disabledTextColorInTablesAndLists(); ///< Return the color for disabled text.
QString jsonFileDialogFilter(); ///< The Open/Save file dialog filter
QString jsonCsvFileDialogFilter(); ///< The file format filter for the import file picker dialog.
QString csvFileDialogFilter(); ///< The file format filter for CSV files.
QString backupFileDialogFilter(); ///< The file format filter for the backup and restore file picker dialogs.


} // namespace globals


#endif // #ifndef BEEFTEXT_GLOBALS_H