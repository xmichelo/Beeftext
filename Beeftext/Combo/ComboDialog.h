/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of dialog for creating/editing a combo
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  

#ifndef BEEFTEXT_COMBO_DIALOG_H
#define BEEFTEXT_COMBO_DIALOG_H


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
   static bool run(SpCombo& combo, QString const& title = QString(), QWidget* parent = nullptr);

public: // member functions
   explicit ComboDialog(SpCombo const& combo, QString const& title = QString(), QWidget* parent = nullptr); ///< Default constructor
   ComboDialog(ComboDialog const&) = delete; ///< Disabled copy constructor
   ComboDialog(ComboDialog&&) = delete; ///< Disabled move constructor
	~ComboDialog() = default; ///< Default destructor
   ComboDialog& operator=(ComboDialog const&) = delete; ///< Disabled assignment operator
   ComboDialog& operator=(ComboDialog&&) = delete; ///< Disabled move assignment operator

private: // member functions
   QMenu* createComboVariableMenu(); ///< Create the combo variable menu
   void insertTextInSnippetEdit(QString const& text, bool move1CharLeft = false) const; ///< Insert some text at the current cursor position in the snippet text edit control
   bool checkAndReportInvalidCombo(); ///< Check the keyword against existing combos and report conflicts
   void setMatchingComboValue(bool useLooseMatching) const; ///< Set the 'Matching' combo value
   bool matchingComboValue() const; ///<  Read the matching mode from the 'Matching' combo value
   void setUseHtmlComboValue(bool useHtml) const; ///< Set the value for the 'Use HTML' combo.
   bool useHtmlComboValue() const; ///< Get the value for the 'HTML' combo.

private slots:
   void onActionOk(); ///< Slot for the 'OK' action
   void onActionNewGroup(); ///< Slot for the 'New Group' action
   void onEditorContextMenuRequested(QPoint const& pos); ///< Slot for the display of the editor's context menu
   void updateGui() const; ///< Update the GUI state
   void onUseHtmlChanged() const; ///< Slot for the change of the 'Use rich text' combo.

private: // data members
   Ui::ComboDialog ui_ {}; ///< The GUI for the dialog
   SpCombo combo_ {nullptr}; ///< The combo
   ComboKeywordValidator validator_; ///< The validator for the keyword
};


#endif // #ifndef BEEFTEXT_COMBO_DIALOG_H