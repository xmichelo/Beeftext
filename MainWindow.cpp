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
#include "UpdateCheckWorker.h"
#include "UpdateDialog.h"
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
{
    ui_.setupUi(this);
    ui_.tabWidget->setCurrentIndex(0);
    
    this->setupActions();
    this->setupSystemTrayIcon();

    PreferencesManager& prefs = PreferencesManager::instance();
    this->restoreGeometry(prefs.mainWindowGeometry());
    if (prefs.autoCheckForUpdates())
      QTimer::singleShot(1000, this, &MainWindow::launchCheckForUpdate);
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
// 
//**********************************************************************************************************************
void MainWindow::launchCheckForUpdate()
{
   emit startedCheckingForUpdate();
   QThread *thread = new QThread;
   UpdateCheckWorker* worker = new UpdateCheckWorker;
   worker->moveToThread(thread);
   connect(thread, &QThread::started, worker, &UpdateCheckWorker::run);
   connect(worker, &UpdateCheckWorker::finished, this, &MainWindow::onUpdateCheckWorkerFinished);
   connect(worker, &UpdateCheckWorker::newVersionIsAvailable, this, &MainWindow::onNewVersionAvailable);
   thread->start();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onUpdateCheckWorkerFinished()
{
   UpdateCheckWorker *worker = dynamic_cast<UpdateCheckWorker*>(this->sender());
   if (!worker)
      throw xmilib::Exception(tr("An Internal error occurred while checking for updates."));
   QThread *thread = worker->thread();
   thread->quit();
   thread->wait();
   worker->deleteLater();
   thread->deleteLater();
   emit finishedCheckingForUpdate();
}


//**********************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
//**********************************************************************************************************************
void MainWindow::onNewVersionAvailable(SPLatestVersionInfo latestVersionInfo)
{
   if (!latestVersionInfo)
   {
      globals::debugLog().addError("New version notifier sent a null latest version information instance.");
      return;
   }
   UpdateDialog(latestVersionInfo, this).exec();
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
