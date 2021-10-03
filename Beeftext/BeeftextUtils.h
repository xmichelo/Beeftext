/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  

#ifndef BEEFTEXT_UTILS_H
#define BEEFTEXT_UTILS_H


#include "Shortcut.h"


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
bool isBeeftextTheForegroundApplication(); ///< Check whether Beeftext is the foreground application
QString getActiveExecutableFileName(); ///< Return the name of the active application's executable file
QString htmlToPlainText(QString const& snippet); ///< Return the plain text for a snippet.
void eraseChars(qint32 count); ///< Erase characters by generating backspace characters.
void insertText(QString const& text); ///< Insert the text given text.
void renderShortcut(SpShortcut const& shortcut); ///< Synthesize the given shortcut.
void moveCursorLeft(qint32 count); ///< Move the cursor the the left by the specified number of characters.
void performTextSubstitution(qint32 charCount, QString const& newText, qint32 cursorPos, ETriggerSource source); ///< Substitute the last characters with the specified text
void reportError(QWidget* parent, QString const& logMessage, QString const& userMessage = QString()); ///< Report an error to the user
bool isAppRunningOnWindows10OrHigher(); ///< Return true if and only if the application is running on windows 10 or higher
qint32 printableCharacterCount(QString const& str); ///< Return the (estimated) number of printable characters in a string
QMimeData* mimeDataFromText(QString const& text); ///< Create a MIME data instance for text content.
QMimeData* mimeDataFromHtml(QString const& html);  ///< Create a MIME data instance for HTML content.
bool warnAndConvertHtmlCombos(); ///< Warn the user about discontinued rich text combo support and convert them to plain text.
QString colorToHex(QColor const& color, bool includeAlpha); ///< Get a hex representation of a color.
bool runShortcutDialog(SpShortcut& shortcut); ///< Runs a dialog allowing the user to select a shortcut.


#endif // #ifndef BEEFTEXT_UTILS_H
