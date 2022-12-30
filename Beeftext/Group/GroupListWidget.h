/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list widget
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_COMBO_GROUP_LIST_WIDGET_H
#define BEEFTEXT_COMBO_GROUP_LIST_WIDGET_H


#include "ui_GroupListWidget.h"
#include "Group.h"


//****************************************************************************************************************************************************
/// \brief ComboGroup list widget class
//****************************************************************************************************************************************************
class GroupListWidget : public QWidget {
Q_OBJECT
public: // member functions
    [[maybe_unused]] explicit GroupListWidget(QWidget *parent = nullptr); ///< Default constructor
    GroupListWidget(GroupListWidget const &) = delete; ///< Disabled copy-constructor
    GroupListWidget(GroupListWidget &&) = delete; ///< Disabled assignment copy-constructor
    ~GroupListWidget() override = default; ///< Destructor
    GroupListWidget &operator=(GroupListWidget const &) = delete; ///< Disabled assignment operator
    GroupListWidget &operator=(GroupListWidget &&) = delete; ///< Disabled move assignment operator
    SpGroup selectedGroup() const; ///< Get the selected group
    SpGroup selectedOrFirstGroup() const; ///< Get the selected group, or the first group if no group is selected
    void selectGroup(SpGroup const &group) const; ///< Select a group
    QMenu *menu(QWidget *parent) const; ///< Get the menu for the widget
    static QString menuTitle(); ///< Return the translated menu title

public slots:
    void onActionNewGroup(); ///< Slot for the 'new group' action
    void selectAllCombosEntry() const; ///< select the special "All combos entry" in the list

signals:
    void selectedGroupChanged(SpGroup const &group); /// Signal emitted when the selected group changes

private: // member functions
    void setupActions(); ///< Setup the actions.
    void updateGui() const; ///< update the GUI state
    void setupGroupsMenu(); ///< Setup the 'groups' menu
    void setupContextMenu(); ///< Setup the context menu
    void changeEvent(QEvent *event) override; ///< Change event handler
    qint32 selectedGroupIndex() const; ///< Retrieve the index of the selected combo
    bool eventFilter(QObject *object, QEvent *event) override; ///< Event filter to override the default behavior of double-click in the table view
    void updateActionsAvailabilityAndNames() const; ///< Update the content of a group menu.

private slots:
    void onActionEditGroup(); ///< Slot for the 'edit group' action
    void onActionDeleteGroup(); ///< Slot for the 'delete group' action
    void onActionEnableDisableGroup(); ///< Slot for the 'Enable/Disable' action.
    void onSelectionChanged(QItemSelection const &, QItemSelection const &); ///< Slot for selection change
    void onGroupMoved(SpGroup const &group, qint32 newIndex) const; ///< Slot for the moving of a group in the list
    void onContextMenuRequested() const; ///< Slot for the context menu
    void onMenuAboutToShow() const; ///< Slot triggered when the menu is about to be displayed
    void onBackupRestored() const; ///< Slot triggered when a backup is restored

private: // data members
    Ui::GroupListWidget ui_; ///< The GUI for the dialog
    QAction* actionNewGroup_ { nullptr }; ///< The 'New Group' action.
    QAction* actionDeleteGroup_ { nullptr }; ///< The 'Delete Group' action.
    QAction* actionEditGroup_ { nullptr }; ///< The 'Edit Group' action.
    QAction* actionEnableDisableGroup_ { nullptr }; ///< The 'Enable/Disable Group' action.
    QMenu *contextMenu_; ///< The context menu for the list view
};


#endif // #ifndef BEEFTEXT_COMBO_GROUP_LIST_WIDGET_H
