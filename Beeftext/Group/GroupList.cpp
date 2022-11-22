/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo group list class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "GroupList.h"
#include "Combo/ComboManager.h"
#include "MimeDataUtils.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {
QString defaultGroupName() { return QObject::tr("Default Group"); } ///< The default group name
QString defaultGroupDescription() { return QObject::tr("The default group."); } ///< The default group description
}


//****************************************************************************************************************************************************
/// \param[in] first The first group
/// \param[in] second The second group
//****************************************************************************************************************************************************
void swap(GroupList &first, GroupList &second) noexcept {
    first.groups_.swap(second.groups_);
    std::swap(first.dropType_, second.dropType_);
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent object of the instance
//****************************************************************************************************************************************************
GroupList::GroupList(QObject *parent)
    : QAbstractListModel(parent)
    , dropType_(GroupDrop) {

}


//****************************************************************************************************************************************************
/// \param[in] ref The reference group to copy
//****************************************************************************************************************************************************
GroupList::GroupList(GroupList const &ref)
    : QAbstractListModel(ref.parent())
    , groups_(ref.groups_)
    , dropType_(ref.dropType_) {

}


//****************************************************************************************************************************************************
/// \param[in] ref The reference group to copy
//****************************************************************************************************************************************************
GroupList::GroupList(GroupList &&ref) noexcept
    : QAbstractListModel(ref.parent())
    , groups_(std::move(ref.groups_))
    , dropType_(ref.dropType_) {

}


//****************************************************************************************************************************************************
/// \param[in] ref The reference group to copy
/// \return A reference to the instance
//****************************************************************************************************************************************************
GroupList &GroupList::operator=(GroupList const &ref) {
    if (&ref != this) {
        groups_ = ref.groups_;
        dropType_ = ref.dropType_;
    }
    return *this;
}


//****************************************************************************************************************************************************
/// \param[in] ref The reference group to copy
/// \return A reference to the instance
//****************************************************************************************************************************************************
GroupList &GroupList::operator=(GroupList &&ref) noexcept {
    if (&ref != this) {
        groups_ = std::move(ref.groups_);
        dropType_ = ref.dropType_;
    }
    return *this;
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the group
/// \return The group a the specified index
//****************************************************************************************************************************************************
SpGroup &GroupList::operator[](qint32 index) {
    return groups_[static_cast<quint32>(index)];
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the group
/// \return The group a the specified index
//****************************************************************************************************************************************************
SpGroup const &GroupList::operator[](qint32 index) const {
    return groups_[static_cast<quint32>(index)];
}


//****************************************************************************************************************************************************
/// \return The number of groups in the list
//****************************************************************************************************************************************************
qint32 GroupList::size() const {
    return static_cast<qint32>(groups_.size());
}


//****************************************************************************************************************************************************
/// \return true if and only if the list is empty
//****************************************************************************************************************************************************
bool GroupList::isEmpty() const {
    return groups_.empty();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupList::clear() {
    groups_.clear();
}


//****************************************************************************************************************************************************
/// return true if and only if a group with this UUID is already in the list
//****************************************************************************************************************************************************
bool GroupList::contains(SpGroup const &group) const {
    return group ? this->end() != this->findByUuid(group->uuid()) : false;
}


//****************************************************************************************************************************************************
/// \return true if the group is valid and not already in the list
//****************************************************************************************************************************************************
bool GroupList::canGroupBeAdded(SpGroup const &group) const {
    return group && group->isValid() && !this->contains(group);
}


//****************************************************************************************************************************************************
/// \param[in] group The group to add
/// \return true if the group has been successfully added
//****************************************************************************************************************************************************
bool GroupList::append(SpGroup const &group) {
    if (!canGroupBeAdded(group)) {
        globals::debugLog().addError("Cannot add group (null or duplicate).");
        return false;
    }
    this->beginInsertRows(QModelIndex(), static_cast<qint32>(groups_.size()) + 1, static_cast<qint32>(groups_.size()) + 1);
    groups_.push_back(group);
    this->endInsertRows();
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] group The group to add
//****************************************************************************************************************************************************
// ReSharper disable once CppInconsistentNaming
void GroupList::push_back(SpGroup const &group) {
    this->beginInsertRows(QModelIndex(), static_cast<qint32>(groups_.size()) + 1, static_cast<qint32>(groups_.size()) + 1);
    groups_.push_back(group);
    this->endInsertRows();
}


//****************************************************************************************************************************************************
/// \param[in] index the index of the group to erase
//****************************************************************************************************************************************************
void GroupList::erase(qint32 index) {
    this->beginRemoveRows(QModelIndex(), index + 1, index + 1);
    groups_.erase(groups_.begin() + index);
    this->endRemoveRows();
}


//****************************************************************************************************************************************************
/// \param[in] uuid The UUID of the combo to find
/// \return An iterator to the found group or end() if not found
//****************************************************************************************************************************************************
GroupList::iterator GroupList::findByUuid(QUuid const &uuid) {
    return std::find_if(this->begin(), this->end(), [&](SpGroup const &group) -> bool { return group && uuid == group->uuid(); });
}


//****************************************************************************************************************************************************
/// \param[in] uuid The UUID of the combo to find
/// \return A constant iterator to the found group or end() if not found
//****************************************************************************************************************************************************
GroupList::const_iterator GroupList::findByUuid(QUuid const &uuid) const {
    return std::find_if(this->begin(), this->end(), [&](SpGroup const &group) -> bool { return group && uuid == group->uuid(); });
}


//****************************************************************************************************************************************************
/// \return An iterator to the beginning of the list
//****************************************************************************************************************************************************
GroupList::iterator GroupList::begin() {
    return groups_.begin();
}


//****************************************************************************************************************************************************
/// \return A constant iterator to the beginning of the list
//****************************************************************************************************************************************************
GroupList::const_iterator GroupList::begin() const {
    return groups_.begin();
}


//****************************************************************************************************************************************************
/// \return An iterator to the end of the list
//****************************************************************************************************************************************************
GroupList::iterator GroupList::end() {
    return groups_.end();
}


//****************************************************************************************************************************************************
/// \return A constant iterator to the end of the list
//****************************************************************************************************************************************************
GroupList::const_iterator GroupList::end() const {
    return groups_.end();
}


//****************************************************************************************************************************************************
/// \return An iterator to the reverse beginning of the list
//****************************************************************************************************************************************************
GroupList::reverse_iterator GroupList::rbegin() {
    return groups_.rbegin();
}


//****************************************************************************************************************************************************
/// \return A constant iterator to the reverse beginning of the list
//****************************************************************************************************************************************************
GroupList::const_reverse_iterator GroupList::rbegin() const {
    return groups_.rbegin();
}


//****************************************************************************************************************************************************
/// \return An iterator to the reverse end of the list
//****************************************************************************************************************************************************
GroupList::reverse_iterator GroupList::rend() {
    return groups_.rend();
}


//****************************************************************************************************************************************************
/// \return A constant iterator to the reverse end of the list
//****************************************************************************************************************************************************
GroupList::const_reverse_iterator GroupList::rend() const {
    return groups_.rend();
}


//****************************************************************************************************************************************************
/// \return A JSON array containing the combo list
//****************************************************************************************************************************************************
QJsonArray GroupList::toJsonArray() const {
    QJsonArray result;
    for (SpGroup const &group: groups_)
        result.append(group->toJsonObject());
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] array The JSON array to parse
/// \param[in] formatVersion The JSON file format version
/// \param[out] outErrorMessage if not null and the function returns false, this variable hold a description of the
/// error on exit
/// \return true if and only if the array was parsed successfully
//****************************************************************************************************************************************************
bool GroupList::readFromJsonArray(QJsonArray const &array, qint32 formatVersion, QString *outErrorMessage) {
    try {
        this->clear();
        for (QJsonValueConstRef const value: array) {
            if (!value.isObject())
                throw xmilib::Exception("The group list is invalid.");
            SpGroup const group = Group::create(value.toObject(), formatVersion);
            if ((!group) || (!group->isValid()))
                throw xmilib::Exception("The group list contains an invalid group.");
            if (!this->append(group))
                throw xmilib::Exception("Could not append one of the groups to the group list.");
        }
        return true;
    }
    catch (xmilib::Exception const &e) {
        if (outErrorMessage)
            *outErrorMessage = e.qwhat();
        return false;
    }
}


//****************************************************************************************************************************************************
/// \return true if the group list was empty and had to be filled with a default group
//****************************************************************************************************************************************************
bool GroupList::ensureNotEmpty() {
    bool const empty = groups_.empty();
    if (empty)
        groups_.push_back(Group::create(defaultGroupName(), defaultGroupDescription()));
    return empty;
}


//****************************************************************************************************************************************************
/// \param[in] title The title of the menu
/// \param[in] disabledGroups The list of groups that should be disabled
/// \param[in] parent The parent widget of the menu
/// \return A menu containing the list of groups
//****************************************************************************************************************************************************
QMenu *GroupList::createMenu(QString const &title, std::set<SpGroup> const &disabledGroups, QWidget *parent) const {
    QMenu *menu = new QMenu(title, parent);
    this->fillMenu(menu, disabledGroups);
    return menu;
}


//****************************************************************************************************************************************************
/// \param[in] menu The menu
/// \param[in] disabledGroups The list of groups that should be disabled
//****************************************************************************************************************************************************
void GroupList::fillMenu(QMenu *menu, std::set<SpGroup> const &disabledGroups) const {
    menu->clear();
    for (SpGroup const &group: groups_) {
        if (!group)
            continue;
        QAction *action = new QAction(group->name(), menu);
        action->setData(QVariant::fromValue(group));
        action->setEnabled(!disabledGroups.count(group));
        menu->addAction(action);
    }
}


//****************************************************************************************************************************************************
/// The drop type is a bit of a hack. The flags() model function determines if an item should accept a drop, 
/// but it should return a different value depending on what is dragged (combo or group)
//****************************************************************************************************************************************************
void GroupList::setDropType(EDropType dropType) {
    dropType_ = dropType;
}


//****************************************************************************************************************************************************
/// \param[in] uuids The list of UUID of the dropped combos
/// \param[in] index the index of the group the combos were dropped on
//****************************************************************************************************************************************************
bool GroupList::processComboListDrop(QList<QUuid> const &uuids, qint32 index) {
    if ((index < 0) || (index >= qint32(groups_.size())) || (!groups_[static_cast<quint32>(index)]) || uuids.isEmpty())
        return false;
    SpGroup const &group = groups_[static_cast<quint32>(index)];
    ComboList &comboList = ComboManager::instance().comboListRef();
    bool changed = false;
    for (QUuid const &uuid: uuids) {
        ComboList::iterator const it = comboList.findByUuid(uuid);
        if ((it == comboList.end()) || ((*it)->group() == group))
            continue;
        (*it)->setGroup(group);
        changed = true;
    }

    if (changed)
        emit combosChangedGroup();
    return changed;
}


//****************************************************************************************************************************************************
/// \param[in] groupIndex The index of the dropped group in the list
/// \param[in] dropIndex The index at which the group has been drop
//****************************************************************************************************************************************************
bool GroupList::processGroupDrop(qint32 groupIndex, qint32 dropIndex) {
    qint32 const listSize = static_cast<qint32>(groups_.size());
    if ((dropIndex < 0) || (groupIndex < 0) || (groupIndex >= listSize))
        return false;
    SpGroup const group = groups_[static_cast<quint32>(groupIndex)];
    if (!group)
        return false;

    // phase 1: copy the group to it news location
    if ((dropIndex == groupIndex) || (dropIndex == groupIndex + 1)) // no effect
        return false;
    this->beginInsertRows(QModelIndex(), dropIndex, dropIndex);
    groups_.insert(groups_.begin() + dropIndex, group);
    this->endInsertRows();

    // phase 2: remove the old item
    qint32 const removeIndex = groupIndex + ((dropIndex >= 0) && (dropIndex < groupIndex) ? 1 : 0); // the srcIndex may have been shifted by the insertion of the copy
    this->beginRemoveRows(QModelIndex(), removeIndex + 1, removeIndex + 1);
    groups_.erase(groups_.begin() + removeIndex);
    this->endRemoveRows();
    this->beginResetModel();
    this->endResetModel();

    // emit a notification signal
    const_iterator const newPosIt = this->findByUuid(group->uuid());
    if (newPosIt == this->end())
        throw xmilib::Exception("Internal error: %1(): could not located moved group.");
    emit groupMoved(group, qint32(newPosIt - this->begin()));
    return true;

}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
int GroupList::rowCount(const QModelIndex &) const {
    return static_cast<qint32>(groups_.size()) + 1;
}


//****************************************************************************************************************************************************
/// \param[in] index The model index
/// \param[in] role The role
/// \return the model data for the given role at the given index
//****************************************************************************************************************************************************
QVariant GroupList::data(QModelIndex const &index, int role) const {
    // note entry at index 0 is special entry "<All combos>"
    QString allCombosStr = tr("<All combos>");
    qint32 const groupCount = static_cast<qint32>(groups_.size());
    qint32 const row = index.row();
    if ((row < 0) || (row > groupCount))
        return QVariant();
    switch (role) {
    case Qt::DisplayRole: {
        if (0 == row)
            return allCombosStr;
        SpGroup const &group = groups_[static_cast<quint32>(row) - 1];
        return group ? group->name() : QString();
    }
    case Qt::ToolTipRole: {
        if (0 == row)
            return allCombosStr;
        SpGroup const &group = groups_[static_cast<quint32>(row) - 1];
        return group ? group->description() : QString();
    }
    case Qt::FontRole: {
        if (0 != row)
            return QVariant();
        // we use italic to hight special <All combos> entry
        QFont font;
        font.setItalic(true);
        return font;
    }
    case Qt::ForegroundRole: {
        if (0 == row)
            return QVariant();
        SpGroup const &group = groups_[static_cast<quint32>(row) - 1];
        return group->enabled() ? QVariant() : globals::disabledTextColorInTablesAndLists();
    }
    default:
        return QVariant();
    }
}


//****************************************************************************************************************************************************
/// \return The supported drop actions
//****************************************************************************************************************************************************
Qt::DropActions GroupList::supportedDropActions() const {
    return Qt::MoveAction;
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the item
/// \return The item flags
//****************************************************************************************************************************************************
Qt::ItemFlags GroupList::flags(QModelIndex const &index) const {
    Qt::ItemFlags const defaultFlags = QAbstractListModel::flags(index);
    // The drop behavior is dictated by the validity of the index, an invalid index means the dropping target is
    // between two valid items
    if (dropType_ == GroupDrop) /// user is dragging a group
    {
        if (index.isValid()) // group cannot be drop onto other items
            return Qt::ItemIsDragEnabled | defaultFlags;
        return Qt::ItemIsDropEnabled | defaultFlags; // group can be dropped in between items
    }
    // user is dragging combos
    if (index.isValid() && index.row() > 0) // combos can be dropped onto other items but not on the first ("all combos")
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    return defaultFlags; // combos cannot be dropped between item
}


//****************************************************************************************************************************************************
/// \return The list of supported MIME type for dropping
//****************************************************************************************************************************************************
QStringList GroupList::mimeTypes() const {
    return QStringList() << kGroupIndexMimeType << kUuuidListMimeType;
}


//****************************************************************************************************************************************************
/// \param[in] indexes The indexes 
/// \return The MIME data for the given indexes
//****************************************************************************************************************************************************
QMimeData *GroupList::mimeData(const QModelIndexList &indexes) const {
    return groupIndexToMimeData(indexes.isEmpty() ? -1 : qMax<qint32>(-1, indexes[0].row() - 1)); // row - 1 because entry at index 0 is special entry "<All combos>"
}


//****************************************************************************************************************************************************
/// \param[in] data The MIME data
/// \param[in] row The row 
/// \param[in] parent The parent index
/// \return true if and only if the drop was successfully processed
//****************************************************************************************************************************************************
bool GroupList::dropMimeData(QMimeData const *data, Qt::DropAction, int row, int,
    QModelIndex const &parent) {
    if (!data)
        return false;

    if (data->hasFormat(kUuuidListMimeType))
        return processComboListDrop(mimeDataToUuidList(*data), parent.row() - 1); // row - 1 because entry at index 0 is special entry "<All combos>"

    return processGroupDrop(mimeDataToGroupIndex(*data), row - 1); // row - 1 because entry at index 0 is special entry "<All combos>"
}


