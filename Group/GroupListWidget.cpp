/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list widget
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "GroupDialog.h"
#include "GroupListWidget.h"
#include "Combo/ComboManager.h"
#include <XMiLib/Exception.h>


//**********************************************************************************************************************
/// \param[in] parent The parent of the widget
//**********************************************************************************************************************
GroupListWidget::GroupListWidget(QWidget* parent)
   : QWidget(nullptr)
{
   ui_.setupUi(this);
   GroupList& groups = ComboManager::instance().comboListRef().groupListRef();
   ui_.listGroup->setModel(&groups);
   this->setupGroupsMenu();
   QItemSelectionModel* selectionModel = ui_.listGroup->selectionModel();
   if (!selectionModel)
      throw xmilib::Exception("The group list selection model is null");
   connect(selectionModel, &QItemSelectionModel::currentChanged, this, &GroupListWidget::onCurrentChanged);
   this->updateGui();
   QTimer::singleShot(0, [&]() {ui_.listGroup->setCurrentIndex(groups.index(groups.rowCount() - 1)); }); // delayed to be sure the signal/slot mechanism work
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void GroupListWidget::updateGui()
{
   qint32 const groupCount = ComboManager::instance().groupListRef().size();
   qint32 const index = this->selectedGroupIndex();
   ui_.actionDeleteGroup->setEnabled(groupCount > 1);
   ui_.actionEditGroup->setEnabled((index >= 0) && (index < groupCount));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void GroupListWidget::setupGroupsMenu()
{
   QMenu* menu = new QMenu(this);
   menu->addAction(ui_.actionNewGroup);
   menu->addAction(ui_.actionEditGroup);
   menu->addSeparator();
   menu->addAction(ui_.actionDeleteGroup);
   ui_.buttonGroups->setMenu(menu);
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void GroupListWidget::changeEvent(QEvent *event)
{
   if (QEvent::LanguageChange == event->type())
      ui_.retranslateUi(this);
   QWidget::changeEvent(event);
}


//**********************************************************************************************************************
/// \return The index of the selected group
/// \return -1 if no group is selected
//**********************************************************************************************************************
qint32 GroupListWidget::selectedGroupIndex() const
{
   QModelIndex const index = ui_.listGroup->currentIndex();
   return index.isValid() ? index.row() : -1;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void GroupListWidget::onActionNewGroup()
{
   try
   {
      SPGroup group = Group::create(QString());
      if (!GroupDialog::run(group, tr("New Group")))
         return;
      ComboManager& comboManager = ComboManager::instance();
      GroupList& groups = comboManager.groupListRef();
      if (!groups.append(group))
         throw xmilib::Exception(tr("The group could not be added to the list."));
      QString errorMessage;
      if (!comboManager.saveComboListToFile(&errorMessage))
         throw xmilib::Exception(errorMessage);
   }
   catch (xmilib::Exception const& e)
   {
      QMessageBox::critical(this, tr("Error"), e.qwhat());
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void GroupListWidget::onActionEditGroup()
{
   try
   {
      ComboManager& comboManager = ComboManager::instance();
      GroupList& groups = comboManager.groupListRef();
      quint32 const index = this->selectedGroupIndex();
      if ((index < 0) || (index >= groups.size()))
         return;
      SPGroup group = groups[index];
      if (!GroupDialog::run(group, tr("Edit Group")))
         return;
      QString errorMessage;
      if (!comboManager.saveComboListToFile(&errorMessage))
         throw xmilib::Exception(errorMessage);
      this->updateGui();
   }
   catch (xmilib::Exception const& e)
   {
      QMessageBox::critical(this, tr("Error"), e.qwhat());
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void GroupListWidget::onActionDeleteGroup()
{
   try
   {
      ComboManager& comboManager = ComboManager::instance();
      GroupList& groups = comboManager.groupListRef();
      qint32 const index = this->selectedGroupIndex();
      if ((groups.size() <= 1) ||(index < 0) || (index >= groups.size()))
         return;
      comboManager.comboListRef().eraseCombosOfGroup(groups[index]);
      groups.erase(index);
      comboManager.saveComboListToFile();
      this->updateGui();
   }
   catch (xmilib::Exception const& e)
   {
      QMessageBox::critical(this, tr("Error"), e.qwhat());
   }
}


//**********************************************************************************************************************
/// \param[in] current The new current item
/// \param[in] previous The previous current item
//**********************************************************************************************************************
void GroupListWidget::onCurrentChanged(QModelIndex const& current, QModelIndex const& previous)
{
   qint32 const row = current.row();
   GroupList& groups = ComboManager::instance().groupListRef();
   this->updateGui();
   emit selectedGroupChanged(((row < 0) || (row >= groups.size())) ? nullptr : groups[row]);
}



