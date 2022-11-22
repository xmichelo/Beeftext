/// \file
/// \author 
///
/// \brief Declaration of behavior preference pane.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PREFS_PANE_BEHAVIOR_H
#define BEEFTEXT_PREFS_PANE_BEHAVIOR_H


#include "PrefPane.h"
#include "Preferences/PreferencesManager.h"
#include "Update/UpdateManager.h"
#include "ui_PrefPaneBehavior.h"


//****************************************************************************************************************************************************
/// \brief Behavior preferences pane.
//****************************************************************************************************************************************************
class PrefPaneBehavior : public PrefPane {
Q_OBJECT
public: // member functions
    explicit PrefPaneBehavior(QWidget *parent = nullptr); ///< Default constructor.
    PrefPaneBehavior(PrefPaneBehavior const &) = delete; ///< Disabled copy-constructor.
    PrefPaneBehavior(PrefPaneBehavior &&) = delete; ///< Disabled assignment copy-constructor.
    ~PrefPaneBehavior() override = default; ///< Destructor.
    PrefPaneBehavior &operator=(PrefPaneBehavior const &) = delete; ///< Disabled assignment operator.
    PrefPaneBehavior &operator=(PrefPaneBehavior &&) = delete; ///< Disabled move assignment operator.
    void load() const override; ///< Load the content of the pane.

private slots:
    void updateGui() const; ///< Update the GUI for the pane.
    void onUpdateIsAvailable(SpLatestVersionInfo const &latestVersionInfo); ///< Slot for the availability of an update
    void onNoUpdateIsAvailable(); ///< Slot for the lack of availability of an update
    void onUpdateCheckStarted() const; ///< Slot for the starting of an update check
    void onUpdateCheckFinished() const; ///< Slot for the finishing of an update check
    void onUpdateCheckFailed(); ///< Slot update check failure
    void onCheckAutoCheckForUpdates(bool checked) const; ///< Slot for the 'Auto check for updated' checkbox.
    void onCheckAutoStart(bool checked) const; ///< Slot for the 'Auto start at login' checkbox.
    void onCheckPlaySoundOnCombo(bool checked) const; ///< Slot for the  'Play sound on combo' checkbox.
    void onCheckUseCustomSound(bool checked) const; ///< Slot for the 'Use custom sound' checkbox.
    void onChangeCustomSound() const; ///< Slot for the 'Change custom sound' button.
    void onPlaySoundButton() const; ///< Slot for the 'Play' button for custom sound.
    void onCheckEnableAppEnableDisableShortcut(bool checked) const; ///< Slot for the 'Enable app enable/disable shortcut' check box.
    void onChangeAppEnableDisableShortcut(); ///< Slot for the button Change of the app enable/disable shortcut.
    void onResetAppEnableDisableShortcut() const; ///< Slot for the button Default of the app enable/disable shortcut.

private: // member functions
    void changeEvent(QEvent *event) override; ///< Change event handler.
    void setUpdateCheckStatus(QString const &status); ///< Set the update check status label message (this is cleared after a few seconds)
    bool validateInput() override; ///< Validate the user input.

private: // data members
    Ui::PrefPaneBehavior ui_ {}; ///< The GUI for the pane.
    PreferencesManager &prefs_; ///< The preferences manager.
    QTimer updateCheckStatusTimer_; ///< The timer for clearing the update check status label

};


#endif // #ifndef BEEFTEXT_PREFS_PANE_BEHAVIOR_H
