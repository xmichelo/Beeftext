/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo table frame
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_COMBO_TABLE_FRAME_H
#define BEEFTEXT_COMBO_TABLE_FRAME_H


#include "ui_ComboTableWidget.h"
#include "ComboSortFilterProxyModel.h"
#include "Combo.h"
#include "Group/GroupListWidget.h"
#include "Group/Group.h"
#include <set>
#include <memory>


//****************************************************************************************************************************************************
/// \brief A class for the frame containing the combo table and associated controls
//****************************************************************************************************************************************************
class ComboTableWidget : public QWidget {
Q_OBJECT
public: // member functions
    explicit ComboTableWidget(QWidget *parent = nullptr); ///< Default constructor
    ComboTableWidget(ComboTableWidget const &) = delete; ///< Disabled copy constructor
    ComboTableWidget(ComboTableWidget &&) = delete; ///< Disabled move constructor
    ~ComboTableWidget() override = default; ///< Default destructor
    ComboTableWidget &operator=(ComboTableWidget const &) = delete; ///< Disabled assignment operator
    ComboTableWidget &operator=(ComboTableWidget &&) = delete; ///< Disabled move assignment operator
    void setGroupListWidget(GroupListWidget *groupListWidget); ///< Set the group list widget associated with this combo
    void runComboImportDialog(QString const &filePath = QString()); ///< Run the combo import dialog
    QMenu *menu(QWidget *parent) const; ///< Get the menu
    static QString menuTitle(); ///< Return the localized title of the menu
    void selectCombo(SpCombo const &combo) const; ///< Select a given combo

public slots:
    void onSelectedGroupChanged(SpGroup const &group); ///< Slot for the changing of the selected group
    void onActionNewCombo(); ///< Slot for the 'Add Combo' action

protected: // member functions
    bool eventFilter(QObject *object, QEvent *event) override; ///< Event filter to override the default behavior of double-click in the table view

private: // member functions
    void setupActions(); ///< Setup the actions.
    void setupTable(); ///< Setup the combo table
    void setupKeyboardShortcuts(); ///< Setup the keyboard shortcuts
    void setupCombosMenu(); ///< Setup the 'Combos' menu
    void setupContextMenu(); ///< Setup the context menu
    qint32 selectedComboCount() const; ///< Get the number of selected combo in the combo table
    SpCombo getSelectedCombo() const; ///< Get the first selected combo
    QList<qint32> getSelectedComboIndexes() const; ///< Retrieve the list indexes of the selected combos
    QList<SpCombo> getSelectedCombos() const; ///< Retrieve the list of the selected combos
    void changeEvent(QEvent *event) override; ///< Change event handler
    void resizeColumnsToContents() const; ///< Resize the columns to fit the content
    std::set<SpGroup> groupsOfSelectedCombos() const; ///< Return a set containing the groups of the selected combos

private slots:
    void updateGui() const; ///< Update the GUI state
    void onActionStartSearch() const; ///< Slot for the start of the search
    void onActionStartSearchInAllGroups() const; ///< Slot for the start of the search in all groups
    void onActionClearSearch() const; ///< Slot for the clearing of the search box
    void onActionDuplicateCombo(); ///< Slot for the 'Duplicate Combo' action
    void onActionDeleteCombo(); ///< Slot for the 'Delete Combo' action
    void onActionEditCombo(); ///< Slot for the 'Edit Combo' action
    void onActionCopySnippet(); ///< Slot for the 'Copy Snippet' action
    void onActionSelectAll(); ///< Slot for the 'Select All' action
    void onActionDeselectAll(); ///< Slot for the 'Deselect All' action
    void onActionEnableDisableCombo(); ///< Slot for the 'Enable/Disable' action
    void onActionExportCombo(); ///< Slot for the 'Export Combo' action
    void onActionExportAllCombos(); ///< Slot for the 'Export All Combos' action
    void onActionImportCombos(); ///< Slot for the 'Import Combos' action
    void onSearchFilterChanged(QString const &text); ///< Slot for the changing of the search field
    void onContextMenuRequested() const; ///< Slot for the combo table context menu
    void onDoubleClick(); ///< Slot for the double clicking in the table view
    void onComboChangedGroup(); ///< Slot for when some combos groups have been changed
    void onContextMenuAboutToShow() const; ///< Slot called when a context menu is about to be shown
    void onMoveToGroupMenuAboutToShow() const; ///< Slot called when a combo menu is about to show
    void onMoveToGroupMenuTriggered(QAction const *action); ///< slot for the triggering of a action in the 'move' menu

private: // data members
    Ui::ComboTableWidget ui_; ///< The GUI for the frame
    QAction *actionCopySnippet_ { nullptr }; ///< The 'Copy Snippet' action.
    QAction *actionDeleteCombo_ { nullptr }; ///< The 'Delete Combo' action.
    QAction *actionDeselectAll_ { nullptr }; ///< The 'Deselect All' action.
    QAction *actionDuplicateCombo_ { nullptr }; ///< The 'Duplicate Combo' action.
    QAction *actionEditCombo_ { nullptr }; ///< The 'Edit Combo' action.
    QAction *actionEnableDisableCombo_ { nullptr }; ///< The 'Enable/Disable Combo' action.
    QAction *actionNewCombo_ { nullptr }; ///< The 'New Combo' action.
    QAction *actionSelectAll_ { nullptr }; ///< The 'Select All' action.
    QAction *actionExportAllCombos_ { nullptr }; ///< The 'Export All Combos' action.
    QAction *actionExportCombo_ { nullptr }; ///< The 'Export Combo' action.
    QAction *actionImportCombos_ { nullptr }; ///< The 'Import Combo' action.

    ComboSortFilterProxyModel proxyModel_; ///< The proxy model for sorting/filtering the combo table
    QMenu *contextMenu_; ///< The context menu for the combo table
    GroupListWidget *groupListWidget_; ///< The group list widget associated with this combo table
};


#endif // #ifndef BEEFTEXT_COMBO_TABLE_FRAME_H
