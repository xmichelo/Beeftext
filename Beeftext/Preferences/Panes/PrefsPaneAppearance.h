/// \file
/// \author 
///
/// \brief Declaration of the appearance preference pane.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PREFS_PANE_APPEARANCE_H
#define BEEFTEXT_PREFS_PANE_APPEARANCE_H


#include "PrefsPane.h"
#include "ui_PrefsPaneAppearance.h"
#include "Preferences/PreferencesManager.h"


//**********************************************************************************************************************
/// \brief The appearance preferences pane.
//**********************************************************************************************************************
class PrefsPaneAppearance: public PrefsPane
{
   Q_OBJECT
public: // member functions
   explicit PrefsPaneAppearance(QWidget* parent = nullptr); ///< Default constructor.
   PrefsPaneAppearance(PrefsPaneAppearance const&) = delete; ///< Disabled copy-constructor.
   PrefsPaneAppearance(PrefsPaneAppearance&&) = delete; ///< Disabled assignment copy-constructor.
   ~PrefsPaneAppearance() override = default; ///< Destructor.
   PrefsPaneAppearance& operator=(PrefsPaneAppearance const&) = delete; ///< Disabled assignment operator.
   PrefsPaneAppearance& operator=(PrefsPaneAppearance&&) = delete; ///< Disabled move assignment operator.
   void load() const override; ///< Load the content of the preference pane.

private slots:
   void onRefreshLanguageList() const; ///< Slot for the 'Refresh Language List' button.
   void onComboLanguageValueChanged(int index) const; ///< Slot for the change of the value in the language combo.
   static void onOpenTranslationFolder(); ///< Slot for the 'Translation Folder' button.
   void onCheckUseCustomTheme(bool checked) const; ///< Slot for the 'Use custom theme' checkbox.
   void onComboThemeValueChanged(int) const; ///< slot for the change of the 'Theme' combo.
   void updateGui() const; ///< Update the GUI.

private: // member functions
   void changeEvent(QEvent* event) override; ///< Change event handler

private: // data members
   Ui::PrefsPaneAppearance ui_ {}; ///< The GUI for the pane.
   PreferencesManager& prefs_; ///< The preferences manager.
};


#endif // #ifndef BEEFTEXT_PREFS_PANE_APPEARANCE_H
