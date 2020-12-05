/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of application constants
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextConstants.h"


namespace constants {


QString const kApplicationName = "Beeftext";
QString const kOrganizationName = "beeftext.org";
qint32 const kVersionMajor = 7;
qint32 const kVersionMinor = 2;
QString const kBeeftextWikiHomeUrl = "https://github.com/xmichelo/Beeftext/wiki";
QString const kGettingStartedUrl = "https://github.com/xmichelo/Beeftext/wiki/Getting-started";
QString const kBeeftextWikiVariablesUrl = "https://github.com/xmichelo/Beeftext/wiki/Variables";
QString const kBeeftextReleasesPagesUrl = "https://github.com/xmichelo/Beeftext/releases";
QString const kBeeftextIssueTrackerUrl = "https://github.com/xmichelo/Beeftext/issues";
QString jsonFileDialogFilter() { return QObject::tr("JSON files (*.json);;All files (*.*)"); }
QString jsonCsvFileDialogFilter() { return QObject::tr("JSON & CSV files (*.json *.csv);;JSON files (*.json);;"
   "CSV files (*.csv);;All files (*.*)"); }
QString csvFileDialogFilter() { return QObject::tr("CSV files (*.csv);;All files (*.*)"); }
QString const backupFileExtension = "btbackup";
QString backupFileDialogFilter() { return QObject::tr("Beeftext backup files (*.%1);;All files (*.*)").arg(backupFileExtension); }
Qt::DateFormat const kJsonExportDateFormat = Qt::ISODateWithMs;
QChar const kEmojiDelimiter = '|';


} // namespace constants
