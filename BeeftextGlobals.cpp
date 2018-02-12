/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of project global variables
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextGlobals.h"


namespace {

QString const kLogFileName = "log.txt"; ///< The name of the log file. Note keep .txt extension for easier opening by the system


//**********************************************************************************************************************
/// \brief Test if the application is running in portable mode
/// 
/// The application is considered as running in portable mode if and only there is a writable folder named Data in
/// the parent folder of the executable folder.
///  
/// This criterion is motivated by the folder hierarchy dictated by the 
/// [PortableApps.com format](https://portableapps.com/development/portableapps.com_format)
/// 
/// \return true if and only if the application is running in portable mode
//**********************************************************************************************************************
bool isInPortableMode_()
{
   QFileInfo fi(QDir(QCoreApplication::applicationDirPath() + "/../Data").absolutePath());
   qDebug() << QDir::toNativeSeparators(QString("portable data folder: %1").arg(fi.absoluteFilePath()));
   return fi.exists() && fi.isWritable();
}


} // anonymous namespace


namespace globals {


//**********************************************************************************************************************
/// \return The application debug log
//**********************************************************************************************************************
xmilib::DebugLog& debugLog()
{
   static xmilib::DebugLog log;
   return log;
}


//**********************************************************************************************************************
/// \return The location of the local storage folder for the application
//**********************************************************************************************************************
QString appDataDir()
{
   return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}


//**********************************************************************************************************************
/// \return The absolute path of the log file
//**********************************************************************************************************************
QString logFilePath()
{
   return QDir(globals::appDataDir()).absoluteFilePath(kLogFileName);
}


//**********************************************************************************************************************
/// \brief Test if the application is running in portable mode
///
/// \return true if and only if the application is running in portable mode
//**********************************************************************************************************************


//**********************************************************************************************************************
/// \return true if and only if the application is in portable mode
//**********************************************************************************************************************
bool isInPortableMode()
{
   // portable mode state cannot change during the execution of an instance of the application, so we 'cache' the
   // value using a static variable
   static bool result = isInPortableMode_();
   return result;
}


} // namespace globals
