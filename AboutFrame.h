/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of the 'About' frame class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__ABOUT__FRAME__H
#define BEEFTEXT__ABOUT__FRAME__H


#include "ui_AboutFrame.h"


//**********************************************************************************************************************
/// \brief A Frame for the about tab of the main window
//**********************************************************************************************************************
class AboutFrame: public QFrame
{
   Q_OBJECT
public: // member functions
   AboutFrame(QWidget* parent = nullptr); ///< Default constructor
	~AboutFrame() = default; ///< Default destructor
   AboutFrame(AboutFrame const&) = delete; ///< Disabled copy constructor
   AboutFrame(AboutFrame&&) = delete; ///< Disabled move constructor
   AboutFrame& operator=(AboutFrame const&) = delete; ///< Disabled assignment operator
   AboutFrame& operator=(AboutFrame&&) = delete; ///< Disabled move assignment operator

private: // member functions
   void completeText() const; ///< Complete the about box text with app name and version number
   void changeEvent(QEvent *event) override; ///< Event change handler

private: // data members
   Ui::AboutFrame ui_; ///< The GUI for the frame
};


#endif // #ifndef BEEFTEXT__ABOUT__FRAME__H