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
   static bool run(SPCombo const& combo, QString const& title = QString(), QWidget* parent = nullptr);

public: // member functions
	ComboDialog(SPCombo const& combo, QString const& title = QString(), QWidget* parent = nullptr); ///< Default constructor
	~ComboDialog() = default; ///< Default destructor
	SPCombo combo() const; ///< Get the combo
   bool checkAndReportInvalidCombo(); ///< Check the combo text against existing combos and report conflicts

private: // member functions
	ComboDialog(ComboDialog const&) = delete; ///< Disabled copy constructor
	ComboDialog(ComboDialog&&) = delete; ///< Disabled move constructor
	ComboDialog& operator=(ComboDialog const&) = delete; ///< Disabled assignment operator
	ComboDialog& operator=(ComboDialog&&) = delete; ///< Disabled move assignment operator
   void setupSubstitutionEditMenu(); /// Setup the editor menus
   QMenu* createComboVariableMenu(); ///< Create the combo variable menu
   void insertTextInSubstitutionEdit(QString const& text, bool move1CharLeft = false); ///< Insert some text at the current cursor position in the substitution text edit control

private slots:
   void onActionOk(); ///< Slot for the 'OK' action
   void onEditorContextMenuRequested(QPoint const& pos); ///< Slot for the display of the editor's context menu
   void updateGui() const; ///< Update the GUI state

private: // data members
   Ui::ComboDialog ui_; ///< The GUI for the dialog
   SPCombo combo_; ///< The combo
   ComboTextValidator validator_; ///< The validator for the combo text
   QMenu* substitutionEditMenu_; ///< The context menu for the substitution text editor
};


#endif // #ifndef BEEFTEXT__COMBO__DIALOG__H