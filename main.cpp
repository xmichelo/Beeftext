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
#include "ComboManager.h"
#include <XMiLib/SystemUtils.h>
#include <XMiLib/Exception.h>


using namespace xmilib;


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
      debugLog.addInfo(QObject::tr("%1 started.").arg(constants::kApplicationName));
      ComboManager& comboManager = ComboManager::instance(); // we make sure the combo manager singleton is instanciated
      QApplication app(argc, argv);
      app.setQuitOnLastWindowClosed(false);
      app.setOrganizationName(constants::kOrganizationName);
      app.setApplicationName(constants::kApplicationName);
      app.setApplicationDisplayName(constants::kApplicationName);
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
