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
#include "Shortcut.h"


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
   void loadPreferences(); ///< Load the preferences
   void savePreferences(); ///< Save the preferences
   void applyPreferences(); ///< Apply the preferences
   void applyAutoStartPreference() const; ///< Apply the 'Autostart at login' preference
   void applyThemePreference() const; ///< Apply the 'theme' preference
   void applyComboListFolderPreference(); ///< Apply the combo list folder preference
   void setUpdateCheckStatus(QString const& status); ///< Set the update check status label message (this is cleared after a few seconds)
   bool promptForAndRemoveAutoBackups(); ///< Ask the user if automatic backups should be deleted and if so, performs the operation
   void changeEvent(QEvent *event) override; ///< Change event handler

private slots:
   void onActionResetToDefaultValues(); ///< Slot for the 'Reset to default values' action
   void onActionChangeComboListFolder(); ///< Slot for the 'Change combo list folder' action
   void onActionResetComboListFolder(); ///< Slot for the 'Reset combo list folder' action
   void onActionChangeShortcut(); ///< Slot for the 'Change shortcut' action
   void onActionResetComboTriggerShortcut(); ///< slot for the 'Reset combo trigger shortcut'
   void onActionRestoreBackup(); ///< Slot for the 'Restore backup action
   void onActionOk(); ///< Slot for the OK button
   void onActionApply(); ///< Slot for the Apply button
   void onUpdateIsAvailable(SPLatestVersionInfo const& latestVersionInfo); ///< Slot for the availability of an update
   void onNoUpdateIsAvailable(); ///< Slot for the lack of availability of an update
   void onUpdateCheckFailed(); ///< Slot update check failure
   void updateGui() const; ///< Update the GUI state

private: // data members
   Ui::PreferencesDialog ui_; ///< The GUI for the dialog
   PreferencesManager& prefs_; ///< The preferences manager
   QTimer updateCheckStatusTimer_; ///< The timer for clearing the update check status label
   SPShortcut triggerShortcut_; ///< The keyboard shortcut for manual combo trigger
   QString previousComboListPath_; ///< The previous path of the combo list
};


#endif // #ifndef BEEFTEXT__PREFERENCES__DIALOG__H