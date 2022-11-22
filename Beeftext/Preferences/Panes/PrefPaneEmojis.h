/// \file
/// \author 
///
/// \brief Declaration of preference pane for emojis
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PREFS_PANE_EMOJIS_H
#define BEEFTEXT_PREFS_PANE_EMOJIS_H


#include "ui_PrefPaneEmojis.h"
#include "PrefPane.h"
#include "Preferences/PreferencesManager.h"


//****************************************************************************************************************************************************
/// \brief The preference pane for emojis.
//****************************************************************************************************************************************************
class PrefPaneEmojis : public PrefPane {
Q_OBJECT
public: // member functions
    explicit PrefPaneEmojis(QWidget *parent = nullptr); ///< Default constructor.
    PrefPaneEmojis(PrefPaneEmojis const &) = delete; ///< Disabled copy-constructor.
    PrefPaneEmojis(PrefPaneEmojis &&) = delete; ///< Disabled assignment copy-constructor.
    ~PrefPaneEmojis() override = default; ///< Destructor.
    PrefPaneEmojis &operator=(PrefPaneEmojis const &) = delete; ///< Disabled assignment operator.
    PrefPaneEmojis &operator=(PrefPaneEmojis &&) = delete; ///< Disabled move assignment operator.
    void load() const override; ///< Load the content of the pane.

private slots:
    void updateGui() const; ///< update the GUI.
    void onCheckEnableEmojiShortcodes(bool checked) const; ///< Slot for the 'Enable emoji shortcodes' checkbox.
    void onEditEmojiExcludedApplications(); ///< Slot for the pane's 'Excluded application button'.
    void onEmojiLeftDelimiterChanged(QString const &value) const; ///< Slot for the change of the value for the emoji left delimiter.
    void onEmojiRightDelimiterChanged(QString const &value) const; ///< Slot for the change of the value for the emoji right delimiter.
    void onCheckShowEmojisInPickerWindow(bool checked) const; ///< Slot for the 'Show emojis in picker window' checkbox.

private: // member functions
    void changeEvent(QEvent *event) override; ///< Change event handler.

private: // data members
    Ui::PrefPaneEmojis ui_ {}; ///< The GUI for the pane.
    PreferencesManager &prefs_; ///< The preferences manager.
};


#endif // #ifndef BEEFTEXT_PREFS_PANE_EMOJIS_H
