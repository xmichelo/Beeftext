/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of backup restoration dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__BACKUP__RESTORE__DIALOG__H
#define BEEFTEXT__BACKUP__RESTORE__DIALOG__H


#include "ui_BackupRestoreDialog.h"


//**********************************************************************************************************************
/// \brief Backup restoration dialog class
//**********************************************************************************************************************
class BackupRestoreDialog: public QDialog
{
   Q_OBJECT
public: // static member functions
   static bool run(QWidget* parent = nullptr); ///< Run the dialog

public: // member functions
   BackupRestoreDialog(QWidget* parent = nullptr); ///< Default constructor
   BackupRestoreDialog(BackupRestoreDialog const&) = delete; ///< Disabled copy-constructor
   BackupRestoreDialog(BackupRestoreDialog&&) = delete; ///< Disabled assignment copy-constructor
   ~BackupRestoreDialog() = default; ///< Destructor
   BackupRestoreDialog& operator=(BackupRestoreDialog const&) = delete; ///< Disabled assignment operator
   BackupRestoreDialog& operator=(BackupRestoreDialog&&) = delete; ///< Disabled move assignment operator

private slots: 
   void onButtonRestore(); ///< Slot for the restore button

private: // member functions
   void fillCombo(); ///< Fill the combo box

private: // data members
   Ui::BackupRestoreDialog ui_; ///< The GUI for the dialog
};


#endif // #ifndef BEEFTEXT__BACKUP__RESTORE__DIALOG__H