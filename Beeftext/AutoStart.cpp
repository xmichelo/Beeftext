/// \file
/// \author 
///
/// \brief Implementation of auto start related function.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "AutoStart.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include "Preferences/PreferencesManager.h"


namespace {


QString const kKeyAppExePath = "AppExePath"; ///< The settings key for the application executable path
QString const kKeyAutoStart = R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run)"; ///< The registry key for autostart


}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void applyAutostartParameters()
{
   if (isInPortableMode())
      return;
   bool const autoStart = PreferencesManager::instance().autoStartAtLogin();
   QString registeredAppPath;
   bool const hasRegisteredApp = registeredApplicationForAutostart(registeredAppPath);
   registeredAppPath = QFileInfo(registeredAppPath).absoluteFilePath();
   xmilib::DebugLog& log = globals::debugLog();
   if (autoStart)
   {
      log.addInfo("Auto-start is enabled.");
      QString const& installedAppPath = QFileInfo(installedApplicationPath()).absoluteFilePath();
      if (installedAppPath.isEmpty() || !QFileInfo(installedAppPath).exists())
      {
         log.addError("Beeftext is not properly installed. The application cannot be registered for auto-start.");
         return;
      }

      if (hasRegisteredApp)
      {
         if (installedAppPath == registeredAppPath)
         {
            log.addInfo(QString("Beefetxt is already properly registered in system for auto-start. Path is '%1'.")
               .arg(QDir::toNativeSeparators(registeredAppPath)));
            return;
         }
         registerApplicationForAutoStart(installedAppPath);
         log.addWarning(QString("The path of the registered instance of Beeftext '%1' is invalid and has "
            "been modified to '%2'.").arg(registeredAppPath).arg(installedAppPath));
         return;
      }
      registerApplicationForAutoStart(installedAppPath);
      log.addInfo(QString("Beefext has been registered for auto-start. Application path is '%1'.")
         .arg(QDir::toNativeSeparators(installedAppPath)));
      return;
   }

   log.addInfo("Auto-start is disabled.");
   if (hasRegisteredApp)
   {
      log.addInfo(QString("Removing registered app from Windows auto-start '%1'")
         .arg(QDir::toNativeSeparators(registeredAppPath)));
      unregisterApplicationFromAutoStart();
   }
}


//**********************************************************************************************************************
/// The value for this preference is set by the NSIS installer. 
//**********************************************************************************************************************
QString installedApplicationPath()
{
   QVariant const v = PreferencesManager::instance().settings().value(kKeyAppExePath).toString();
   return v.canConvert<QString>() ? QDir::fromNativeSeparators(v.toString()) : QString();
}


//**********************************************************************************************************************
/// \
//**********************************************************************************************************************
bool registeredApplicationForAutostart(QString& outPath)
{
   QSettings const settings(kKeyAutoStart, QSettings::NativeFormat);
   if (!settings.contains(constants::kApplicationName))
      return false;
   QVariant const v = settings.value(constants::kApplicationName);
   if (!v.canConvert<QString>())
      return false;
   outPath = QDir::fromNativeSeparators(v.toString());
   return true;
}


//**********************************************************************************************************************
/// \param[in] appPath The path of the application.
//**********************************************************************************************************************
void registerApplicationForAutoStart(QString const& appPath)
{
   QSettings(kKeyAutoStart, QSettings::NativeFormat).setValue(constants::kApplicationName,
      QDir::toNativeSeparators(appPath));
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void unregisterApplicationFromAutoStart()
{
   QSettings(kKeyAutoStart, QSettings::NativeFormat).remove(constants::kApplicationName);
}


