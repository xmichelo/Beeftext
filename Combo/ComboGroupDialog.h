/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__GROUP__DIALOG__H
#define BEEFTEXT__COMBO__GROUP__DIALOG__H


#include "ui_ComboGroupDialog.h"
#include "ComboGroup.h"


//**********************************************************************************************************************
/// \brief Combo groups dialog class
//**********************************************************************************************************************
class ComboGroupDialog: public QDialog
{
   Q_OBJECT
public: // static functions
   static bool run(SPComboGroup& group, QString const& title, QWidget* parent = nullptr); ///< Run the dialog

public: // member functions
   ComboGroupDialog(SPComboGroup& group, QString const& title, QWidget* parent = nullptr); ///< Default constructor
   ComboGroupDialog(ComboGroupDialog const&) = delete; ///< Disabled copy-constructor
   ComboGroupDialog(ComboGroupDialog&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboGroupDialog() = default; ///< Destructor
   ComboGroupDialog& operator=(ComboGroupDialog const&) = delete; ///< Disabled assignment operator
   ComboGroupDialog& operator=(ComboGroupDialog&&) = delete; ///< Disabled move assignment operator

private slots:
   void onActionOk(); ///< Slot for the OK action
   void onNameChanged(); ///< Slot for the changing of the name

private:  // member functions
   bool isInputValid(); ///< Check whether the user input is valid
   void updateGui(); ///< Update the GUI state

private: // data members
   Ui::ComboGroupDialog ui_; ///< The GUI for the dialog
   SPComboGroup group_; ///< The group
};


#endif // #ifndef BEEFTEXT__COMBO__GROUP__DIALOG__H
