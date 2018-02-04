/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of the application's main window
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "MainWindow.h"
#include "Combo/ComboManager.h"
#include "PreferencesManager.h"
#include "BeeftextUtils.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"


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

   this->setupSystemTrayIcon();
   PreferencesManager& prefs = PreferencesManager::instance();
   this->restoreGeometry(prefs.mainWindowGeometry());
   connect(ui_.actionVisitBeeftextWiki, &QAction::triggered, []() 
      { QDesktopServices::openUrl(QUrl(constants::kBeeftextWikiHomeUrl)); });
   connect(ui_.actionShowAboutTab, &QAction::triggered, [this]() { this->showTab(2); });
}


//**********************************************************************************************************************
/// \param[in] event The event 
//**********************************************************************************************************************
void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
   if (event->mimeData()->hasUrls())
      event->acceptProposedAction();
}


//**********************************************************************************************************************
/// \param[in] event The event 
//**********************************************************************************************************************
void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
   if (event->mimeData()->hasUrls())
      event->acceptProposedAction();
}


//**********************************************************************************************************************
/// \param[in] event The event 
//**********************************************************************************************************************
void MainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
   event->accept();
}


//**********************************************************************************************************************
/// \param[in] event The event 
//**********************************************************************************************************************
void MainWindow::dropEvent(QDropEvent* event)
{
   QMimeData const* mimeData = event->mimeData();
   if (!mimeData->hasUrls())
      return;
   QList<QUrl> urls = mimeData->urls();
   if (urls.size() > 0) // should always be the case
   {
      event->acceptProposedAction();
      this->showTab(0);
      // note we need to postpone the launch of the dialog to end the event handler ASAP, otherwise the application
      // that the file was dropped from will like be frozen until we complete the import dialog
      QTimer::singleShot(0, [urls, this]() {ui_.frameCombos->runComboImportDialog(urls[0].toLocalFile()); });
   }
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


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::setupSystemTrayIcon()
{
   disconnect(&systemTrayIcon_, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayIconActivated);
   connect(&systemTrayIcon_, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayIconActivated);

   systemTrayIcon_.setIcon(QIcon(":/MainWindow/Resources/BeeftextIcon.ico"));
   systemTrayIcon_.setToolTip(constants::kApplicationName);
   systemTrayIcon_.show();

   QMenu* menu = new QMenu(this);
   QAction* action = new QAction("Open Beeftext", this);
   connect(action, &QAction::triggered, [this]() { this->showTab(0); });
   menu->addAction(action);
   menu->setDefaultAction(action);
   action = new QAction("Preferences", this);
   connect(action, &QAction::triggered, [this]() { this->showTab(1); });
   menu->addAction(action);
#ifndef NDEBUG
   menu->addSeparator();
   QAction* actionShowLog = new QAction(tr("Open Log File"), this);
   connect(actionShowLog, &QAction::triggered, []() { openLogFile(); });
   menu->addAction(actionShowLog);
   QAction* actionShowStyleSheet = new QAction(tr("Show Stylesheet Editor"), this);
   connect(actionShowStyleSheet, &QAction::triggered, &styleSheetEditor_, &StyleSheetEditor::show);
   menu->addAction(actionShowStyleSheet);
#endif // #ifndef NDEBUG
   menu->addSeparator();
   menu->addAction(ui_.actionExit);
   menu->addSeparator();

   QMenu* oldMenu = systemTrayIcon_.contextMenu();
   systemTrayIcon_.setContextMenu(menu);
   if (oldMenu)
      oldMenu->deleteLater();
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void MainWindow::changeEvent(QEvent *event)
{
   if (QEvent::LanguageChange == event->type())
   {
      ui_.retranslateUi(this);
      this->setupSystemTrayIcon();
   }
   QMainWindow::changeEvent(event);
}


//**********************************************************************************************************************
/// \param[in] index the index of the tab to show
//**********************************************************************************************************************
void MainWindow::showTab(qint32 index)
{
   ui_.tabWidget->setCurrentIndex(index);
   this->show();
   this->raise();
   this->activateWindow();
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
      this->showTab(0);
      break;
   default:
      break;
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionExit()
{
   this->close(); // ensure the close event handler is called
   qApp->quit();
}

