/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application


#include "stdafx.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include "PreferencesManager.h"


namespace {


QString const kRegKeyAutoStart = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
QString const kPortableModeBeaconFileName = "Portable.bin"; ///< The name of the 'beacon' file used to detect if the application should run in portable mode


//**********************************************************************************************************************
/// \brief Test if the application is running in portable mode
/// 
/// \return true if and only if the application is running in portable mode
//**********************************************************************************************************************
bool isInPortableMode_()
{
   return QFileInfo(QDir(QCoreApplication::applicationDirPath())
      .absoluteFilePath(kPortableModeBeaconFileName)).exists();
}


}


//**********************************************************************************************************************
/// To register the application for auto start at login, we use a registry key that contains the path of the installed
/// application. This key is written by the NSIS installer script. As a consequence, if the application has not be
/// installed using the installer (for example on a development system), this function will fail
///
/// \return true if the operation was successful
//**********************************************************************************************************************
bool registerApplicationForAutoStart()
{
   if (isInPortableMode())
      return false;
   QString const installedPath = PreferencesManager::instance().getInstalledApplicationPath();
   if (installedPath.isEmpty() || (!QFileInfo(installedPath).exists()))
      return false;
   
   QSettings(kRegKeyAutoStart, QSettings::NativeFormat).setValue(constants::kApplicationName, 
      QDir::toNativeSeparators(installedPath));
   return true;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void unregisterApplicationFromAutoStart()
{
   if (isInPortableMode())
   QSettings(kRegKeyAutoStart, QSettings::NativeFormat).remove(constants::kApplicationName);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void openLogFile()
{
   QDesktopServices::openUrl(QUrl::fromLocalFile(globals::logFilePath()));
}


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

