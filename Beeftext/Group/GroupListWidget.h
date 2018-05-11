/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list widget
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__GROUP__LIST__WIDGET__H
#define BEEFTEXT__COMBO__GROUP__LIST__WIDGET__H


#include "ui_GroupListWidget.h"
#include "Group.h"


//**********************************************************************************************************************
/// \brief ComboGroup list widget class
//**********************************************************************************************************************
class GroupListWidget: public QWidget
{
   Q_OBJECT
public: // member functions
   GroupListWidget(QWidget* parent = nullptr); ///< Default constructor
   GroupListWidget(GroupListWidget const&) = delete; ///< Disabled copy-constructor
   GroupListWidget(GroupListWidget&&) = delete; ///< Disabled assignment copy-constructor
   ~GroupListWidget() = default; ///< Destructor
   GroupListWidget& operator=(GroupListWidget const&) = delete; ///< Disabled assignment operator
   GroupListWidget& operator=(GroupListWidget&&) = delete; ///< Disabled move assignment operator
   SPGroup selectedGroup() const; ///< Get the selected group
   SPGroup selectedOrFirstGroup() const; ///< Get the selected group, or the first group if no group is selected
   void selectGroup(SPGroup const& group); ///< Select a group
   void selectAllCombosEntry() const; ///< select the special "All combos entry" in the list
   QMenu* menu(QWidget* parent) const; ///< Get the menu for the widget

signals: 
   void selectedGroupChanged(SPGroup const& group); /// Signal emitted when the selected group changes

private: // member functions
   void updateGui(); ///< update the GUI state
   void setupGroupsMenu(); ///< Setup the 'groups' menu
   void setupContextMenu(); ///< Setup the context menu
   void changeEvent(QEvent *event) override; ///< Change event handler
   qint32 selectedGroupIndex() const; ///< Retrieve the index of the selected combo
   bool eventFilter(QObject *object, QEvent *event) override; ///< Event filter to override the default behavior of double-click in the table view

private slots: 
   void onActionNewGroup(); ///< Slot for the 'new group' action
   void onActionEditGroup(); ///< Slot for the 'edit group' action
   void onActionDeleteGroup(); ///< Slot for the 'delete group' action
   void onSelectionChanged(QItemSelection const&, QItemSelection const&); ///< Slot for selection change
   void onGroupMoved(SPGroup group, qint32 newIndex); ///< Slot for the moving of a group in the list
   void onContextMenuRequested(); ///< Slot for the context menu
   void onBackupRestored(); ///< Slot triggered when a backup is restored

private: // data members
   Ui::GroupListWidget ui_; ///< The GUI for the dialog
   QMenu* contextMenu_; ///< The context menu for the list view
};


#endif // #ifndef BEEFTEXT__COMBO__GROUP__LIST__WIDGET__H
