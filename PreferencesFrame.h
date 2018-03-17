/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preferences frame
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__PREFERENCES_FRAME
#define BEEFTEXT__PREFERENCES_FRAME


#include "ui_PreferencesFrame.h"
#include "PreferencesManager.h"
#include "LatestVersionInfo.h"


//**********************************************************************************************************************
/// \brief A frame class for the application preferences
//**********************************************************************************************************************
class PreferencesFrame: public QFrame
{
   Q_OBJECT
public: // member functions
	PreferencesFrame(QWidget* parent = nullptr); ///< Default constructor
	~PreferencesFrame() = default; ///< Default destructor
	
private: // member functions
	PreferencesFrame(PreferencesFrame const&) = delete; ///< Disabled copy constructor
	PreferencesFrame(PreferencesFrame&&) = delete; ///< Disabled move constructor
	PreferencesFrame& operator=(PreferencesFrame const&) = delete; ///< Disabled assignment operator
	PreferencesFrame& operator=(PreferencesFrame&&) = delete; ///< Disabled move assignment operator
   void loadPreferences() const; ///< Load the preferences
   void applyAutoStartPreference() const; ///< Apply the 'Autostart at login' preference
   void applyThemePreference() const; ///< Apply the 'theme' preference
   void applyComboListFolderPreference(QString const& folderPath, QString const& previousPath); ///< Apply the combo list folder preference
   void setUpdateCheckStatus(QString const& status); ///< Set the update check status label message (this is cleared after a few seconds)
   void changeEvent(QEvent *event) override; ///< Change event handler
   void updateGui() const; ///< Update the GUI state

private slots:
   void onActionResetToDefaultValues(); ///< Slot for the 'Reset to default values' action
   static void onActionOpenLogFile(); ///< Slot for the 'Open log file' action
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

private: // data members
   Ui::PreferencesFrame ui_; ///< The GUI for the frame
   PreferencesManager& prefs_; ///< The preferences manager
   QTimer updateCheckStatusTimer_; ///< The timer for clearing the update check status label
};


#endif // #ifndef BEEFTEXT__PREFERENCES_FRAME