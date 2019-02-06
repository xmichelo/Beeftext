/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  

#ifndef BEEFTEXT_UTILS_H
#define BEEFTEXT_UTILS_H


void openLogFile(); ///< Open the log file with the default text editor
bool isInPortableMode(); ///< Test whether the application is running in portable mode
bool usePortableAppsFolderLayout(); ///< Test if the application is using the PortableApps.com layout
QString getActiveExecutableFileName(); ///< Return the name of the active application's executable file
void performTextSubstitution(qint32 charCount, QString const& newText, qint32 cursorLeftShift); ///< Substitute the last characters with the specified text
void reportInternalError(QString const& error, QWidget* parent = nullptr); ///< Report an internal error using a message box and write details in the log


#endif // #ifndef BEEFTEXT_UTILS_H
