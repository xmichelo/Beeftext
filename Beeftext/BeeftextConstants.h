/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of application constants
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_CONSTANTS_H
#define BEEFTEXT_CONSTANTS_H


#include <XMiLib/VersionNumber/VersionNumber.h>


Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr)


namespace constants {


extern xmilib::VersionNumber const kVersionNumber; ///< The version number of the application.

QChar constexpr kEmojiDelimiter = '|'; ///< The delimiter for emojis
QColor constexpr blueBeeftextColor(0x25, 0x8c, 0xc0); ///< Return the blue color used for the GUI.
Qt::DateFormat constexpr kJsonExportDateFormat = Qt::ISODateWithMs; ///< The date/time export format used for JSon docs

extern QString const kApplicationName; ///< The application name
extern QString const kOrganizationName; ///< The organization name
extern QString const kBeeftextWikiHomeUrl; ///< The URL of the Beeftext wiki
extern QString const kGettingStartedUrl; ///< The URL of the Getting started page
extern QString const kBeeftextWikiVariablesUrl; ///< The URL of the page about variables in the Beeftext wiki
extern QString const kBeeftextReleasesPagesUrl; ///< The URL of the page for the project releases on GitHub
extern QString const kBeeftextIssueTrackerUrl; ///< The URL of the page for the issue tracker on GitHub.
extern QString const backupFileExtension; ///< The extension for backup files.
extern QString const kKeyVariableRegExpStr; ///< A regular expression matching the #{key:} variable.
extern QString const kShortcutVariableRegExpStr; ///< A regular expression matching the #{shortcut:} variable.
extern QString const kDelayVariableRegExpStr; ///< A regular expression matching the #{delay:} variable.
extern QRegularExpression const kVariableRegExp; ///< The following regular expression detects the first variable #{}, ensuring the closing } is not preceded by a \. Lazy (a.k.a. non-greedy) operators are used to match the first variable with the smallest possible contents inside the #{}.


enum EITemType {
    Combo = 0, ///< The combo item type.
    Emoji = 1, ///< The emoji item type.
}; ///< An enumeration for item type.


enum {
    TypeRole = Qt::UserRole, ///< The role for the type of item (combo or emoji).
    PointerRole, /// The role for the pointer to the item.
}; ///< Anonymous enumeration for Beeftext customs model roles


} // namespace constants


#endif // #ifndef BEEFTEXT_CONSTANTS_H
