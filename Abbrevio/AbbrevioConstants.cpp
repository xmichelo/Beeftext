/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of application constants
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "AbbrevioConstants.h"


namespace constants {


xmilib::VersionNumber const kVersionNumber(16, 0);
QString const kApplicationName = "Abbrevio";
QString const kOrganizationName = "abbrevio.org";
QString const kAbbrevioWikiHomeUrl = "https://github.com/CanScribe/Abbrevio/wiki";
QString const kGettingStartedUrl = "https://github.com/CanScribe/Abbrevio/wiki/Getting-started";
QString const kAbbrevioWikiVariablesUrl = "https://github.com/CanScribe/Abbrevio/wiki/Variables";
QString const kAbbrevioReleasesPagesUrl = "https://github.com/CanScribe/Abbrevio/releases";
QString const kAbbrevioIssueTrackerUrl = "https://github.com/CanScribe/Abbrevio/issues";
QString const backupFileExtension = "btbackup";
QString const kKeyVariableRegExpStr(R"(#{key:(\w+)(?>:(\d+))?})");
QString const kShortcutVariableRegExpStr(R"(#{shortcut:(.+)})");
QString const kDelayVariableRegExpStr(R"(#{delay:(\d+)})");
QRegularExpression const kVariableRegExp(R"(#\{((.*?)(?<!\\))\})");


} // namespace constants
