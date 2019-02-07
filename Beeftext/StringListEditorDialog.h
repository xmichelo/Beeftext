/// \file
/// \author 
///
/// \brief Declaration of string list editor dialog class
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_STRING_LIST_EDITOR_DIALOG_H
#define BEEFTEXT_STRING_LIST_EDITOR_DIALOG_H


#include "ui_StringListEditorDialog.h"
#include "StringListEditorModel.h"


//**********************************************************************************************************************
/// \brief String list editor dialog class
//**********************************************************************************************************************
class StringListEditorDialog: public QDialog
{
   Q_OBJECT
public: // static member functions
   static void run(QWidget* parent = nullptr); ///< Run the dialog

public: // member functions
   explicit StringListEditorDialog(QWidget* parent = nullptr); ///< Default constructor
   StringListEditorDialog(StringListEditorDialog const&) = delete; ///< Disabled copy-constructor
   StringListEditorDialog(StringListEditorDialog&&) = delete; ///< Disabled assignment copy-constructor
   ~StringListEditorDialog() = default; ///< Destructor
   StringListEditorDialog& operator=(StringListEditorDialog const&) = delete; ///< Disabled assignment operator
   StringListEditorDialog& operator=(StringListEditorDialog&&) = delete; ///< Disabled move assignment operator

private slots:
   void onActionOk(); ///< Slot for the 'OK' action
   void onActionAddString(); ///< Slot for the 'Add string' action
   void onActionRemoveString(); ///< Slot for the 'Add string' action
   void onSelectionChanged(); ///< Slot for changes of the selection

private: // member function
   void updateGui(); ///< Update the GUI state

private: // data members
   Ui::StringListEditorDialog ui_ {}; ///< The GUI for the dialog
   StringListEditorModel model_; ///< The model used by the list view
};


#endif // #ifndef BEEFTEXT_STRING_LIST_EDITOR_DIALOG_H
