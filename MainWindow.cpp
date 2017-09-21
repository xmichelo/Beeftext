/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of the application's main window
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "MainWindow.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"


using namespace xmilib;


//**********************************************************************************************************************
//
//**********************************************************************************************************************
MainWindow::MainWindow()
   : QMainWindow()
   , debugLogWindow_(std::make_unique<DebugLogWindow>(&globals::debugLog()))
{
    ui_.setupUi(this);
    this->setupActions();
    this->setupSystemTrayIcon();
    if (constants::kShowLogOnStartup)
       debugLogWindow_->show();
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
   menu->addSeparator();
   menu->addAction(ui_.actionExit);
   menu->setActiveAction(ui_.actionShowMainWindow);

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
   case QSystemTrayIcon::DoubleClick:
      this->onActionShowMainWindow();
      break;
   case QSystemTrayIcon::Trigger: // a.k.a single click
      systemTrayIcon_.contextMenu()->popup(QCursor::pos());
      break;
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionShowMainWindow()
{
   this->show();
   this->raise();
   this->activateWindow();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionExit()
{
   qApp->quit();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionShowDebugLog()
{
   Q_ASSERT(debugLogWindow_);
   debugLogWindow_->show();
}


