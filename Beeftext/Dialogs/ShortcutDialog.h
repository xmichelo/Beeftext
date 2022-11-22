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


//****************************************************************************************************************************************************
/// \brief A class for the shortcut editor dialog
//****************************************************************************************************************************************************
class ShortcutDialog : public QDialog {
Q_OBJECT
public: // static member functions
    static SpShortcut run(QWidget *parent, bool forGlobalRegistering); ///< Run the shortcut dialog.
    static bool run(QWidget *parent, SpShortcut &inOutShortcut, bool forGlobalRegistering); ///< Run the shortcut dialog with a default value

public: // member functions
    ShortcutDialog(ShortcutDialog const &) = delete; ///< Disabled copy constructor
    ShortcutDialog(ShortcutDialog &&) = delete; ///< Disabled move constructor
    ~ShortcutDialog() override; ///< Default destructor
    ShortcutDialog &operator=(ShortcutDialog const &) = delete; ///< Disabled assignment operator
    ShortcutDialog &operator=(ShortcutDialog &&) = delete; ///< Disabled move assignment operator
    SpShortcut shortcut() const; ///< Get the shortcut

private: // member functions
    explicit ShortcutDialog(QWidget *parent, SpShortcut const &shortcut, bool forGlobalRegistering); ///< Default constructor
    void updateGui() const; ///< Update the state of the GUI.
    bool validateShortcut(SpShortcut const &shortcut) const; ///< Validate the shortcut.

private slots:
    void onShortcutPressed(SpShortcut const &shortcut); ///< Slot for the pressing of a shortcut.

private: // data members
    Ui::ShortcutDialog ui_; ///< The GUI for the dialog
    SpShortcut shortcut_; ///< The shortcut
    bool forGlobalRegistering_ { false }; ///< Must the shortcut be acceptable for global registering.
    bool shortcutsWereEnabled_ { true }; ///< Were shortcuts enabled before the dialog was shown?
};


#endif // #ifndef BEEFTEXT_SHORTCUT_DIALOG_H