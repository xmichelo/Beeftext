/// \file
/// \author 
///
/// \brief Declaration of the appearance preference pane.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PREFS_PANE_APPEARANCE_H
#define BEEFTEXT_PREFS_PANE_APPEARANCE_H


#include "PrefPane.h"
#include "ui_PrefPaneAppearance.h"
#include "Preferences/PreferencesManager.h"


//****************************************************************************************************************************************************
/// \brief The appearance preferences pane.
//****************************************************************************************************************************************************
class PrefPaneAppearance : public PrefPane {
Q_OBJECT
public: // member functions
    explicit PrefPaneAppearance(QWidget *parent = nullptr); ///< Default constructor.
    PrefPaneAppearance(PrefPaneAppearance const &) = delete; ///< Disabled copy-constructor.
    PrefPaneAppearance(PrefPaneAppearance &&) = delete; ///< Disabled assignment copy-constructor.
    ~PrefPaneAppearance() override = default; ///< Destructor.
    PrefPaneAppearance &operator=(PrefPaneAppearance const &) = delete; ///< Disabled assignment operator.
    PrefPaneAppearance &operator=(PrefPaneAppearance &&) = delete; ///< Disabled move assignment operator.
    void load() const override; ///< Load the content of the preference pane.

private slots:
    void onRefreshLanguageList() const; ///< Slot for the 'Refresh Language List' button.
    void onComboLanguageValueChanged(int index) const; ///< Slot for the change of the value in the language combo.
    static void onOpenTranslationFolder(); ///< Slot for the 'Translation Folder' button.
    void onCheckUseCustomTheme(bool checked) const; ///< Slot for the 'Use custom theme' checkbox.
    void onComboThemeValueChanged(int) const; ///< slot for the change of the 'Theme' combo.
    void updateGui() const; ///< Update the GUI.

private: // member functions
    void changeEvent(QEvent *event) override; ///< Change event handler

private: // data members
    Ui::PrefPaneAppearance ui_ {}; ///< The GUI for the pane.
    PreferencesManager &prefs_; ///< The preferences manager.
};


#endif // #ifndef BEEFTEXT_PREFS_PANE_APPEARANCE_H
