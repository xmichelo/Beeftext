/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list widget
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboManager.h"
#include "ComboGroupListWidget.h"
#include <XMiLib/Exception.h>


//**********************************************************************************************************************
/// \param[in] parent The parent of the widget
//**********************************************************************************************************************
ComboGroupListWidget::ComboGroupListWidget(QWidget* parent)
   : QWidget(nullptr)
{
   ui_.setupUi(this);
   ui_.listGroup->setModel(&ComboManager::instance().comboListRef().groupListRef());
   this->setupGroupsMenu();
   QItemSelectionModel* selectionModel = ui_.listGroup->selectionModel();
   if (!selectionModel)
      throw xmilib::Exception("The Combo group list selection model is null");
   connect(selectionModel, &QItemSelectionModel::currentChanged, this, &ComboGroupListWidget::onCurrentChanged);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupListWidget::setupGroupsMenu()
{
   QMenu* menu = new QMenu(this);
   menu->addAction(ui_.actionNewGroup);
   menu->addSeparator();
   menu->addAction(ui_.actionDeleteGroup);
   ui_.buttonGroups->setMenu(menu);
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void ComboGroupListWidget::changeEvent(QEvent *event)
{
   if (QEvent::LanguageChange == event->type())
      ui_.retranslateUi(this);
   QWidget::changeEvent(event);
}


//**********************************************************************************************************************
/// \return The index of the selected group
/// \return -1 if no group is selected
//**********************************************************************************************************************
qint32 ComboGroupListWidget::getSelectedGroupIndex() const
{
   QModelIndex const index = ui_.listGroup->currentIndex();
   return index.isValid() ? index.row() : -1;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupListWidget::onActionNewGroup()
{
   ComboManager& comboManager = ComboManager::instance();
   ComboGroupList& groups = comboManager.groupListRef();
   SPComboGroup group = ComboGroup::create("My Group", "A test group");
   groups.append(group);
   ComboManager::instance().saveComboListToFile();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupListWidget::onActionDeleteGroup()
{
   ComboGroupList& groups = ComboManager::instance().groupListRef();
   qint32 const index = this->getSelectedGroupIndex();
   if ((index < 0) || (index >= groups.size()))
      return;
   groups.erase(index);
   ComboManager::instance().saveComboListToFile();
}


//**********************************************************************************************************************
/// \param[in] current The new current item
/// \param[in] previous The previous current item
//**********************************************************************************************************************
void ComboGroupListWidget::onCurrentChanged(QModelIndex const& current, QModelIndex const& previous)
{
   qint32 const row = current.row();
   ComboGroupList& groups = ComboManager::instance().groupListRef();
   emit selectedGroupChanged(((row < 0) || (row >= groups.size())) ? nullptr : groups[row]);
}


