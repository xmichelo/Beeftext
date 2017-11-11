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
#include "Combo/ComboManager.h"
#include <XMiLib/SystemUtils.h>
#include <XMiLib/Exception.h>


namespace {
   QString const kSharedMemoryKey = "Beeftext";
   QString const kLogFileName = "Log.txt";
}

using namespace xmilib;


void ensureAppDataDirExists(); ///< Make sure the application data folder exists


//**********************************************************************************************************************
/// \brief Application entry point
///
/// \param[in] argc The number of command line arguments
/// \param[in] argv The list of command line arguments
//**********************************************************************************************************************
int main(int argc, char *argv[])
{
   QString const kUnhandledException = QObject::tr("Unhandled Exception");
   DebugLog& debugLog = globals::debugLog();
      try
   {
      QApplication app(argc, argv);

      // check for an existing instance of the application
      QSharedMemory sharedMem(kSharedMemoryKey);
      if (!sharedMem.create(1))
      {
         QMessageBox::information(nullptr, QObject::tr("Already running"), 
            QObject::tr("Another instance of the application is already running."));
         return 1;
      }

      app.setQuitOnLastWindowClosed(false);
      app.setOrganizationName(constants::kOrganizationName);
      app.setApplicationName(constants::kApplicationName);
      app.setApplicationDisplayName(constants::kApplicationName);
      ensureAppDataDirExists();
      debugLog.enableLoggingToFile(QDir(globals::getAppDataDir()).absoluteFilePath(kLogFileName));
      debugLog.setMaxEntryCount(1);
      debugLog.addInfo(QObject::tr("%1 started.").arg(constants::kApplicationName));

      ComboManager& comboManager = ComboManager::instance(); // we make sure the combo manager singleton is instanciated
      MainWindow window;
      qint32 returnCode = app.exec();
      debugLog.addInfo(QObject::tr("Application exited with return code %1").arg(returnCode));
      return returnCode;
   }
   catch (xmilib::Exception const& e)
   {
      debugLog.addError(QObject::tr("Application crashed because of an unhandled exception: %1").arg(e.qwhat()));
      displaySystemErrorDialog(kUnhandledException, e.qwhat());
   }
   catch (std::exception const& e)
   {
      debugLog.addError(QObject::tr("Application crashed because of an unhandled exception: %1").arg(e.what()));
      displaySystemErrorDialog(kUnhandledException, e.what());
   }
   catch (...)
   {
      debugLog.addError(QObject::tr("Application crashed because of an unhandled exception."));
      displaySystemErrorDialog(kUnhandledException, QObject::tr("An unhandled exception occurred."));
   }
   return 1;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ensureAppDataDirExists()
{
   QString const path = globals::getAppDataDir();
   QDir dir(path);
   if (dir.exists())
      return;
   QDir().mkpath(path);
   if (!dir.exists())
      throw xmilib::Exception(QObject::tr("The application data folder '%1' could not be created")
         .arg(QDir::toNativeSeparators(path)));
}
