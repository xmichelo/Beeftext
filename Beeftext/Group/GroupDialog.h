/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__GROUP__DIALOG__H
#define BEEFTEXT__COMBO__GROUP__DIALOG__H


#include "ui_GroupDialog.h"
#include "Group.h"


//**********************************************************************************************************************
/// \brief Combo groups dialog class
//**********************************************************************************************************************
class GroupDialog: public QDialog
{
   Q_OBJECT
public: // static functions
   static bool run(SPGroup& group, QString const& title, QWidget* parent = nullptr); ///< Run the dialog

public: // member functions
   GroupDialog(SPGroup& group, QString const& title, QWidget* parent = nullptr); ///< Default constructor
   GroupDialog(GroupDialog const&) = delete; ///< Disabled copy-constructor
   GroupDialog(GroupDialog&&) = delete; ///< Disabled assignment copy-constructor
   ~GroupDialog() = default; ///< Destructor
   GroupDialog& operator=(GroupDialog const&) = delete; ///< Disabled assignment operator
   GroupDialog& operator=(GroupDialog&&) = delete; ///< Disabled move assignment operator

private slots:
   void onActionOk(); ///< Slot for the OK action
   void onNameChanged(); ///< Slot for the changing of the name

private:  // member functions
   bool isInputValid(); ///< Check whether the user input is valid
   void updateGui(); ///< Update the GUI state

private: // data members
   Ui::GroupDialog ui_; ///< The GUI for the dialog
   SPGroup group_; ///< The group
};


#endif // #ifndef BEEFTEXT__COMBO__GROUP__DIALOG__H
