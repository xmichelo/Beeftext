/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of the application's main window
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "MainWindow.h"
#include "PreferencesManager.h"
#include "Update/UpdateCheckWorker.h"
#include "Update/UpdateDialog.h"
#include "Combo/ComboManager.h"
#include "Combo/ComboDialog.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


//**********************************************************************************************************************
//
//**********************************************************************************************************************
MainWindow::MainWindow()
   : QMainWindow()
#ifndef NDEBUG
   , styleSheetEditor_(nullptr)
#endif
{
   ui_.setupUi(this);
   ui_.tabWidget->setCurrentIndex(0);

   this->setupActions();
   this->setupSystemTrayIcon();

   PreferencesManager& prefs = PreferencesManager::instance();
   this->restoreGeometry(prefs.mainWindowGeometry());
}

//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::setupActions()
{
   QString const exitActionText = tr("E&xit %1").arg(constants::kApplicationName);
   ui_.actionExit->setText(exitActionText);
   ui_.actionExit->setToolTip(exitActionText);
   ui_.actionExit->setIconText(exitActionText);

   QString const showActionText = tr("&Open %1").arg(constants::kApplicationName);
   ui_.actionShowMainWindow->setText(showActionText);
   ui_.actionShowMainWindow->setToolTip(showActionText);
   ui_.actionShowMainWindow->setIconText(showActionText);

   connect(&systemTrayIcon_, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayIconActivated);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::setupSystemTrayIcon()
{
   systemTrayIcon_.setIcon(QIcon(":/MainWindow/Resources/BeeftextIcon.ico"));
   systemTrayIcon_.setToolTip(constants::kApplicationName);
   systemTrayIcon_.show();

   QMenu* menu = new QMenu(this);
   menu->addAction(ui_.actionShowMainWindow);
   menu->addAction(ui_.actionShowPreferences);
#ifndef NDEBUG
   QAction* actionShowStyleSheet = new QAction(this);
   actionShowStyleSheet->setText(tr("Show Stylesheet Editor"));
   connect(actionShowStyleSheet, &QAction::triggered, &styleSheetEditor_, &StyleSheetEditor::show);
   menu->addAction(actionShowStyleSheet);
#endif // #ifndef NDEBUG
   menu->addSeparator();
   menu->addAction(ui_.actionExit);
   menu->setDefaultAction(ui_.actionShowMainWindow);
   menu->addSeparator();

   systemTrayIcon_.setContextMenu(menu);
}


//**********************************************************************************************************************
/// An 'activation' in an action performed on the system tray icon.
/// \param[in] reason The reason for the activation
//**********************************************************************************************************************
void MainWindow::onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
   switch (reason)
   {
   case QSystemTrayIcon::Trigger: // a.k.a single click
      this->onActionShowMainWindow();
      break;
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionShowMainWindow()
{
   ui_.tabWidget->setCurrentIndex(0);
   this->show();
   this->raise();
   this->activateWindow();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionShowPreferences()
{
   ui_.tabWidget->setCurrentIndex(1);
   this->show();
   this->raise();
   this->activateWindow();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionExit()
{
   this->close(); // ensure the close event handler is called
   qApp->quit();
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void MainWindow::closeEvent(QCloseEvent *event)
{
   // note that we save the geometry every time we close the window, not the app, simply because otherwise we would
   // have to do it in the destructor, where the state of the window may be uncertain.
   PreferencesManager::instance().setMainWindowGeometry(this->saveGeometry());
}
