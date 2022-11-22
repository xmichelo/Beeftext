/// \file
/// \author 
///
/// \brief Declaration of auto start related function.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_AUTO_START_H
#define BEEFTEXT_AUTO_START_H


void applyAutostartParameters(); ///< Verify that the parameters for the auto start of the application are properly applied.
QString installedApplicationPath(); ///< Retrieve the path of the installed application
bool registeredApplicationForAutostart(QString &outPath); ///< check if an application is registered as Beeftext for auto-start, and if so, returns its path.
void registerApplicationForAutoStart(QString const &appPath); ///< Register the application to be automatically started at login
void unregisterApplicationFromAutoStart();


#endif // #ifndef BEEFTEXT_AUTO_START_H
