/// \file
/// \author 
///
/// \brief Declaration of combo picker window
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_COMBO_PICKER_WINDOW_H
#define BEEFTEXT_COMBO_PICKER_WINDOW_H


#include "ui_ComboPickerWindow.h"


void showComboPickerWindow(); ///< Show the combo picker window.


//**********************************************************************************************************************
/// \brief The combo picker window class
//**********************************************************************************************************************
class ComboPickerWindow: public QWidget
{
   Q_OBJECT
public: // member functions
   ComboPickerWindow(); ///< Default constructor
   ComboPickerWindow(ComboPickerWindow const&) = delete; ///< Disabled copy-constructor
   ComboPickerWindow(ComboPickerWindow&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboPickerWindow() = default; ///< Destructor
   ComboPickerWindow& operator=(ComboPickerWindow const&) = delete; ///< Disabled assignment operator
   ComboPickerWindow& operator=(ComboPickerWindow&&) = delete; ///< Disabled move assignment operator

private: // data member
   Ui::ComboPickerWindow ui_; ///< The GUI for the window
};


#endif // #ifndef BEEFTEXT_COMBO_PICKER_WINDOW_H
