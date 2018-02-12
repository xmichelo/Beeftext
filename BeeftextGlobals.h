/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of project global variables
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__GLOBALS__H
#define BEEFTEXT__GLOBALS__H


#include <XMiLib/DebugLog/DebugLogWindow.h>


namespace globals {


xmilib::DebugLog& debugLog(); ///< Returns a reference to the debug log
QString appDataDir(); ///< Returns the location of the local storage folder for the application
QString logFilePath(); ///< Returns the path of the log file
bool isInPortableMode(); ///< Test whether the application is running in portable mode


} // namespace globals


#endif // #ifndef BEEFTEXT__GLOBALS__H