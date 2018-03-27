/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include "PreferencesManager.h"
#include <Psapi.h>


namespace {


QString const kRegKeyAutoStart = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
QString const kPortableModeBeaconFileName = "Portable.bin"; ///< The name of the 'beacon' file used to detect if the application should run in portable mode
QString const kPortableAppsModeBeaconFileName = "PortableApps.bin"; ///< The name of the 'beacon file used to detect if the app is in PortableApps mode
QStringList const kShiftInsertPasteAppNames{ "mintty.exe", "putty.exe", "powershell.exe" }; ///< The list of application executables name that support only pasting using the Shift+Insert shortcut


//**********************************************************************************************************************
/// \brief Test if the application is running in portable mode
/// 
/// \return true if and only if the application is running in portable mode
//**********************************************************************************************************************
bool isInPortableMode_()
{
   QDir const appDir(QCoreApplication::applicationDirPath());
   return QFileInfo(appDir.absoluteFilePath(kPortableModeBeaconFileName)).exists() ||
      QFileInfo(appDir.absoluteFilePath(kPortableAppsModeBeaconFileName)).exists();
}


//**********************************************************************************************************************
/// \brief Test if the application is using the PortableApps layout
///
/// \return true if the application is running as part of the PortableApps
//**********************************************************************************************************************
bool usePortableAppsFolderLayout_()
{
   return QFileInfo(QDir(QCoreApplication::applicationDirPath())
      .absoluteFilePath(kPortableAppsModeBeaconFileName)).exists();
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


//**********************************************************************************************************************
/// \return true if the application is running as part of the PortableApps.com distribution
//**********************************************************************************************************************
bool usePortableAppsFolderLayout()
{
   return QFileInfo(QDir(QCoreApplication::applicationDirPath())
      .absoluteFilePath(kPortableAppsModeBeaconFileName)).exists();
}


//**********************************************************************************************************************
/// \return The name of the currently active application, including its extension (e.g. "explorer.exe")
/// \return A null string in case of failure
//**********************************************************************************************************************
QString getActiveExecutableFileName()
{
   WCHAR buffer[MAX_PATH + 1] = { 0 };
   DWORD processId = 0;
   GetWindowThreadProcessId(GetForegroundWindow(), &processId);
   HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
   if (!processHandle)
      return QString();
   bool const ok = GetModuleFileNameEx(processHandle, nullptr, buffer, MAX_PATH);
   CloseHandle(processHandle);
   return ok ? QFileInfo(QDir::fromNativeSeparators(QString::fromWCharArray(buffer))).fileName() : QString();
}


//**********************************************************************************************************************
/// We consider that by default, applications support Ctrl+V, except a few that are individually listed
///
/// \param[in] The name of the executable, including its extension (e.g. "putty.exe")
/// \return true if and only if the application support pasting using the Ctrl+V shortcut
//**********************************************************************************************************************
bool doesApplicationSupportCtrlVShortcut(QString const& appExeName)
{
   return !kShiftInsertPasteAppNames.contains(appExeName, Qt::CaseInsensitive);
}