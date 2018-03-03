/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo group list class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboGroupList.h"
#include "BeeftextGlobals.h"


//**********************************************************************************************************************
/// \param[in] parent The parent object of the instance
//**********************************************************************************************************************
ComboGroupList::ComboGroupList(QObject* parent)
   : QAbstractTableModel(parent)
{

}


//**********************************************************************************************************************
/// \return The number of groups in the list
//**********************************************************************************************************************
qint32 ComboGroupList::size() const
{
   return groups_.size();
}


//**********************************************************************************************************************
/// \return true if and only if the list is empty
//**********************************************************************************************************************
bool ComboGroupList::isEmpty() const
{
   return groups_.empty();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupList::clear()
{
   groups_.clear();
}


//**********************************************************************************************************************
/// return true if and only if a group with this UUID is already in the list
//**********************************************************************************************************************
bool ComboGroupList::contains(SPComboGroup const& group) const
{
   return group ? this->end() != this->findByUuid(group->uuid()) : false;
}


//**********************************************************************************************************************
/// \return true if the group is valid and not already in the list
//**********************************************************************************************************************
bool ComboGroupList::canGroupBeAdded(SPComboGroup const& group) const
{
   return group && group->isValid() && !this->contains(group);
}


//**********************************************************************************************************************
/// \param[in] group The group to add
/// \return true if the group has been successfully added
//**********************************************************************************************************************
bool ComboGroupList::append(SPComboGroup const& group)
{
   if (!canGroupBeAdded(group))
   {
      globals::debugLog().addError("Cannot add group (null or duplicate).");
      return false;
   }
   this->beginInsertRows(QModelIndex(), groups_.size(), groups_.size());
   groups_.push_back(group);
   this->endInsertRows();
   return true;
}


//**********************************************************************************************************************
/// \param[in] group The group to add
//**********************************************************************************************************************
void ComboGroupList::push_back(SPComboGroup const& group)
{
   this->beginInsertRows(QModelIndex(), groups_.size(), groups_.size());
   groups_.push_back(group);
   this->endInsertRows();
}


//**********************************************************************************************************************
/// \param[in] index the index of the group to erase
//**********************************************************************************************************************
void ComboGroupList::erase(qint32 index)
{
   this->beginRemoveRows(QModelIndex(), index, index);
   groups_.erase(groups_.begin() + index);
   this->endRemoveRows();
}


//**********************************************************************************************************************
/// \param[in] uuid The UUID of the combo to find
/// \return An iterator to the found group or end() if not found
//**********************************************************************************************************************
ComboGroupList::iterator ComboGroupList::findByUuid(QUuid const& uuid)
{
   return std::find_if(this->begin(), this->end(), [&](SPComboGroup const& group) -> bool
      { return group && uuid == group->uuid(); });
}


//**********************************************************************************************************************
/// \param[in] uuid The UUID of the combo to find
/// \return A constant iterator to the found group or end() if not found
//**********************************************************************************************************************
ComboGroupList::const_iterator ComboGroupList::findByUuid(QUuid const& uuid) const
{
   return std::find_if(this->begin(), this->end(), [&](SPComboGroup const& group) -> bool
   { return group && uuid == group->uuid(); });
}


//**********************************************************************************************************************
/// \return An iterator to the beginning of the list
//**********************************************************************************************************************
ComboGroupList::iterator ComboGroupList::begin()
{
   return groups_.begin();
}


//**********************************************************************************************************************
/// \return A constant iterator to the beginning of the list
//**********************************************************************************************************************
ComboGroupList::const_iterator ComboGroupList::begin() const
{
   return groups_.begin();
}


//**********************************************************************************************************************
/// \return An iterator to the end of the list
//**********************************************************************************************************************
ComboGroupList::iterator ComboGroupList::end()
{
   return groups_.end();
}


//**********************************************************************************************************************
/// \return A constant iterator to the end of the list
//**********************************************************************************************************************
ComboGroupList::const_iterator ComboGroupList::end() const
{
   return groups_.end();
}


//**********************************************************************************************************************
/// \return An iterator to the reverse beginning of the list
//**********************************************************************************************************************
ComboGroupList::reverse_iterator ComboGroupList::rbegin()
{
   return groups_.rbegin();
}


//**********************************************************************************************************************
/// \return A constant iterator to the reverse beginning of the list
//**********************************************************************************************************************
ComboGroupList::const_reverse_iterator ComboGroupList::rbegin() const
{
   return groups_.rbegin();
}


//**********************************************************************************************************************
/// \return An iterator to the reverse end of the list
//**********************************************************************************************************************
ComboGroupList::reverse_iterator ComboGroupList::rend()
{
   return groups_.rend();
}


//**********************************************************************************************************************
/// \return A constant iterator to the reverse end of the list
//**********************************************************************************************************************
ComboGroupList::const_reverse_iterator ComboGroupList::rend() const
{
   return groups_.rend();
}


