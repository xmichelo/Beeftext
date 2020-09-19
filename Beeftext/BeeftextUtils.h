/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  

#ifndef BEEFTEXT_UTILS_H
#define BEEFTEXT_UTILS_H


//**********************************************************************************************************************
/// \brief Enumeration for the source that triggered a combo.
//**********************************************************************************************************************
enum class ETriggerSource
{
   Keyword, ///< The combo was triggered by a keyword
   ComboPicker, ///< The combo was trigger by the combo picker
}; ///< 


void openLogFile(); ///< Open the log file with the default text editor
bool isInPortableMode(); ///< Test whether the application is running in portable mode
bool usePortableAppsFolderLayout(); ///< Test if the application is using the PortableApps.com layout
QString getActiveExecutableFileName(); ///< Return the name of the active application's executable file
QString snippetToPlainText(QString const& snippet, bool isHtml); ///< Return the plain text for a snippet.
void performTextSubstitution(qint32 charCount, QString const& newText, bool isHtml, qint32 cursorPos, 
   ETriggerSource source); ///< Substitute the last characters with the specified text
void reportError(QWidget* parent, QString const& logMessage, QString const& userMessage = QString()); ///< Report an error to the user
bool isAppRunningOnWindows10OrHigher(); ///< Return true if and only if the application is running on windows 10 or higher
qint32 printableCharacterCount(QString const& str); ///< Return the (estimated) number of printable characters in a string
QMimeData* mimeDataFromSnippet(QString const& snippet, bool isHtml); ///< Retrieve the MIME data corresponding to a combo's snippet.


#endif // #ifndef BEEFTEXT_UTILS_H
