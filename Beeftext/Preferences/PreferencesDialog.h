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


//****************************************************************************************************************************************************
/// \brief Preferences dialog class.
//****************************************************************************************************************************************************
class PreferencesDialog : public QDialog {
Q_OBJECT
public: // member functions
    explicit PreferencesDialog(QWidget *parent = nullptr); ///< Default constructor.
    PreferencesDialog(PreferencesDialog const &) = delete; ///< Disabled copy-constructor.
    PreferencesDialog(PreferencesDialog &&) = delete; ///< Disabled assignment copy-constructor.
    ~PreferencesDialog() override = default; ///< Destructor.
    PreferencesDialog &operator=(PreferencesDialog const &) = delete; ///< Disabled assignment operator.
    PreferencesDialog &operator=(PreferencesDialog &&) = delete; ///< Disabled move assignment operator.
    void load() const; ///< Load the preferences.

private slots:
    void onResetToDefaultValues(); ///< Slot for the 'Reset to default values' action.
    void onResetWarnings(); ///< Slot for the 'Reset warnings' action.
    void onClose(); ///< Slot for the OK button.
    void onExport(); ///< Slot for the 'Export' button.
    void onImport(); ///< Slot for the 'Import' button.

private: // member functions
    bool validateInput(); ///< Check that the values for the dialog are consistent.
    void changeEvent(QEvent *) override; ///< Change event handler.

private: // data members
    Ui::PreferencesDialog ui_ {}; ///< The GUI for the dialog.
    PreferencesManager &prefs_; ///< The preferences manager.
    QString previousComboListPath_; ///< The previous path of the combo list.
    QList<PrefPane *> panes_; ///< The preference panes.
};


#endif // #ifndef BEEFTEXT_PREFERENCES_DIALOG_H
