/// \file
/// \author 
///
/// \brief Declaration of preferences pane interface.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PREFS_PANE_H
#define BEEFTEXT_PREFS_PANE_H


//**********************************************************************************************************************
/// \brief Abstract interface for preference panes.
//**********************************************************************************************************************
class PrefsPane: public QFrame
{
   Q_OBJECT
public: // member functions
   explicit PrefsPane(QWidget* parent = nullptr): QFrame(parent) {} ///< Default constructor.
   PrefsPane(PrefsPane const&) = delete; ///< Disabled copy-constructor.
   PrefsPane(PrefsPane&&) = delete; ///< Disabled assignment copy-constructor.
   ~PrefsPane() override = default; ///< Destructor.
   PrefsPane& operator=(PrefsPane const&) = delete; ///< Disabled assignment operator.
   PrefsPane& operator=(PrefsPane&&) = delete; ///< Disabled move assignment operator.
   virtual void load() const = 0; ///< load the preferences.
   virtual bool validateInput() { return true; } ///< Validate the user input
};


#endif // #ifndef BEEFTEXT_PREFS_PANE_H
