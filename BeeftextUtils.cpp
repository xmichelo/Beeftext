/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application


#include "stdafx.h"
#include "BeeftextUtils.h"
#include "BeeftextConstants.h"
#include "PreferencesManager.h"


namespace {
   QString const kRegKeyAutoStart = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
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
   QString const installedPath = PreferencesManager::instance().getInstalledApplicationPath();
   if (installedPath.isEmpty() || (!QFileInfo(installedPath).exists()))
      return false;
   ;
   QSettings(kRegKeyAutoStart, QSettings::NativeFormat).setValue(constants::kApplicationName, 
      QDir::toNativeSeparators(installedPath));
   return true;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void unregisterApplicationFromAutoStart()
{
   QSettings(kRegKeyAutoStart, QSettings::NativeFormat).remove(constants::kApplicationName);
}


