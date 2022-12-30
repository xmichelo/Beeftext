/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo import dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_COMBO_IMPORT_DIALOG
#define BEEFTEXT_COMBO_IMPORT_DIALOG


#include "ui_ComboImportDialog.h"
#include "ComboList.h"
#include "Group/Group.h"


//****************************************************************************************************************************************************
/// \brief Combo import dialog class
//****************************************************************************************************************************************************
class ComboImportDialog : public QDialog {
Q_OBJECT
public: // member functions
    explicit ComboImportDialog(QString const &filePath = QString(), SpGroup const &group = SpGroup(), QWidget *parent = nullptr); ///< Default constructor
    ComboImportDialog(ComboImportDialog const &) = delete; ///< Disabled copy constructor
    ComboImportDialog(ComboImportDialog &&) = delete; ///< Disabled move constructor
    ~ComboImportDialog() override = default; ///< Default destructor
    ComboImportDialog &operator=(ComboImportDialog const &) = delete; ///< Disabled assignment operator
    ComboImportDialog &operator=(ComboImportDialog &&) = delete; ///< Disabled move assignment operator

protected: // member functions
    /// \name Drag and drop functions
    ///\{
    void dragEnterEvent(QDragEnterEvent *event) override; ///< Drag enter event handler
    void dragMoveEvent(QDragMoveEvent *event) override; ///< Drag move event handler
    void dragLeaveEvent(QDragLeaveEvent *event) override; ///< Drag leave event handler
    void dropEvent(QDropEvent *event) override; ///< Drop event handler
    ///\}

private: // data members
    void updateGui() const; ///< Update the GUI state
    qint32 computeTotalImportCount() const; ///< Return the number of combos to be imported according to the user selection in the dialog
    void performFinalImport(qint32 &outFailureCount); ///< Perform the final import of combos

private slots:
    void onImport(); ///< Slot for the 'Import' action
    void onCancel(); ///< Slot for the 'Cancel' action
    void onBrowse(); ///< Slot for the 'Browse' action
    void onEditPathTextChanged(QString const &text); ///< Slot for the changing of the content of the path edit control
    void onConflictRadioToggled(bool state) const; ///< Slot for the toggling of a conflict resolution radio button

private: // data members
    Ui::ComboImportDialog ui_; ///< The GUI for the dialog
    QString currentError_; ///< The current error message being displayed.
    ComboList importableCombos_; ///< The list of combos that can be imported as is
    ComboList conflictingNewerCombos_; ///< The list of conflicting combos with a newer modification date than the current one
    ComboList conflictingOlderCombos_; ///< The list of conflicting combos with an older modification date than the current one
};


#endif // #ifndef BEEFTEXT_COMBO_IMPORT_DIALOG