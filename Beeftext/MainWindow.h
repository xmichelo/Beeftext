/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of the application's main window
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_MAIN_WINDOW_H
#define BEEFTEXT_MAIN_WINDOW_H


#include "ui_MainWindow.h"
#include "Preferences/PreferencesDialog.h"


#ifndef NDEBUG


#include <XMiLib/StyleSheetEditor/StyleSheetEditor.h>


#endif // #ifndef NDEBUG


//****************************************************************************************************************************************************
/// \brief Main window class
//****************************************************************************************************************************************************
class MainWindow : public QMainWindow {
Q_OBJECT
public: // member functions
    MainWindow(MainWindow const &) = delete; ///< Disabled copy constructor
    MainWindow(MainWindow &&) = delete; ///< Disabled move constructor
    MainWindow(); ///< Default constructor
    ~MainWindow() override = default; ///< Default destructor
    MainWindow &operator=(MainWindow const &) = delete; ///< Disabled assignment operator
    MainWindow &operator=(MainWindow &&) = delete; ///< Disabled move assignment operator

public slots:
    void onAnotherAppInstanceLaunch(); ///< Slot for the launch of another instance of the app
    void onActionEnableDisableBeeftext(); ///< Slot for the Enable/Disable Beeftext
    void onShowComboMenu(); ///< Show the combo menu

protected: // member functions
    /// \name Drag and drop functions
    ///\{
    void dragEnterEvent(QDragEnterEvent *event) override; ///< Drag enter event handler
    void dragMoveEvent(QDragMoveEvent *event) override; ///< Drag move event handler
    void dragLeaveEvent(QDragLeaveEvent *event) override; ///< Drag leave event handler
    void dropEvent(QDropEvent *event) override; ///< Drop event handler
    ///\}
    void closeEvent(QCloseEvent *) override; ///< Window close event handler

private: // member functions
    void setupSystemTrayIcon(); ///< Setup the system tray icon
    void changeEvent(QEvent *event) override; ///< Change event handler
    void showWindow(); ///< Ensure the window is visible, active and on top
    void restoreWindowGeometry(); ///< Restore the geometry of the window.
    void restoreBackup(QString const &path); ///< The path of the backup file

private slots:
    void onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason); ///< Slot for the activation of the system tray icon
    void onActionExit(); ///< Slot for the 'Exit' action
    static void onActionShowAboutDialog(); ///< Slot for the 'Show About dialog' action
    void onActionShowPreferencesDialog(); ///< Slot for the 'Show Preferences dialog' action
    static void onActionOpenLogFile(); ///< Slot for the 'Open Log File' action
    static void onActionShowLogWindow(); ///< Slot for the 'Show Log Window action.
    void onActionBackup(); ///< Slot for the 'Backup' action.
    void onActionRestore(); ///< Slot for the 'Restore' action.
    void onActionGenerateCheatSheet(); ///< Slot for the 'Generate Cheat Sheet' action.
    void onWriteDebugLogFileChanged(bool value) const; ///< Slot for the change of the 'Write debug log file' preference.

private: // data members
    Ui::MainWindow ui_ {}; ///< The GUI for the window
    QSystemTrayIcon systemTrayIcon_; ///< The system tray icon
    PreferencesDialog *prefsDialog_ { nullptr }; ///< The preference dialog
    QMenu *groupsMenu_ { nullptr }; ///< The groups menu in the main menu
    QMenu *combosMenu_ { nullptr }; ///< The combos menu in the main menu
#ifndef NDEBUG
    xmilib::StyleSheetEditor styleSheetEditor_ { nullptr }; ///< The style sheet editor window
#endif // #ifndef NDEBUG
};


#endif // #ifndef BEEFTEXT_MAIN_WINDOW_H


