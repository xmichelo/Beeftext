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
#include <XMiLib/Exception.h>


//**********************************************************************************************************************
/// \param[in] first The first combo group
/// \param[in] second The second combo group
//**********************************************************************************************************************
void swap(ComboGroupList& first, ComboGroupList& second)
{
   first.groups_.swap(second.groups_);
}


//**********************************************************************************************************************
/// \param[in] parent The parent object of the instance
//**********************************************************************************************************************
ComboGroupList::ComboGroupList(QObject* parent)
   : QAbstractListModel(parent)
{

}


//**********************************************************************************************************************
/// \param[in] ref The reference group to copy
//**********************************************************************************************************************
ComboGroupList::ComboGroupList(ComboGroupList const& ref)
   : groups_(ref.groups_)
{

}


//**********************************************************************************************************************
/// \param[in] ref The reference group to copy
//**********************************************************************************************************************
ComboGroupList::ComboGroupList(ComboGroupList&& ref)
   : groups_(std::move(ref.groups_))
{

}


//**********************************************************************************************************************
/// \param[in] ref The reference group to copy
/// \return A reference to the instance
//**********************************************************************************************************************
ComboGroupList& ComboGroupList::operator=(ComboGroupList const& ref)
{
   if (&ref != this)
      groups_ = ref.groups_;
   return *this;
}


//**********************************************************************************************************************
/// \param[in] ref The reference group to copy
/// \return A reference to the instance
//**********************************************************************************************************************
ComboGroupList& ComboGroupList::operator=(ComboGroupList&& ref)
{
   if (&ref != this)
      groups_ = std::move(ref.groups_);
   return *this;
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


//**********************************************************************************************************************
/// \return A JSON array containing the combo list
//**********************************************************************************************************************
QJsonArray ComboGroupList::toJsonArray() const
{
   QJsonArray result;
   for (SPComboGroup const& group: groups_)
      result.append(group->toJsonObject());
   return result;
}


//**********************************************************************************************************************
/// \param[in] array The JSON array to parse
/// \param[in] formatVersion The JSON file format version
/// \param[out] outErrorMessage if not null and the function returns false, this variable hold a description of the
/// error on exit
/// \return true if and only if the array was parsed successfully
//**********************************************************************************************************************
bool ComboGroupList::readFromJsonArray(QJsonArray const& array, qint32 formatVersion, QString* outErrorMessage)
{
   try
   {
      this->clear();
      for (QJsonValue const& value : array)
      {
         if (!value.isObject())
            throw xmilib::Exception("The combo group list is invalid.");
         SPComboGroup group = ComboGroup::create(value.toObject(), formatVersion);
         if ((!group) || (!group->isValid()))
            throw xmilib::Exception("The combo group list contains an invalid group.");
         if (!this->append(group))
            throw xmilib::Exception("Could not append one of the combo groups to the group list.");
      }
      return true;
   }
   catch (xmilib::Exception const& e)
   {
      if (outErrorMessage)
         *outErrorMessage = e.qwhat();
      return false;
   }
}


//**********************************************************************************************************************
/// \param[in] parent The parent model index
//**********************************************************************************************************************
int ComboGroupList::rowCount(const QModelIndex &parent) const
{
   return groups_.size() + 1;
}


//**********************************************************************************************************************
/// \param[in] index The model index
/// \param[in] role The role
/// \return the model data for the given role at the given index
//**********************************************************************************************************************
QVariant ComboGroupList::data(QModelIndex const& index, int role) const
{
   qint32 const groupCount = groups_.size();
   qint32 const row = index.row();
   if ((row < 0) || (row >= groupCount + 2))
      return QVariant();
   if ((Qt::DisplayRole != role) && (Qt::ToolTipRole != role))
      return QVariant();
   bool const toolTipRole = (Qt::ToolTipRole == role);
   if (groupCount == row) 
      return toolTipRole ? tr("The default group."): tr("Default Group");
   SPComboGroup group = groups_[row];
   return group ? (toolTipRole ? group->name() : group->description()): QString();
}


