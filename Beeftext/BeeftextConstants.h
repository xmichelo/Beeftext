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
extern Qt::WindowFlags const kDefaultDialogFlags; ///< The default dialog class
extern QString const kBeeftextWikiHomeUrl; ///< The URL of the Beeftext wiki
extern QString const kBeeftextWikiVariablesUrl; ///< The URL of the page about variables in the Beeftext wiki
extern QString const kJsonFileDialogFilter; ///< The Open/Save file dialog filter
extern QString const kStyleSheet; // The application stylesheet
extern Qt::DateFormat const kJsonExportDateFormat; ///< The date/time export format used for JSon docs


} // namespace constants


#endif // #ifndef BEEFTEXT_CONSTANTS_H
