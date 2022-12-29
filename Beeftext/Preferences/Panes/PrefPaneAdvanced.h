/// \file
/// \author 
///
/// \brief Declaration of the advanced preferences pane.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PREFS_PANE_ADVANCED_H
#define BEEFTEXT_PREFS_PANE_ADVANCED_H


#include "PrefPane.h"
#include "ui_PrefPaneAdvanced.h"
#include "Preferences/PreferencesManager.h"


//****************************************************************************************************************************************************
/// \brief The advanced preference pane.
//****************************************************************************************************************************************************
class PrefPaneAdvanced : public PrefPane {
Q_OBJECT
public: // member functions
    explicit PrefPaneAdvanced(QWidget *parent = nullptr); ///< Default constructor.
    PrefPaneAdvanced(PrefPaneAdvanced const &) = delete; ///< Disabled copy-constructor.
    PrefPaneAdvanced(PrefPaneAdvanced &&) = delete; ///< Disabled assignment copy-constructor.
    ~PrefPaneAdvanced() override = default; ///< Destructor.
    PrefPaneAdvanced &operator=(PrefPaneAdvanced const &) = delete; ///< Disabled assignment operator.
    PrefPaneAdvanced &operator=(PrefPaneAdvanced &&) = delete; ///< Disabled move assignment operator.
    void load() const override;

private slots:
    void updateGui() const; ///< Update the GUI of the pane.
    void onSpinDelayBetweenKeystrokesChanged(int value) const; ///< Slot for the 'Delay between keystrokes' spin value change.
    void onChangeComboListFolder(); ///< Slot for the 'Change combo list folder' action
    void onResetComboListFolder(); ///< Slot for the 'Reset combo list folder' action
    void onOpenComboListFolder() const; ///< Slot for the 'Open' button of the combo list folder.
    void onCheckWriteDebugLogFile(bool checked) const; ///< Slot the for 'Write debug log file' checkbox
    void onCheckUseLegacyCopyPaste(bool checked) const; ///< Slot for the 'Use legacy copy/paste'.
    void onCheckRestoreClipboardAfterSubstitution(bool checked) const; ///< Slot for the 'Restore clipboard after substitution' check box.
    void onCheckUseShiftInsertForPasting(bool checked) const; ///< Slot for the 'Use Shift+Insert for pasting' checkbox.
    void onCheckUseCustomPowerShellVersion(bool checked); ///< Slot for the 'Use custom PowerShell version' check box.
    void onChangeCustomPowershellVersion(); ///< Slot for the 'Change' button of the custom PowerShell version.
    void onCheckAutoBackup(bool value); ///< Slot for the 'auto backup' checkbox.
    void onCheckUseCustomBackupLocation(bool value) const; ///< Slot for the 'Use custom backup location' checkbox.
    void onChangeCustomBackupLocation(); ///< Slot for the 'Change' button of the custom backup location.
    void onRestoreBackup(); ///< Slot for the 'Restore backup' action
    void onEditSensitiveApplications(); ///< Slot for the 'Edit sensitive applications' action
    void onEditExcludedApplications(); ///< Slot for the 'Edit sensitive applications' action

private: // member functions
    void changeEvent(QEvent *event) override; ///< Change event handler.
    bool promptForAndRemoveAutoBackups(); ///< Ask the user if automatic backups should be deleted and if so, performs the operation

private: // data members
    Ui::PrefPaneAdvanced ui_ {}; ///< The GUI of the pane.
    PreferencesManager &prefs_; ///< The preferences manager.
};


#endif // #ifndef BEEFTEXT_PREFS_PANE_ADVANCED_H
