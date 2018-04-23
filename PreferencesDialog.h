/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preferences dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__PREFERENCES__DIALOG__H
#define BEEFTEXT__PREFERENCES__DIALOG__H


#include "ui_PreferencesDialog.h"
#include "PreferencesManager.h"
#include "LatestVersionInfo.h"


//**********************************************************************************************************************
/// \brief Preferences dialog class
//**********************************************************************************************************************
class PreferencesDialog: public QDialog
{
   Q_OBJECT
public: // member functions
   PreferencesDialog(QWidget* parent = nullptr); ///< Default constructor
   PreferencesDialog(PreferencesDialog const&) = delete; ///< Disabled copy-constructor
   PreferencesDialog(PreferencesDialog&&) = delete; ///< Disabled assignment copy-constructor
   ~PreferencesDialog() = default; ///< Destructor
   PreferencesDialog& operator=(PreferencesDialog const&) = delete; ///< Disabled assignment operator
   PreferencesDialog& operator=(PreferencesDialog&&) = delete; ///< Disabled move assignment operator

private: // member functions
   void loadPreferences() const; ///< Load the preferences
   void applyAutoStartPreference() const; ///< Apply the 'Autostart at login' preference
   void applyThemePreference() const; ///< Apply the 'theme' preference
   void applyComboListFolderPreference(QString const& folderPath, QString const& previousPath); ///< Apply the combo list folder preference
   void setUpdateCheckStatus(QString const& status); ///< Set the update check status label message (this is cleared after a few seconds)
   void changeEvent(QEvent *event) override; ///< Change event handler
   void updateGui() const; ///< Update the GUI state

private slots:
   void onActionResetToDefaultValues(); ///< Slot for the 'Reset to default values' action
   void onActionChangeComboListFolder(); ///< Slot for the 'Change combo list folder' action
   void onActionResetComboListFolder(); ///< Slot for the 'Reset combo list folder' action
   void onActionChangeShortcut() const; ///< Slot for the 'Change shortcut' action
   void onActionResetComboTriggerShortcut() const; ///< slot for the 'Reset combo trigger shortcut'
   void onActionRestoreBackup(); ///< Slot for the 'Restore backup action
   void onPlaySoundOnComboCheckChanged() const; ///< Slot for the changing of the 'Play sound on combo' check box
   void onAutoStartCheckChanged() const; ///< Slot for the changing of the 'Autostart at login' check box
   void onAutoCheckForUpdatesCheckChanged() const; ///< Slot for the changing of the 'Auto check for updates' check box
   void onUseCustomThemeCheckChanged() const; ///< Slot for the changing of the 'Use custom theme' check box
   void onRadioAutomaticComboTriggerChecked(bool checked) const; ///< Slot for the changing of the 'Use automatic substitution' radio button
   void onUseClipboardForComboSubstitutionCheckChanged() const; ///< Slot for the changing of the 'Use clipboard for combo substitution' check box
   void onAutoBackupCheckChanged(); ///< Slot for the changing of the 'Auto backup' check box
   void onLocaleChanged() const; ///< Slot for the change of the locale
   void onUpdateIsAvailable(SPLatestVersionInfo const& latestVersionInfo); ///< Slot for the availability of an update
   void onNoUpdateIsAvailable(); ///< Slot for the lack of availability of an update
   void onUpdateCheckFailed(); ///< Slot update check failure
   void onActionOk(); ///< Slot for the OK button

private: // data members
   Ui::PreferencesDialog ui_; ///< The GUI for the dialog
   PreferencesManager& prefs_; ///< The preferences manager
   QTimer updateCheckStatusTimer_; ///< The timer for clearing the update check status label
};


#endif // #ifndef BEEFTEXT__PREFERENCES__DIALOG__H