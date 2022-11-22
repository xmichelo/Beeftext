/// \file
/// \author 
///
/// \brief Declaration of preferences pane interface.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PREF_PANE_H
#define BEEFTEXT_PREF_PANE_H


//****************************************************************************************************************************************************
/// \brief Abstract interface for preference panes.
//****************************************************************************************************************************************************
class PrefPane : public QFrame {
Q_OBJECT
public: // member functions
    explicit PrefPane(QWidget *parent = nullptr)
        : QFrame(parent) {} ///< Default constructor.
    PrefPane(PrefPane const &) = delete; ///< Disabled copy-constructor.
    PrefPane(PrefPane &&) = delete; ///< Disabled assignment copy-constructor.
    ~PrefPane() override = default; ///< Destructor.
    PrefPane &operator=(PrefPane const &) = delete; ///< Disabled assignment operator.
    PrefPane &operator=(PrefPane &&) = delete; ///< Disabled move assignment operator.
    virtual void load() const = 0; ///< load the preferences.
    virtual bool validateInput() { return true; } ///< Validate the user input
};


#endif // #ifndef BEEFTEXT_PREF_PANE_H
