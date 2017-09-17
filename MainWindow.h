/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of the application's main window
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__MAIN__WINDOW__H
#define BEEFTEXT__MAIN__WINDOW__H


#include "ui_MainWindow.h"


//**********************************************************************************************************************
/// \brief Main window class
//**********************************************************************************************************************
class MainWindow: public QMainWindow
{
   Q_OBJECT
public: // member functions
   MainWindow(QWidget* parent = nullptr); ///< Default constructor
	~MainWindow() = default; ///< Default destructor
	
private: // member functions
   MainWindow(MainWindow const&) = delete; ///< Disabled copy constructor
   MainWindow(MainWindow const&&) = delete; ///< Disabled move constructor
   MainWindow& operator=(MainWindow const&) = delete; ///< Disabled assignment operator
   MainWindow& operator=(MainWindow const&&) = delete; ///< Disabled move assignment operator
   
private: // data members
   Ui::MainWindow ui_; ///< The GUI for the window
};


#endif // #ifndef BEEFTEXT__MAIN__WINDOW__H


