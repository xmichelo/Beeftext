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
#include <XMiLib/Exception.h>


namespace {


QString const kKeyAppExePath = "AppExePath"; ///< The settings key for the application executable path
QString const kKeyAutoStart = R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run)"; ///< The registry key for autostart


}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void applyAutostartParameters() {
    if (isInPortableMode())
        return;
    bool const autoStart = PreferencesManager::instance().autoStartAtLogin();
    QString registeredAppPath;
    bool const hasRegisteredApp = registeredApplicationForAutostart(registeredAppPath);
    registeredAppPath = QFileInfo(registeredAppPath).absoluteFilePath();
    xmilib::DebugLog &log = globals::debugLog();
    if (autoStart) {
        log.addInfo("Auto-start is enabled.");
        QString const &installedAppPath = QFileInfo(installedApplicationPath()).absoluteFilePath();
        if (installedAppPath.isEmpty()) {
            log.addError("Beeftext is not properly installed (installedAppPath is empty). The application cannot be "
                         "registered for auto-start.");
            return;
        }
        if (!QFileInfo(installedAppPath).exists()) {
            log.addError(QString("Beeftext is not properly installed (installedAppPath points to a non-existing "
                                 "file '%1'). The application cannot be registered for auto-start.").arg(installedAppPath));
            return;
        }

        if (hasRegisteredApp) {
            if (installedAppPath == registeredAppPath) {
                log.addInfo(QString("Beeftext is already properly registered in system for auto-start. Path is '%1'.")
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
    if (hasRegisteredApp) {
        log.addInfo(QString("Removing registered app from Windows auto-start '%1'")
            .arg(QDir::toNativeSeparators(registeredAppPath)));
        unregisterApplicationFromAutoStart();
    }
}


//****************************************************************************************************************************************************
/// The value for this preference is set by the NSIS installer. 
//****************************************************************************************************************************************************
QString installedApplicationPath() {
    QFileInfo const exeInfo = QFileInfo(QCoreApplication::applicationFilePath());
    QString const exePath = exeInfo.canonicalFilePath();
    QVariant const v = PreferencesManager::instance().settings().value(kKeyAppExePath).toString();
    try {
        if (!v.canConvert<QString>())
            throw xmilib::Exception("The path of the installed application is not available in the registry. ");
        QFileInfo const regExeInfo = QFileInfo(v.toString());
        if (!regExeInfo.exists())
            throw xmilib::Exception("The path of the installed application in the registry point to a non existing "
                                    "file. ");
        return regExeInfo.canonicalFilePath();
    }
    catch (xmilib::Exception const &e) {
        globals::debugLog().addWarning(QString("%1 The current executable path will be considered to be the installed "
                                               "application path : %2").arg(e.qwhat()).arg(QDir::toNativeSeparators(exePath)));
        return exePath;
    }
}


//****************************************************************************************************************************************************
/// \param[out] outPath The path of the application registered for autostart.
//****************************************************************************************************************************************************
bool registeredApplicationForAutostart(QString &outPath) {
    QSettings const settings(kKeyAutoStart, QSettings::NativeFormat);
    if (!settings.contains(constants::kApplicationName))
        return false;
    QVariant const v = settings.value(constants::kApplicationName);
    if (!v.canConvert<QString>())
        return false;
    outPath = QDir::fromNativeSeparators(v.toString());
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] appPath The path of the application.
//****************************************************************************************************************************************************
void registerApplicationForAutoStart(QString const &appPath) {
    QSettings(kKeyAutoStart, QSettings::NativeFormat).setValue(constants::kApplicationName, QDir::toNativeSeparators(appPath));
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void unregisterApplicationFromAutoStart() {
    QSettings(kKeyAutoStart, QSettings::NativeFormat).remove(constants::kApplicationName);
}


