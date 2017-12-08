/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of update dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__UPDATE__DIALOG__H
#define BEEFTEXT__UPDATE__DIALOG__H


#include "ui_UpdateDialog.h"
#include "LatestVersionInfo.h"


//**********************************************************************************************************************
/// \brief A dialog for software updates
//**********************************************************************************************************************
class UpdateDialog: public QDialog
{
   Q_OBJECT
public: // member functions
	UpdateDialog(SPLatestVersionInfo lastestVersionInfo, QWidget* parent = nullptr); ///< Default constructor
	~UpdateDialog() = default; ///< Default destructor
	
private slots:
   void onActionInstall(); ///< Callback for the 'Install' action
   void onActionSkip(); ///< Callback for the 'Skip' action

private: // member functions
	UpdateDialog(UpdateDialog const&) = delete; ///< Disabled copy constructor
	UpdateDialog(UpdateDialog&&) = delete; ///< Disabled move constructor
	UpdateDialog& operator=(UpdateDialog const&) = delete; ///< Disabled assignment operator
	UpdateDialog& operator=(UpdateDialog&&) = delete; ///< Disabled move assignment operator

private: // data members
   Ui::UpdateDialog ui_; ///< The GUI for the dialog
   SPLatestVersionInfo latestVersionInfo_; ///< The latest version info
};


#endif // #ifndef BEEFTEXT__UPDATE__DIALOG__H

