/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of application entry point
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "MainWindow.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include "SensitiveApplicationManager.h"
#include "EmojiManager.h"
#include "Update/UpdateManager.h"
#include "PreferencesManager.h"
#include "I18nManager.h"
#include "Combo/ComboManager.h"
#include "Combo/LastUseFile.h"
#include <XMiLib/SingleInstanceApp.h>
#include <XMiLib/SystemUtils.h>
#include <XMiLib/Exception.h>
#include "XMiLib/GlobalShortcut/GlobalShortcutManager.h"
#include <QtPlatformHeaders/QWindowsWindowFunctions>

using namespace xmilib;


void ensureDirExists(QString const& path); ///< Make sure a folder exists.
void ensureAppDataDirsExist(); ///< Make sure the application data folder exists
void ensureMainWindowHasAHandle(MainWindow& mainWindow); ///< Ensure that the main window has a Win32 handle
void removeFileMarkedForDeletion(); ///< Remove the software update file that may have been marker for deletion


//**********************************************************************************************************************
/// \brief Application entry point
///
/// \param[in] argc The number of command line arguments
/// \param[in] argv The list of command line arguments
//**********************************************************************************************************************
int main(int argc, char *argv[])
{
   qRegisterMetaType<SpLatestVersionInfo>(); // required to use SpLatestVersionInfo in a queued signal/slot connection
   qRegisterMetaType<SpGroup>(); // required to use SpGroup in a queued signal/slot connection
   QString const unhandledException = "Unhandled Exception";
   DebugLog& debugLog = globals::debugLog();
   try
   {
      QApplication app(argc, argv);

      // check for an existing instance of the application
      SingleInstanceApplication singleInstanceApp("BeeftextSingleInstanceIdentifier");
      if (!singleInstanceApp.isFirstInstance())
      {
         // SingleInstance app detected that another instance is running and 'put a flag in memory to indicate
         // to the other instance that another one tried to be created
#ifndef NODEBUG
         QMessageBox::information(nullptr, QString(), "Another instance of the application is already running");
#endif
         return 1;
      }

      QGuiApplication::setQuitOnLastWindowClosed(false);
      QGuiApplication::setOrganizationName(constants::kOrganizationName);
      QGuiApplication::setApplicationName(constants::kApplicationName);

      ensureAppDataDirsExist();
      PreferencesManager& prefs = PreferencesManager::instance();
      if (prefs.writeDebugLogFile())
         debugLog.enableLoggingToFile(globals::logFilePath());
      debugLog.setMaxEntryCount(1);
      debugLog.addInfo(QString("%1 started.").arg(constants::kApplicationName));
      removeFileMarkedForDeletion();
      prefs.checkRemoveAndNotifyAboutRemovalOfClipboardUsage(); // \todo remove this after Beeftext 8.0.
      ComboManager& comboManager = ComboManager::instance(); // we make sure the combo manager singleton is instanciated
      (void)UpdateManager::instance(); // we make sure the update manager singleton is instanciated
      (void)SensitiveApplicationManager::instance(); ///< We load the sensitive application files
      EmojiManager::instance().loadEmojis();
      MainWindow window;
#ifdef Q_OS_WIN
      QWindowsWindowFunctions::setWindowActivationBehavior(QWindowsWindowFunctions::AlwaysActivateWindow);
#endif
      ensureMainWindowHasAHandle(window);
      GlobalShortcutManager& shortcutManager = GlobalShortcutManager::instance();
      GlobalShortcut const* shortcut = shortcutManager.create(MOD_CONTROL | MOD_ALT | MOD_SHIFT, 'V');
      QObject::connect(shortcut, &GlobalShortcut::triggered, &window, &MainWindow::onActionEnableDisableBeeftext);
      if (!prefs.alreadyLaunched())
      {
         window.show();
         if ((!PreferencesManager::instance().alreadyLaunched()) && (QMessageBox::Yes == QMessageBox::information(
            &window, QObject::tr("Getting Started"), QObject::tr("New to Beeftext?\n\nDo you want to read a short "
            "'Getting Started' tutorial?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)))
            QDesktopServices::openUrl(QUrl(constants::kGettingStartedUrl));
      }
      QObject::connect(&singleInstanceApp, &SingleInstanceApplication::anotherInstanceWasLaunched,
         &window, &MainWindow::onAnotherAppInstanceLaunch);
      prefs.setAlreadyLaunched();
      qint32 const returnCode = QApplication::exec();
      saveLastUseDateTimes(comboManager.comboListRef());
      debugLog.addInfo(QString("Application exited with return code %1").arg(returnCode));
      I18nManager::instance().unloadTranslation(); // required to avoid crash because otherwise the app instance could be destroyed before the translators
      return returnCode;
   }
   catch (Exception const& e)
   {
      debugLog.addError(QString("Application crashed because of an unhandled exception: %1").arg(e.qwhat()));
      displaySystemErrorDialog(unhandledException, e.qwhat());
   }
   catch (std::exception const& e)
   {
      debugLog.addError(QString("Application crashed because of an unhandled exception: %1").arg(e.what()));
      displaySystemErrorDialog(unhandledException, e.what());
   }
   catch (...)
   {
      debugLog.addError(QString("Application crashed because of an unhandled exception."));
      displaySystemErrorDialog(unhandledException, QObject::tr("An unhandled exception occurred."));
   }
   return 1;
}


//**********************************************************************************************************************
/// \param[in] path The path
//**********************************************************************************************************************
void ensureDirExists(QString const& path)
{
   QDir const dir(path);
   if (dir.exists())
      return;
   QDir().mkpath(path);
   if (!dir.exists())
      throw Exception(QString("The application data folder '%1' could not be created")
         .arg(QDir::toNativeSeparators(path)));
}

//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ensureAppDataDirsExist()
{
   ensureDirExists(globals::appDataDir());
   ensureDirExists(globals::userTranslationRootFolderPath());
}


//**********************************************************************************************************************
/// The application only get a findable window handle (HWND) only if we show it. The uninstaller needs this handle
/// to request a shutdown of the application.
/// 
/// \param[in] mainWindow The main window
//**********************************************************************************************************************
void ensureMainWindowHasAHandle(MainWindow& mainWindow)
{
   mainWindow.setWindowOpacity(0);
   mainWindow.show();
   mainWindow.hide();
   mainWindow.setWindowOpacity(1);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void removeFileMarkedForDeletion()
{
   PreferencesManager& prefs = PreferencesManager::instance();
   DebugLog& debugLog = globals::debugLog();
   QString const path = prefs.fileMarkedForDeletionOnStartup();
   if (path.isEmpty())
      return;
   prefs.clearFileMarkedForDeletionOnStartup();
   QFile file(path);
   QString const nativePath = QDir::toNativeSeparators(path);
   if (!file.exists())
   {
      debugLog.addWarning(QString("The following file was marked for deletion but does not exist: %1")
         .arg(nativePath));
      return;
   }
   if (file.remove())
      debugLog.addInfo(QString("The following file was successfully removed: %1").arg(nativePath));
   else
      debugLog.addWarning(QString("The following file was marked for deletion but could not be removed: %1")
         .arg(nativePath));
}
