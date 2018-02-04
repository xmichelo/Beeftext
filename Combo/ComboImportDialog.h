/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo import dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__IMPORT__DIALOG
#define BEEFTEXT__COMBO__IMPORT__DIALOG


#include "ui_ComboImportDialog.h"
#include "ComboList.h"


//**********************************************************************************************************************
/// \brief Combo import dialog class
//**********************************************************************************************************************
class ComboImportDialog: public QDialog
{
   Q_OBJECT
public: // member functions
	ComboImportDialog(QWidget* parent = nullptr); ///< Default constructor
	ComboImportDialog(ComboImportDialog const&) = delete; ///< Disabled copy constructor
	ComboImportDialog(ComboImportDialog&&) = delete; ///< Disabled move constructor
	~ComboImportDialog() = default; ///< Default destructor
	ComboImportDialog& operator=(ComboImportDialog const&) = delete; ///< Disabled assignment operator
	ComboImportDialog& operator=(ComboImportDialog&&) = delete; ///< Disabled move assignment operator

protected: // member functions
   /// \name Drag and drop functions
   ///\{
   void dragEnterEvent(QDragEnterEvent* event); ///< Drag enter event handler
   void dragMoveEvent(QDragMoveEvent* event); ///< Drag move event handler
   void dragLeaveEvent(QDragLeaveEvent* event); ///< Drag leave event handler
   void dropEvent(QDropEvent* event); ///< Drop event handler
   ///\}

private: // data members
   void updateGui(); ///< Update the GUI state

private slots:
   void onActionImport(); ///< Slot for the 'Import' action
   void onActionCancel(); ///< Slot for the 'Cancel' action
   void onActionBrowse(); ///< Slot for the 'Browse' action
   void onEditPathTextChanged(QString const& text); ///< Slot for the changing of the content of the path edit control

private: // data members
   Ui::ComboImportDialog ui_; ///< The GUI for the dialog
   ComboList comboList_; ///< The list of combos to import
};


#endif // #ifndef BEEFTEXT__COMBO__IMPORT__DIALOG