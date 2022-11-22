/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of the 'About' frame class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_ABOUT_DIALOG_H
#define BEEFTEXT_ABOUT_DIALOG_H


#include "ui_AboutDialog.h"


//****************************************************************************************************************************************************
/// \brief A Frame for the about tab of the main window
//****************************************************************************************************************************************************
class AboutDialog : public QDialog {
Q_OBJECT
public: // member functions
    explicit AboutDialog(QWidget *parent = nullptr); ///< Default constructor
    ~AboutDialog() override = default; ///< Default destructor
    AboutDialog(AboutDialog const &) = delete; ///< Disabled copy constructor
    AboutDialog(AboutDialog &&) = delete; ///< Disabled move constructor
    AboutDialog &operator=(AboutDialog const &) = delete; ///< Disabled assignment operator
    AboutDialog &operator=(AboutDialog &&) = delete; ///< Disabled move assignment operator

private: // member functions
    void completeText() const; ///< Complete the about box text with app name and version number
    void changeEvent(QEvent *event) override; ///< Event change handler

private: // data members
    Ui::AboutDialog ui_; ///< The GUI for the frame
};


#endif // #ifndef BEEFTEXT_ABOUT_DIALOG_H