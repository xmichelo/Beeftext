/// \file
/// \author 
///
/// \brief Declaration of combos preferences pane.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PREFS_PANE_COMBOS_H
#define BEEFTEXT_PREFS_PANE_COMBOS_H


#include "PrefPane.h"
#include "ui_PrefPaneCombos.h"
#include "Preferences/PreferencesManager.h"


//****************************************************************************************************************************************************
/// \brief Combos preference pane.
//****************************************************************************************************************************************************
class PrefPaneCombos : public PrefPane {
Q_OBJECT
public: // member functions
    explicit PrefPaneCombos(QWidget *parent = nullptr); ///< Default constructor.
    PrefPaneCombos(PrefPaneCombos const &) = delete; ///< Disabled copy-constructor.
    PrefPaneCombos(PrefPaneCombos &&) = delete; ///< Disabled assignment copy-constructor.
    ~PrefPaneCombos() override = default; ///< Destructor.
    PrefPaneCombos &operator=(PrefPaneCombos const &) = delete; ///< Disabled assignment operator.
    PrefPaneCombos &operator=(PrefPaneCombos &&) = delete; ///< Disabled move assignment operator.
    void load() const override; ///< Load the content of the pane

private slots:
    void updateGui() const; ///< Update the GUI.
    void onRadioAutomaticComboTrigger(bool checked) const; ///< Slot for the 'Automatic combo trigger' button.
    void onCheckComboTriggersOnSpace(bool checked) const; ///< Slot for the 'Combo triggers on space' check box.
    void onCheckKeepFinalSpace(bool checked) const; ///< Slot for the 'Keep final space character' check box.
    void onChangeComboTriggerShortcut(); ///< Slot for the 'Change shortcut' action
    void onResetComboTriggerShortcut() const; ///< Slot for the 'Reset combo trigger shortcut'
    void onChangeDefaultMatchingMode() const; ///< Slot for the 'Default matching mode' combo value change.
    void onChangeDefaultCaseSensitivity() const; ///< Slot for the 'Default case sensitivity' combo value change.
    void onCheckEnablePickerWindow(bool checked); ///< Slot for the 'Enable combo picker' checkbox.
    void onChangePickerWindowShortcut(); ///< Slot for the combo picker 'Change' button.
    void onResetPickerWindowShortcut(); ///< Slot for the combo picker 'Reset' button.

private: // member functions
    void changeEvent(QEvent *) override; ///< Change event handler
    void applyComboPickerShortcut(); ///< Try to apply combo picker shortcut.

private: // data members
    Ui::PrefPaneCombos ui_ {}; ///< The GUI for the pane.
    PreferencesManager &prefs_; ///< The preferences.
};


#endif // #ifndef BEEFTEXT_PREFS_PANE_COMBOS_H
