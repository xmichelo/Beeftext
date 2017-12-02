/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of dialog for creating/editing a combo
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  

#ifndef BEEFTEXT__COMBO__DIALOG__H
#define BEEFTEXT__COMBO__DIALOG__H


#include "ui_ComboDialog.h"
#include "ComboTextValidator.h"
#include "Combo/Combo.h"


//**********************************************************************************************************************
/// \brief A dialog class for creating and editing a combo
//**********************************************************************************************************************
class ComboDialog: public QDialog
{
   Q_OBJECT
public:
   static bool run(SPCombo combo, QString const& title = QString(), QWidget* parent = nullptr);

public: // member functions
	ComboDialog(SPCombo combo, QString const& title = QString(), QWidget* parent = nullptr); ///< Default constructor
	~ComboDialog() = default; ///< Default destructor
	SPCombo combo() const; ///< Get the combo
   bool checkAndReportInvalidCombo(); ///< Check the combo text against existing combos and report conflicts

private: // member functions
	ComboDialog(ComboDialog const&) = delete; ///< Disabled copy constructor
	ComboDialog(ComboDialog&&) = delete; ///< Disabled move constructor
	ComboDialog& operator=(ComboDialog const&) = delete; ///< Disabled assignment operator
	ComboDialog& operator=(ComboDialog&&) = delete; ///< Disabled move assignment operator

private slots:
   void onActionOk(); ///< Slot for the 'OK' action
   void updateGui(); ///< Update the GUI state

private: // data members
   Ui::ComboDialog ui_; ///< The GUI for the dialog
   SPCombo combo_; ///< The combo
   ComboTextValidator validator_; ///< The validator for the combo text
};


#endif // #ifndef BEEFTEXT__COMBO__DIALOG__H