/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list widget
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__GROUP__LIST__WIDGET__H
#define BEEFTEXT__COMBO__GROUP__LIST__WIDGET__H


#include "ui_ComboGroupListWidget.h"
#include "ComboGroup.h"


//**********************************************************************************************************************
/// \brief ComboGroup list widget class
//**********************************************************************************************************************
class ComboGroupListWidget: public QWidget
{
   Q_OBJECT
public: // member functions
   ComboGroupListWidget(QWidget* parent = nullptr); ///< Default constructor
   ComboGroupListWidget(ComboGroupListWidget const&) = delete; ///< Disabled copy-constructor
   ComboGroupListWidget(ComboGroupListWidget&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboGroupListWidget() = default; ///< Destructor
   ComboGroupListWidget& operator=(ComboGroupListWidget const&) = delete; ///< Disabled assignment operator
   ComboGroupListWidget& operator=(ComboGroupListWidget&&) = delete; ///< Disabled move assignment operator

signals: 
   void selectedGroupChanged(SPComboGroup const& group); /// Signal emitted when the selected group changes

private: // member functions
   void setupGroupsMenu(); ///< setup the 'groups' menu
   void changeEvent(QEvent *event) override; ///< Change event handler
   qint32 getSelectedGroupIndex() const; ///< Retrieve the index of the selected combo

private slots: 
   void onActionNewGroup(); ///< Slot for the 'new group' action
   void onActionDeleteGroup(); ///< Slot for the 'delete group' action
   void onCurrentChanged(QModelIndex const& current, QModelIndex const& previous);
private: // data members
   Ui::ComboGroupListWidget ui_; ///< The GUI for the dialog
};


#endif // #ifndef BEEFTEXT__COMBO__GROUP__LIST__WIDGET__H