/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo box class to select a group


#include "stdafx.h"
#include "GroupComboBox.h"



//**********************************************************************************************************************
/// \param[in] parent The parent widget
//**********************************************************************************************************************
GroupComboBox::GroupComboBox(QWidget* parent)
   : QComboBox(parent)
{

}


//**********************************************************************************************************************
/// \param[in] groups The group list to fill the combo box with
//**********************************************************************************************************************
void GroupComboBox::setContent(GroupList const& groups)
{
   this->clear();
   for (SPGroup const& group : groups)
      if (group)
         this->addItem(group->name(), QVariant::fromValue<SPGroup>(group));
   this->addItem(tr("<Default group>"), QVariant::fromValue<SPGroup>(SPGroup()));
}


//**********************************************************************************************************************
/// \param[in] group The group to select
//**********************************************************************************************************************
void GroupComboBox::setCurrentGroup(SPGroup const& group)
{
   qint32 const index = this->findData(QVariant::fromValue<SPGroup>(group));
   qint32 const itemCount = this->count();
   if ((index >= 0) || (index < itemCount))
      this->setCurrentIndex(index);
   else
      this->setCurrentIndex(itemCount - 1);
}


//**********************************************************************************************************************
/// \return The currently selected group
//**********************************************************************************************************************
SPGroup GroupComboBox::currentGroup()
{
   QVariant const data = this->currentData();
   return data.canConvert<SPGroup>() ? data.value<SPGroup>(): SPGroup();
}


