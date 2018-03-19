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
#include "UpdateManager.h"
#include "I18nManager.h"
#include "Combo/ComboManager.h"
#include <XMiLib/SystemUtils.h>
#include <XMiLib/Exception.h>


namespace {
   QString const kSharedMemoryKey = "Beeftext";
}

using namespace xmilib;


void ensureAppDataDirExists(); ///< Make sure the application data folder exists
void ensureMainWindowHasAHandle(MainWindow& window); ///< Ensure that the main window has a Win32 handle
void removeFileMarkedForDeletion(); ///< Remove the software update file that may have been marker for deletion


//**********************************************************************************************************************
/// \brief Application entry point
///
/// \param[in] argc The number of command line arguments
/// \param[in] argv The list of command line arguments
//**********************************************************************************************************************
int main(int argc, char *argv[])
{
   qRegisterMetaType<SPLatestVersionInfo>(); // required to use SPLatestVersionInfo in a queued signal/slot connection
   qRegisterMetaType<SPGroup>(); // required to use SPGroup in a queued signal/slot connection
   QString const kUnhandledException = "Unhandled Exception";
   DebugLog& debugLog = globals::debugLog();
      try
   {
      QApplication app(argc, argv);

      // check for an existing instance of the application
      QSharedMemory sharedMem(kSharedMemoryKey);
      if (!sharedMem.create(1))
      {
         QMessageBox::information(nullptr, QObject::tr("Already Running"), 
            QObject::tr("Another instance of the application is already running."));
         return 1;
      }

      app.setQuitOnLastWindowClosed(false);
      app.setOrganizationName(constants::kOrganizationName);
      app.setApplicationName(constants::kApplicationName);

      ensureAppDataDirExists();
      debugLog.enableLoggingToFile(globals::logFilePath());
      debugLog.setMaxEntryCount(1);
      debugLog.addInfo(QString("%1 started.").arg(constants::kApplicationName));
      removeFileMarkedForDeletion();
      PreferencesManager& prefs = PreferencesManager::instance();
      I18nManager& i18nManager = I18nManager::instance();
      i18nManager.setLocale(prefs.locale());
      // ReSharper disable CppDeclaratorNeverUsed
      ComboManager& comboManager = ComboManager::instance(); // we make sure the combo manager singleton is instanciated
      UpdateManager& updateManager = UpdateManager::instance(); // we make sure the update manager singleton is instanciated
      // ReSharper restore CppDeclaratorNeverUsed
      if (prefs.useCustomTheme())
         qApp->setStyleSheet(constants::kStyleSheet); // some style on table view do not apply properly if applied before window creation
      MainWindow window;
      ensureMainWindowHasAHandle(window);
      if (!prefs.alreadyLaunched())
         window.show();
      prefs.setAlreadyLaunched();
      qint32 const returnCode = app.exec();
      debugLog.addInfo(QString("Application exited with return code %1").arg(returnCode));
      i18nManager.unloadTranslation(); // required to avoid crash because otherwise the app instance could be destroyed before the translators
      return returnCode;
   }
   catch (xmilib::Exception const& e)
   {
      debugLog.addError(QString("Application crashed because of an unhandled exception: %1").arg(e.qwhat()));
      displaySystemErrorDialog(kUnhandledException, e.qwhat());
   }
   catch (std::exception const& e)
   {
      debugLog.addError(QString("Application crashed because of an unhandled exception: %1").arg(e.what()));
      displaySystemErrorDialog(kUnhandledException, e.what());
   }
   catch (...)
   {
      debugLog.addError(QString("Application crashed because of an unhandled exception."));
      displaySystemErrorDialog(kUnhandledException, QObject::tr("An unhandled exception occurred."));
   }
   return 1;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ensureAppDataDirExists()
{
   QString const path = globals::appDataDir();
   QDir dir(path);
   if (dir.exists())
      return;
   QDir().mkpath(path);
   if (!dir.exists())
      throw xmilib::Exception(QString("The application data folder '%1' could not be created")
         .arg(QDir::toNativeSeparators(path)));
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


