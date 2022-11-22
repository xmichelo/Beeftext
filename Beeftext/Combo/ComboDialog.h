/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of dialog for creating/editing a combo
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  

#ifndef BEEFTEXT_COMBO_DIALOG_H
#define BEEFTEXT_COMBO_DIALOG_H


#include "ui_ComboDialog.h"
#include "ComboKeywordValidator.h"
#include "Combo/Combo.h"


//****************************************************************************************************************************************************
/// \brief A dialog class for creating and editing a combo
//****************************************************************************************************************************************************
class ComboDialog : public QDialog {
Q_OBJECT
public:
    static bool run(SpCombo const &combo, QString const &title = QString(), QWidget *parent = nullptr);

public: // member functions
    explicit ComboDialog(SpCombo const &combo, QString const &title = QString(), QWidget *parent = nullptr); ///< Default constructor
    ComboDialog(ComboDialog const &) = delete; ///< Disabled copy constructor
    ComboDialog(ComboDialog &&) = delete; ///< Disabled move constructor
    ~ComboDialog() override = default; ///< Default destructor
    ComboDialog &operator=(ComboDialog const &) = delete; ///< Disabled assignment operator
    ComboDialog &operator=(ComboDialog &&) = delete; ///< Disabled move assignment operator

private: // member functions
    bool checkAndReportInvalidCombo(); ///< Check the keyword against existing combos and report conflicts

private slots:
    void onActionOk(); ///< Slot for the 'OK' action
    void onActionNewGroup(); ///< Slot for the 'New Group' action
    void updateGui() const; ///< Update the GUI state

private: // data members
    Ui::ComboDialog ui_ {}; ///< The GUI for the dialog
    SpCombo combo_ { nullptr }; ///< The combo
    ComboKeywordValidator validator_; ///< The validator for the keyword
};


#endif // #ifndef BEEFTEXT_COMBO_DIALOG_H