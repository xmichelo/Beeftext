/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of the application's main window
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "MainWindow.h"
#include "AboutDialog.h"
#include "PreferencesDialog.h"
#include "PreferencesManager.h"
#include "Combo/ComboManager.h"
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
   this->setupSystemTrayIcon();
   this->menuBar()->insertMenu(ui_.menu_Advanced->menuAction(), ui_.frameCombos->groupListWidget()->menu(this));
   this->menuBar()->insertMenu(ui_.menu_Advanced->menuAction(), ui_.frameCombos->comboTableWidget()->createMenu(this));

   PreferencesManager& prefs = PreferencesManager::instance();
   this->restoreGeometry(prefs.mainWindowGeometry());
   connect(ui_.actionVisitBeeftextWiki, &QAction::triggered, []() 
      { QDesktopServices::openUrl(QUrl(constants::kBeeftextWikiHomeUrl)); });
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
      this->showWindow();
      // note we need to postpone the launch of the dialog to end the event handler ASAP, otherwise the application
      // that the file was dropped from will like be frozen until we complete the import dialog
      QTimer::singleShot(0, [urls, this]() {ui_.frameCombos->comboTableWidget()->
         runComboImportDialog(urls[0].toLocalFile()); });
   }
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void MainWindow::closeEvent(QCloseEvent*)
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

   bool enabled = ComboManager::instance().isEnabled();
   qApp->setApplicationDisplayName(constants::kApplicationName 
      + (isInPortableMode() ? tr(" - Portable Edition") : QString())
      + (enabled ? QString() : tr(" - PAUSED")));
   this->setWindowTitle(QString()); // force refresh of the title bar

   QIcon const icon(enabled ? ":/MainWindow/Resources/BeeftextIcon.ico" 
      : ":/MainWindow/Resources/BeeftextIconGrayscale.ico");
   systemTrayIcon_.setIcon(icon);
   systemTrayIcon_.setToolTip(constants::kApplicationName);
   systemTrayIcon_.show();
   qApp->setWindowIcon(icon);

   QMenu* menu = new QMenu(this);
   QAction* action = new QAction(tr("Open Beeftext"), this);
   connect(action, &QAction::triggered, [this]() { this->showWindow(); });
   menu->addAction(action);
   menu->addSeparator();
   ui_.actionEnableDisableBeeftext->setText(enabled ? tr("&Pause Beeftext"): tr("&Resume Beeftext"));
   menu->addAction(ui_.actionEnableDisableBeeftext);
   menu->addSeparator();

   menu->setDefaultAction(action);
   action = new QAction(tr("Preferences"), this);
   connect(action, &QAction::triggered, [this]() { this->onActionShowPreferencesDialog(); });
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
//
//**********************************************************************************************************************
void MainWindow::showWindow()
{
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
      this->showWindow();
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


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionEnableDisableBeeftext()
{
   ComboManager& comboManager = ComboManager::instance();
   comboManager.setEnabled(!comboManager.isEnabled());
   this->setupSystemTrayIcon();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionShowAboutDialog()
{
   AboutDialog().exec();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionShowPreferencesDialog()
{
   PreferencesDialog().exec();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionOpenLogFile()
{
   openLogFile();
}
