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
   void loadPreferences(); ///< Load the preferences
   void applyAutoStartPreference(); ///< Apply the 'Autostart at login' preference

private slots:
   void onActionResetToDefaultValues(); ///< Slot for the 'Reset to default values' action
   void onPlaySoundOnComboCheckChanged(); ///< Slot for the changing of the 'Play sound on combo' check box
   void onAutoStartCheckChanged(); ///< Slot for the changing of the 'Autostart at login' check box
   void onAutoCheckForUpdatesCheckChanged(); ///< Slot for the changing of the 'Auto check for updates' check box

private: // data members
   Ui::PreferencesFrame ui_; ///< The GUI for the frame
   PreferencesManager& prefs_; ///< The preferences manager
};


#endif // #ifndef BEEFTEXT__PREFERENCES_FRAME