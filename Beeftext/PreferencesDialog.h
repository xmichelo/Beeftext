/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preferences dialog
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_PREFERENCES_DIALOG_H
#define BEEFTEXT_PREFERENCES_DIALOG_H


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
   explicit PreferencesDialog(QWidget* parent = nullptr); ///< Default constructor
   PreferencesDialog(PreferencesDialog const&) = delete; ///< Disabled copy-constructor
   PreferencesDialog(PreferencesDialog&&) = delete; ///< Disabled assignment copy-constructor
   ~PreferencesDialog() = default; ///< Destructor
   PreferencesDialog& operator=(PreferencesDialog const&) = delete; ///< Disabled assignment operator
   PreferencesDialog& operator=(PreferencesDialog&&) = delete; ///< Disabled move assignment operator
   void loadPreferences(); ///< Load the preferences

private: // member functions
   void savePreferences(); ///< Save the preferences
   bool validateComboListFolderPath(); ///< Validate the selected combo list folder path
   void setUpdateCheckStatus(QString const& status); ///< Set the update check status label message (this is cleared after a few seconds)
   bool promptForAndRemoveAutoBackups(); ///< Ask the user if automatic backups should be deleted and if so, performs the operation
   void changeEvent(QEvent *event) override; ///< Change event handler

private slots:
   void onResetToDefaultValues(); ///< Slot for the 'Reset to default values' action
   void onChangeComboListFolder(); ///< Slot for the 'Change combo list folder' action
   void onResetComboListFolder(); ///< Slot for the 'Reset combo list folder' action
   void onOpenComboListFolder() const; ///< Slot for the 'Open' button of the combo list folder.
   void onChangeComboTriggerShortcut(); ///< Slot for the 'Change shortcut' action
   void onResetComboTriggerShortcut(); ///< slot for the 'Reset combo trigger shortcut'
   void onChangeComboPickerShortcut(); ///< Slot for the combo picker 'Change' button.
   void onResetComboPickerShortcut(); ///< Slot for the combo picker 'Reset' button.
   void onRestoreBackup(); ///< Slot for the 'Restore backup' action
   void onResetWarnings(); ///< Slot for teh 'Reset warnings' action
   void onOk(); ///< Slot for the OK button
   void onApply(); ///< Slot for the Apply button
   void onUpdateIsAvailable(SpLatestVersionInfo const& latestVersionInfo); ///< Slot for the availability of an update
   void onNoUpdateIsAvailable(); ///< Slot for the lack of availability of an update
   void onUpdateCheckStarted() const; ///< Slot for the starting of an update check
   void onUpdateCheckFinished() const; ///< Slot for the finishing of an update check
   void onUpdateCheckFailed(); ///< Slot update check failure
   void onEditSensitiveApplications(); ///< Slot for the 'Edit sensitive applications' action
   void onEditEmojiExcludedApplications(); ///< Slot for the 'Edit emoji excluded applications' action
   void updateGui() const; ///< Update the GUI state

private: // data members
   Ui::PreferencesDialog ui_ {}; ///< The GUI for the dialog
   PreferencesManager& prefs_; ///< The preferences manager
   QTimer updateCheckStatusTimer_; ///< The timer for clearing the update check status label
   SpShortcut triggerShortcut_; ///< The keyboard shortcut for manual combo trigger
   SpShortcut comboPickerShortcut_; ///< The keyboard shortcut for the combo picker.
   QString previousComboListPath_; ///< The previous path of the combo list
};


#endif // #ifndef BEEFTEXT_PREFERENCES_DIALOG_H