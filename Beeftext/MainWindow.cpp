/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of the application's main window
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "MainWindow.h"
#include "Dialogs/AboutDialog.h"
#include "Preferences/PreferencesDialog.h"
#include "Preferences/PreferencesManager.h"
#include "Combo/ComboManager.h"
#include "Combo/ComboTableWidget.h"
#include "Group/GroupListWidget.h"
#include "BeeftextUtils.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include "InputManager.h"
#include <XMiLib/Exception.h>


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
MainWindow::MainWindow() {
    ui_.setupUi(this);
    groupsMenu_ = ui_.frameCombos->groupListWidget()->menu(this);
    combosMenu_ = ui_.frameCombos->comboTableWidget()->menu(this);
    this->setupSystemTrayIcon();
    this->menuBar()->insertMenu(ui_.menu_Advanced->menuAction(), groupsMenu_);
    this->menuBar()->insertMenu(ui_.menu_Advanced->menuAction(), combosMenu_);
    PreferencesManager const &prefs = PreferencesManager::instance();
    this->restoreWindowGeometry();
    ui_.actionOpenLogFile->setEnabled(prefs.writeDebugLogFile());
    connect(&InputManager::instance(), &InputManager::appEnableDisableShortcutTriggered, this, &MainWindow::onActionEnableDisableBeeftext);
    connect(ui_.actionVisitBeeftextWiki, &QAction::triggered, []() { QDesktopServices::openUrl(QUrl(constants::kBeeftextWikiHomeUrl)); });
    connect(ui_.actionGettingStarted, &QAction::triggered, []() { QDesktopServices::openUrl(QUrl(constants::kGettingStartedUrl)); });
    connect(ui_.actionShowReleaseNotes, &QAction::triggered, []() { QDesktopServices::openUrl(QUrl(constants::kBeeftextReleasesPagesUrl)); });
    connect(ui_.actionReportBug, &QAction::triggered, []() { QDesktopServices::openUrl(QUrl(constants::kBeeftextIssueTrackerUrl)); });
    connect(&InputManager::instance(), &InputManager::comboMenuShortcutTriggered, this, &MainWindow::onShowComboMenu);
    connect(&prefs, &PreferencesManager::writeDebugLogFileChanged, this, &MainWindow::onWriteDebugLogFileChanged);
#ifdef NDEBUG
    ui_.menu_Advanced->removeAction(ui_.actionShowLogWindow);
#endif
}


//****************************************************************************************************************************************************
/// \param[in] event The event 
//****************************************************************************************************************************************************
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}


//****************************************************************************************************************************************************
/// \param[in] event The event 
//****************************************************************************************************************************************************
void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}


//****************************************************************************************************************************************************
/// \param[in] event The event 
//****************************************************************************************************************************************************
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}


//****************************************************************************************************************************************************
/// \param[in] event The event 
//****************************************************************************************************************************************************
void MainWindow::dropEvent(QDropEvent *event) {
    QMimeData const *mimeData = event->mimeData();
    if (!mimeData->hasUrls())
        return;
    QList<QUrl> const urls = mimeData->urls();
    if (!urls.empty()) // should always be the case
    {
        event->acceptProposedAction();
        this->showWindow();
        QString const path = urls[0].toLocalFile();
        // note we need to postpone the launch of the dialogs to end the event handler ASAP, otherwise the application
        // that the file was dropped from will like be frozen until we complete the import dialog
        if (QFileInfo(path).suffix() == constants::backupFileExtension)
            QTimer::singleShot(0, [path, this]() { this->restoreBackup(path); });
        else
            QTimer::singleShot(0, [path, this]() { ui_.frameCombos->comboTableWidget()->runComboImportDialog(path); });
    }
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::closeEvent(QCloseEvent *) {
    // note that we save the geometry every time we close the window, not the app, simply because otherwise we would
    // have to do it in the destructor, where the state of the window may be uncertain.
    PreferencesManager const &prefs = PreferencesManager::instance();
    prefs.setMainWindowGeometry(this->saveGeometry());
    QSplitter const *splitter = ui_.frameCombos->splitter();
    if (splitter)
        prefs.setMainWindowSplitterState(splitter->saveState());
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void MainWindow::setupSystemTrayIcon() {
    disconnect(&systemTrayIcon_, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayIconActivated);
    connect(&systemTrayIcon_, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayIconActivated);
    QString const pausedIndicator = tr(" - PAUSED");
    bool const enabled = PreferencesManager::instance().beeftextEnabled();
    QGuiApplication::setApplicationDisplayName(constants::kApplicationName
                                               + (isInPortableMode() ? tr(" - Portable Edition") : QString())
                                               + (enabled ? QString() : pausedIndicator));
    this->setWindowTitle(QString()); // force refresh of the title bar

    QIcon const icon(enabled ? ":/MainWindow/Resources/BeeftextIcon.ico"
                             : ":/MainWindow/Resources/BeeftextIconGrayscale.ico");
    systemTrayIcon_.setIcon(icon);
    systemTrayIcon_.setToolTip(constants::kApplicationName + (enabled ? "" : pausedIndicator));
    systemTrayIcon_.show();
    QGuiApplication::setWindowIcon(icon);

    QMenu *menu = new QMenu(this);
    QAction *action = new QAction(tr("Open Beeftext"), this);
    connect(action, &QAction::triggered, [this]() { this->showWindow(); });
    menu->addAction(action);
    menu->setDefaultAction(action);

    action = new QAction(tr("New Combo"), this);
    connect(action, &QAction::triggered, [this]() { ui_.frameCombos->comboTableWidget()->onActionNewCombo(); });
    menu->addAction(action);

    action = new QAction(tr("New Group"), this);
    connect(action, &QAction::triggered, [this]() { ui_.frameCombos->groupListWidget()->onActionNewGroup(); });
    menu->addAction(action);

    menu->addSeparator();

    ui_.actionEnableDisableBeeftext->setText(enabled ? tr("&Pause Beeftext") : tr("&Resume Beeftext"));
    menu->addAction(ui_.actionEnableDisableBeeftext);

    menu->addSeparator();

    action = new QAction(tr("Preferences"), this);
    connect(action, &QAction::triggered, [this]() { onActionShowPreferencesDialog(); });
    menu->addAction(action);
#ifndef NDEBUG
    menu->addSeparator();
    QAction *actionShowLogWindow = new QAction(tr("Show Log Window"), this);
    connect(actionShowLogWindow, &QAction::triggered, this, &MainWindow::onActionShowLogWindow);
    menu->addAction(actionShowLogWindow);
    QAction *actionShowLog = new QAction(tr("Open Log File"), this);
    connect(actionShowLog, &QAction::triggered, []() { openLogFile(); });
    menu->addAction(actionShowLog);
    QAction *actionShowStyleSheet = new QAction(tr("Show Stylesheet Editor"), this);
    connect(actionShowStyleSheet, &QAction::triggered, &styleSheetEditor_, &xmilib::StyleSheetEditor::show);
    menu->addAction(actionShowStyleSheet);
#endif // #ifndef NDEBUG
    menu->addSeparator();
    menu->addAction(ui_.actionExit);
    menu->addSeparator();

    QMenu *oldMenu = systemTrayIcon_.contextMenu();
    systemTrayIcon_.setContextMenu(menu);
    if (oldMenu)
        oldMenu->deleteLater();
}


//****************************************************************************************************************************************************
/// \param[in] event The event
//****************************************************************************************************************************************************
void MainWindow::changeEvent(QEvent *event) {
    if (QEvent::LanguageChange == event->type()) {
        ui_.retranslateUi(this);
        groupsMenu_->setTitle(GroupListWidget::menuTitle());
        combosMenu_->setTitle(ComboTableWidget::menuTitle());
        this->setupSystemTrayIcon();
    }
    QMainWindow::changeEvent(event);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::showWindow() {
    this->show();
    this->raise();
    this->activateWindow();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::restoreWindowGeometry() {
    PreferencesManager const &prefs = PreferencesManager::instance();
    QByteArray array = prefs.mainWindowGeometry();
    if (!array.isEmpty())
        this->restoreGeometry(array);

    QSplitter *splitter = ui_.frameCombos->splitter();
    array = prefs.mainWindowSplitterState();
    if (splitter && !array.isEmpty())
        splitter->restoreState(array);
}


//****************************************************************************************************************************************************
/// \param[in] path The path of the file to backup
//****************************************************************************************************************************************************
void MainWindow::restoreBackup(QString const &path) {
    try {
        if ((ComboManager::instance().comboListRef().rowCount(QModelIndex()) > 0)
            && (!questionDialog(this, tr("Restore"), tr("If you restore a backup, all your current combos will "
                                                        "be deleted and replaced by the content of the backup file."), tr("Restore"), tr("Cancel"))))
            return;
        if (!ComboManager::instance().restoreBackup(path))
            throw xmilib::Exception("Could not restore backup file.");
    }
    catch (xmilib::Exception const &e) {
        QMessageBox::critical(this, tr("Error"), e.qwhat());
    }
}


//****************************************************************************************************************************************************
/// An 'activation' in an action performed on the system tray icon.
/// \param[in] reason The reason for the activation
//****************************************************************************************************************************************************
void MainWindow::onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (QSystemTrayIcon::Trigger == reason) //  a.k.a single click
        this->showWindow();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void MainWindow::onActionExit() {
    this->close(); // ensure the close event handler is called
    QCoreApplication::quit();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void MainWindow::onActionEnableDisableBeeftext() {
    PreferencesManager const &prefs = PreferencesManager::instance();
    bool const enabled = prefs.beeftextEnabled();
    prefs.setBeeftextEnabled(!enabled);
    this->setupSystemTrayIcon();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onShowComboMenu() {
    QMenu *menu = new QMenu(this);
    QAction *action = new QAction(tr("Open Beeftext"), this);
    connect(action, &QAction::triggered, [this]() { this->showWindow(); });
    menu->addAction(action);
    menu->popup(QCursor::pos());
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void MainWindow::onActionShowAboutDialog() {
    AboutDialog().exec();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void MainWindow::onActionShowPreferencesDialog() {
    if (!prefsDialog_)
        prefsDialog_ = new PreferencesDialog(this);
    else
        prefsDialog_->load(); // just to refresh if the user tweaked the registry values directly.
    prefsDialog_->show();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void MainWindow::onActionOpenLogFile() {
    openLogFile();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onActionShowLogWindow() {
    xmilib::DebugLogWindow &logWindow = globals::debugLogWindow();
    logWindow.setStyleSheet(QString());
    logWindow.show();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onActionBackup() {
    QString folder = PreferencesManager::instance().lastComboImportExportPath();
    if (!QFileInfo(folder).isDir())
        folder = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString const path = QFileDialog::getSaveFileName(this, tr("Backup"), QDir(folder).absoluteFilePath(QString("Beeftext.%1").arg(constants::backupFileExtension)), globals::backupFileDialogFilter());
    if (path.isEmpty())
        return;
    QString errMsg;
    if (!ComboManager::instance().comboListRef().save(path, true, &errMsg))
        QMessageBox::critical(this, tr("Error"), errMsg);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onActionRestore() {
    QString folder = PreferencesManager::instance().lastComboImportExportPath();
    if (!QFileInfo(folder).isDir())
        folder = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString const path = QFileDialog::getOpenFileName(this, tr("Restore"), QDir(folder).absolutePath(), globals::backupFileDialogFilter());
    if (path.isEmpty())
        return;
    this->restoreBackup(path);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onActionGenerateCheatSheet() {
    QString folder = PreferencesManager::instance().lastComboImportExportPath();
    if (!QFileInfo(folder).isDir())
        folder = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString const path = QFileDialog::getSaveFileName(this, tr("Generate Cheat Sheet"), QDir(folder).absoluteFilePath("BeeftextCheatSheet.csv"), globals::csvFileDialogFilter());
    if (path.isEmpty())
        return;
    QString errMsg;
    if (!ComboManager::instance().comboListRef().exportCheatSheet(path, &errMsg))
        QMessageBox::critical(this, tr("Error"), errMsg);

}


//****************************************************************************************************************************************************
/// \param[in] value The new value for the preference.
//****************************************************************************************************************************************************
void MainWindow::onWriteDebugLogFileChanged(bool value) const {
    ui_.actionOpenLogFile->setEnabled(value);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onAnotherAppInstanceLaunch() {
    this->showWindow();
}
