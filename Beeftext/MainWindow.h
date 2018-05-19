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
#ifndef NDEBUG
#include <XMiLib/StyleSheetEditor/StyleSheetEditor.h>
#endif // #ifndef NDEBUG


//**********************************************************************************************************************
/// \brief Main window class
//**********************************************************************************************************************
class MainWindow: public QMainWindow
{
   Q_OBJECT
public: // member functions
   MainWindow(MainWindow const&) = delete; ///< Disabled copy constructor
   MainWindow(MainWindow&&) = delete; ///< Disabled move constructor
   MainWindow(); ///< Default constructor
   ~MainWindow() = default; ///< Default destructor
   MainWindow& operator=(MainWindow const&) = delete; ///< Disabled assignment operator
   MainWindow& operator=(MainWindow&&) = delete; ///< Disabled move assignment operator

protected: // member functions
   /// \name Drag and drop functions
   ///\{
   void dragEnterEvent(QDragEnterEvent* event) override; ///< Drag enter event handler
   void dragMoveEvent(QDragMoveEvent* event) override; ///< Drag move event handler
   void dragLeaveEvent(QDragLeaveEvent* event) override; ///< Drag leave event handler
   void dropEvent(QDropEvent* event) override; ///< Drop event handler
   ///\}
   void closeEvent(QCloseEvent*) override; ///< Window close event handler

private: // member functions
   void setupSystemTrayIcon(); ///< Setup the system tray icon
   void changeEvent(QEvent *event) override; ///< Change event handler
   void showWindow(); ///< Ensure the window is visible, active and on top

signals:
   void startedCheckingForUpdate(); ///< Signal for the start of the update check
   void finishedCheckingForUpdate(); ///< Signal for the end of the update check (this signal is emitted BEFORE displaying the software update dialog)

private slots: 
   void onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason); ///< Slot for the activation of the system tray icon
   void onActionExit(); ///< Slot for the 'Exit' action
   void onActionEnableDisableBeeftext(); ///< Slot for the Enable/Disable Beeftext
   static void onActionShowAboutDialog(); ///< Slot for the 'Show About dialog' action
   static void onActionShowPreferencesDialog(); ///< Slot for the 'Show Preferences dialog' action
   static void onActionOpenLogFile(); ///< Slot for the 'Open Log File' action

private: // data members
   Ui::MainWindow ui_; ///< The GUI for the window
   QSystemTrayIcon systemTrayIcon_; ///< The system tray icon
#ifndef NDEBUG
   xmilib::StyleSheetEditor styleSheetEditor_; ///< The style sheet editor window
#endif // #ifndef NDEBUG
};


#endif // #ifndef BEEFTEXT_MAIN_WINDOW_H


