/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  

#ifndef BEEFTEXT__UTILS__H
#define BEEFTEXT__UTILS__H


bool registerApplicationForAutoStart(); ///< Register the application to be automatically started at login
void unregisterApplicationFromAutoStart(); ///< Unregister the application to be automatically started at login
void openLogFile(); ///< Open the log file with the default text editor
bool isInPortableMode(); ///< Test whether the application is running in portable mode
bool usePortableAppsFolderLayout(); ///< Test if the application is using the PortableApps.com layout
QString getActiveExecutableFileName(); ///< Return the name of the active application's executable file
bool doesApplicationSupportCtrlVShortcut(QString const& appExeName); ///< Check if the application supports the Ctrl+V shortcut for pasting


#endif // #ifndef BEEFTEXT__UTILS__H
