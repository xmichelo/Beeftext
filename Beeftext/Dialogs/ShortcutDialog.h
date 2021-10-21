/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of shortcut dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_SHORTCUT_DIALOG_H
#define BEEFTEXT_SHORTCUT_DIALOG_H


#include "ui_ShortcutDialog.h"
#include "Shortcut.h"


//**********************************************************************************************************************
/// \brief A class for the shortcut editor dialog
//**********************************************************************************************************************
class ShortcutDialog: public QDialog
{
   Q_OBJECT
public: // static member functions
   static SpShortcut run(QWidget* parent = nullptr); ///< Run the shortcut dialog.
public: // member functions
   explicit ShortcutDialog(SpShortcut const& shortcut, QWidget* parent = nullptr); ///< Default constructor
	ShortcutDialog(ShortcutDialog const&) = delete; ///< Disabled copy constructor
	ShortcutDialog(ShortcutDialog&&) = delete; ///< Disabled move constructor
	~ShortcutDialog() override; ///< Default destructor
	ShortcutDialog& operator=(ShortcutDialog const&) = delete; ///< Disabled assignment operator
	ShortcutDialog& operator=(ShortcutDialog&&) = delete; ///< Disabled move assignment operator
   SpShortcut shortcut() const; ///< Get the shortcut

private slots:
   void onShortcutPressed(SpShortcut const& shortcut); ///< Slot for the pressing of a shortcut.

private: // member functions
   void updateGui() const; ///< Update the state of the GUI

private: // data members
   Ui::ShortcutDialog ui_; ///< The GUI for the dialog
   SpShortcut shortcut_; ///< The shortcut
   bool shortcutsWereEnabled_ { true }; ///< Were shortcuts enabled before the dialog was shown?
};


#endif // #ifndef BEEFTEXT_SHORTCUT_DIALOG_H