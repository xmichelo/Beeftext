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
#include "ComboKeywordValidator.h"
#include "Combo/Combo.h"


//**********************************************************************************************************************
/// \brief A dialog class for creating and editing a combo
//**********************************************************************************************************************
class ComboDialog: public QDialog
{
   Q_OBJECT
public:
   static bool run(SPCombo& combo, QString const& title = QString(), QWidget* parent = nullptr);

public: // member functions
   ComboDialog(SPCombo const& combo, QString const& title = QString(), QWidget* parent = nullptr); ///< Default constructor
   ComboDialog(ComboDialog const&) = delete; ///< Disabled copy constructor
   ComboDialog(ComboDialog&&) = delete; ///< Disabled move constructor
	~ComboDialog() = default; ///< Default destructor
   ComboDialog& operator=(ComboDialog const&) = delete; ///< Disabled assignment operator
   ComboDialog& operator=(ComboDialog&&) = delete; ///< Disabled move assignment operator

private: // member functions
   void setupSnipperEditMenu(); /// Setup the editor menus
   QMenu* createComboVariableMenu(); ///< Create the combo variable menu
   void insertTextInSnippetEdit(QString const& text, bool move1CharLeft = false); ///< Insert some text at the current cursor position in the snippet text edit control
   bool checkAndReportInvalidCombo(); ///< Check the keyword against existing combos and report conflicts

private slots:
   void onActionOk(); ///< Slot for the 'OK' action
   void onActionNewGroup(); ///< Slot for the 'New Group' action
   void onEditorContextMenuRequested(QPoint const& pos); ///< Slot for the display of the editor's context menu
   void updateGui() const; ///< Update the GUI state

private: // data members
   Ui::ComboDialog ui_; ///< The GUI for the dialog
   SPCombo combo_; ///< The combo
   ComboKeywordValidator validator_; ///< The validator for the keyword
   QMenu* snippetEditMenu_; ///< The context menu for the snippet text editor
};


#endif // #ifndef BEEFTEXT__COMBO__DIALOG__H