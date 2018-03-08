/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo frame
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__FRAME__H
#define BEEFTEXT__COMBO__FRAME__H


#include "ui_ComboFrame.h"


//**********************************************************************************************************************
/// \brief Combo frame class
//**********************************************************************************************************************
class ComboFrame: public QFrame
{
   Q_OBJECT
public: // member functions
   ComboFrame(QWidget* parent = nullptr); ///< Default constructor
   ComboFrame(ComboFrame const&) = delete; ///< Disabled copy-constructor
   ComboFrame(ComboFrame&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboFrame() = default; ///< Destructor
   ComboFrame& operator=(ComboFrame const&) = delete; ///< Disabled assignment operator
   ComboFrame& operator=(ComboFrame&&) = delete; ///< Disabled move assignment operator
   GroupListWidget* comboGroupListWidget(); ///< Return the combo group list widget
   ComboTableWidget* comboTableWidget(); ///< Return the combo table widget

private: // data members
   Ui::ComboFrame ui_; ///< The GUI for the frame
};


#endif // #ifndef BEEFTEXT__COMBO__FRAME__H