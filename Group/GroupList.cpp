/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo group list class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "GroupList.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {
   QString kDefaultGroupName() { return QObject::tr("Default Group"); } ///< The default group name
   QString kDefaultGroupDescription() { return QObject::tr("The default group."); } ///< The default group description
}

//**********************************************************************************************************************
/// \param[in] first The first group
/// \param[in] second The second group
//**********************************************************************************************************************
void swap(GroupList& first, GroupList& second)
{
   first.groups_.swap(second.groups_);
}


//**********************************************************************************************************************
/// \param[in] parent The parent object of the instance
//**********************************************************************************************************************
GroupList::GroupList(QObject* parent)
   : QAbstractListModel(parent)
{

}


//**********************************************************************************************************************
/// \param[in] ref The reference group to copy
//**********************************************************************************************************************
GroupList::GroupList(GroupList const& ref)
   : groups_(ref.groups_)
{

}


//**********************************************************************************************************************
/// \param[in] ref The reference group to copy
//**********************************************************************************************************************
GroupList::GroupList(GroupList&& ref)
   : groups_(std::move(ref.groups_))
{

}


//**********************************************************************************************************************
/// \param[in] ref The reference group to copy
/// \return A reference to the instance
//**********************************************************************************************************************
GroupList& GroupList::operator=(GroupList const& ref)
{
   if (&ref != this)
      groups_ = ref.groups_;
   return *this;
}


//**********************************************************************************************************************
/// \param[in] ref The reference group to copy
/// \return A reference to the instance
//**********************************************************************************************************************
GroupList& GroupList::operator=(GroupList&& ref)
{
   if (&ref != this)
      groups_ = std::move(ref.groups_);
   return *this;
}


//**********************************************************************************************************************
/// \param[in] index The index of the group
/// \return The group a the specified index
//**********************************************************************************************************************
SPGroup& GroupList::operator[](qint32 index)
{
   return groups_[index];
}


//**********************************************************************************************************************
/// \param[in] index The index of the group
/// \return The group a the specified index
//**********************************************************************************************************************
SPGroup const& GroupList::operator[](qint32 index) const
{
   return groups_[index];
}


//**********************************************************************************************************************
/// \return The number of groups in the list
//**********************************************************************************************************************
qint32 GroupList::size() const
{
   return groups_.size();
}


//**********************************************************************************************************************
/// \return true if and only if the list is empty
//**********************************************************************************************************************
bool GroupList::isEmpty() const
{
   return groups_.empty();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void GroupList::clear()
{
   groups_.clear();
}


//**********************************************************************************************************************
/// return true if and only if a group with this UUID is already in the list
//**********************************************************************************************************************
bool GroupList::contains(SPGroup const& group) const
{
   return group ? this->end() != this->findByUuid(group->uuid()) : false;
}


//**********************************************************************************************************************
/// \return true if the group is valid and not already in the list
//**********************************************************************************************************************
bool GroupList::canGroupBeAdded(SPGroup const& group) const
{
   return group && group->isValid() && !this->contains(group);
}


//**********************************************************************************************************************
/// \param[in] group The group to add
/// \return true if the group has been successfully added
//**********************************************************************************************************************
bool GroupList::append(SPGroup const& group)
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
void GroupList::push_back(SPGroup const& group)
{
   this->beginInsertRows(QModelIndex(), groups_.size(), groups_.size());
   groups_.push_back(group);
   this->endInsertRows();
}


//**********************************************************************************************************************
/// \param[in] index the index of the group to erase
//**********************************************************************************************************************
void GroupList::erase(qint32 index)
{
   this->beginRemoveRows(QModelIndex(), index, index);
   groups_.erase(groups_.begin() + index);
   this->endRemoveRows();
}


//**********************************************************************************************************************
/// \param[in] uuid The UUID of the combo to find
/// \return An iterator to the found group or end() if not found
//**********************************************************************************************************************
GroupList::iterator GroupList::findByUuid(QUuid const& uuid)
{
   return std::find_if(this->begin(), this->end(), [&](SPGroup const& group) -> bool
      { return group && uuid == group->uuid(); });
}


//**********************************************************************************************************************
/// \param[in] uuid The UUID of the combo to find
/// \return A constant iterator to the found group or end() if not found
//**********************************************************************************************************************
GroupList::const_iterator GroupList::findByUuid(QUuid const& uuid) const
{
   return std::find_if(this->begin(), this->end(), [&](SPGroup const& group) -> bool
   { return group && uuid == group->uuid(); });
}


//**********************************************************************************************************************
/// \return An iterator to the beginning of the list
//**********************************************************************************************************************
GroupList::iterator GroupList::begin()
{
   return groups_.begin();
}


//**********************************************************************************************************************
/// \return A constant iterator to the beginning of the list
//**********************************************************************************************************************
GroupList::const_iterator GroupList::begin() const
{
   return groups_.begin();
}


//**********************************************************************************************************************
/// \return An iterator to the end of the list
//**********************************************************************************************************************
GroupList::iterator GroupList::end()
{
   return groups_.end();
}


//**********************************************************************************************************************
/// \return A constant iterator to the end of the list
//**********************************************************************************************************************
GroupList::const_iterator GroupList::end() const
{
   return groups_.end();
}


//**********************************************************************************************************************
/// \return An iterator to the reverse beginning of the list
//**********************************************************************************************************************
GroupList::reverse_iterator GroupList::rbegin()
{
   return groups_.rbegin();
}


//**********************************************************************************************************************
/// \return A constant iterator to the reverse beginning of the list
//**********************************************************************************************************************
GroupList::const_reverse_iterator GroupList::rbegin() const
{
   return groups_.rbegin();
}


//**********************************************************************************************************************
/// \return An iterator to the reverse end of the list
//**********************************************************************************************************************
GroupList::reverse_iterator GroupList::rend()
{
   return groups_.rend();
}


//**********************************************************************************************************************
/// \return A constant iterator to the reverse end of the list
//**********************************************************************************************************************
GroupList::const_reverse_iterator GroupList::rend() const
{
   return groups_.rend();
}


//**********************************************************************************************************************
/// \return A JSON array containing the combo list
//**********************************************************************************************************************
QJsonArray GroupList::toJsonArray() const
{
   QJsonArray result;
   for (SPGroup const& group: groups_)
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
bool GroupList::readFromJsonArray(QJsonArray const& array, qint32 formatVersion, QString* outErrorMessage)
{
   try
   {
      this->clear();
      for (QJsonValue const& value : array)
      {
         if (!value.isObject())
            throw xmilib::Exception("The group list is invalid.");
         SPGroup group = Group::create(value.toObject(), formatVersion);
         if ((!group) || (!group->isValid()))
            throw xmilib::Exception("The group list contains an invalid group.");
         if (!this->append(group))
            throw xmilib::Exception("Could not append one of the groups to the group list.");
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
/// \return true if the group list was empty and had to be filled with a default group
//**********************************************************************************************************************
bool GroupList::ensureNotEmpty()
{
   bool const empty = groups_.empty();
   if (empty)
      groups_.push_back(Group::create(kDefaultGroupName(), kDefaultGroupDescription()));
   return empty;
}


//**********************************************************************************************************************
/// \param[in] parent The parent model index
//**********************************************************************************************************************
int GroupList::rowCount(const QModelIndex &parent) const
{
   return groups_.size();
}


//**********************************************************************************************************************
/// \param[in] index The model index
/// \param[in] role The role
/// \return the model data for the given role at the given index
//**********************************************************************************************************************
QVariant GroupList::data(QModelIndex const& index, int role) const
{
   qint32 const groupCount = groups_.size();
   qint32 const row = index.row();
   if ((row < 0) || (row >= groupCount))
      return QVariant();
   if ((Qt::DisplayRole != role) && (Qt::ToolTipRole != role))
      return QVariant();
   SPGroup group = groups_[row];
   return group ? ((Qt::ToolTipRole == role) ? group->description() : group->name()): QString();
}


