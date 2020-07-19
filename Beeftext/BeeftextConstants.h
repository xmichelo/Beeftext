/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of application constants
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_CONSTANTS_H
#define BEEFTEXT_CONSTANTS_H


namespace constants {


extern QString const kApplicationName; ///< The application name
extern QString const kOrganizationName; ///< The organization name
extern qint32 const kVersionMajor; ///< The major version number of the application
extern qint32 const kVersionMinor; ///< The minor version number of the application
extern QString const kBeeftextWikiHomeUrl; ///< The URL of the Beeftext wiki
extern QString const kGettingStartedUrl; ///< The URL of the Getting started page
extern QString const kBeeftextWikiVariablesUrl; ///< The URL of the page about variables in the Beeftext wiki
extern QString const kBeeftextReleasesPagesUrl; ///< The URL of the page for the project releases on GitHub
extern QString const kBeeftextIssueTrackerUrl; ///< The URL of the page for the issue tracker on GitHub.
QString jsonFileDialogFilter(); ///< The Open/Save file dialog filter
QString jsonCsvFileDialogFilter(); ///< The file format filter for the import file picker dialog.
QString csvFileDialogFilter(); ///< The file format filter for CSV files.
extern QString const backupFileExtension; ///< The extension for backup files.
QString backupFileDialogFilter(); ///< The file format filter for the backup and restore file picker dialogs.
extern Qt::DateFormat const kJsonExportDateFormat; ///< The date/time export format used for JSon docs
extern QChar const kEmojiDelimiter; ///< The delimiter for emojis


} // namespace constants


#endif // #ifndef BEEFTEXT_CONSTANTS_H
