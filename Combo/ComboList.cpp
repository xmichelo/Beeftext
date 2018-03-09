/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of Combo list class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboList.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {
qint32 const kJsonComboListFileFormatVersionNumber = 3; ///< The version number for the combo list file format
QString const kKeyFileFormatVersion = "fileFormatVersion"; ///< The JSon key for the file format version
QString const kKeyCombos = "combos"; ///< The JSon key for combos
QString const kKeyGroups = "groups"; ///< The JSon key for groups
}


QString const ComboList::defaultFileName = "comboList.json";


//**********************************************************************************************************************
/// \param[in] first The first combo
/// \param[in] second The second combo
//**********************************************************************************************************************
void swap(ComboList& first, ComboList& second)
{
   first.combos_.swap(second.combos_);
   swap(first.groups_, second.groups_);
}

//**********************************************************************************************************************
/// \param[in] parent The parent object of the model
//**********************************************************************************************************************
ComboList::ComboList(QObject* parent)
   : QAbstractTableModel(parent)
{

}


//**********************************************************************************************************************
/// \param[in] ref The combo list to copy from
//**********************************************************************************************************************
ComboList::ComboList(ComboList const& ref)
   : combos_(ref.combos_)
   , groups_(ref.groups_)
{

}


//**********************************************************************************************************************
/// \param[in] ref The combo list to copy from
//**********************************************************************************************************************
ComboList::ComboList(ComboList&& ref)
   : combos_(std::move(ref.combos_))
   , groups_(std::move(ref.groups_))
{

}


//**********************************************************************************************************************
/// \param[in] ref The combo list to copy fromS
//**********************************************************************************************************************
ComboList& ComboList::operator=(ComboList const& ref)
{
   if (&ref != this)
   {
      combos_ = ref.combos_;
      groups_ = ref.groups_;
   }
   return *this;
}


//**********************************************************************************************************************
/// \param[in] ref The combo list to copy from
//**********************************************************************************************************************
ComboList& ComboList::operator=(ComboList&& ref)
{
   if (&ref != this)
   {
      combos_ = std::move(ref.combos_);
      groups_ = std::move(ref.groups_);
   }
   return *this;
}


//**********************************************************************************************************************
/// \return A reference to the group list
//**********************************************************************************************************************
GroupList& ComboList::groupListRef()
{
   return groups_;
}


//**********************************************************************************************************************
/// \return A constant reference to the group list
//**********************************************************************************************************************
GroupList const& ComboList::groupListRef() const
{
   return groups_;
}

//**********************************************************************************************************************
/// \return The number of combos in the combo list
//**********************************************************************************************************************
qint32 ComboList::size() const
{
   return combos_.size();
}


//**********************************************************************************************************************
/// \return true if and only if the combo list is empty
//**********************************************************************************************************************
bool ComboList::isEmpty() const
{
   return combos_.empty();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboList::clear()
{
   this->beginResetModel();
   combos_.clear();
   groups_.clear();
   this->endResetModel();
}


//**********************************************************************************************************************
/// \param[in] combo The combo
/// \return if a combo with the same UUID is already in the list
//**********************************************************************************************************************
bool ComboList::contains(SPCombo const& combo) const
{
   return combo ? this->end() != this->findByUuid(combo->uuid()) : false;
}


//**********************************************************************************************************************
/// \param[in] keyword The keyword
/// \return true if the keyword is used by a combo in the list
//**********************************************************************************************************************
bool ComboList::isKeywordUsed(QString const& keyword) const
{
   return this->end() != this->findByKeyword(keyword);
}


//**********************************************************************************************************************
/// \return 
//**********************************************************************************************************************
bool ComboList::canComboBeAdded(SPCombo const& combo) const
{
   return combo ? !(this->contains(combo) || this->isKeywordUsed(combo->keyword())) : false;
}


//**********************************************************************************************************************
/// \param[in] combo The combo to append
/// \return true if and only if the combo was successfully added to the list
//**********************************************************************************************************************
bool ComboList::append(SPCombo const& combo)
{
   if (!this->canComboBeAdded(combo))
   {
      globals::debugLog().addError("Cannot add combo (duplicate or keyword conflict).");
      return false;
   }
   this->beginInsertRows(QModelIndex(), combos_.size(), combos_.size());
   combos_.push_back(combo);
   this->endInsertRows();
   return true;
}


//**********************************************************************************************************************
/// \note Unlike ComboList::append() this variant does not perform any check on combo prior to adding
///
/// \param[in] combo the combo to add
//**********************************************************************************************************************
void ComboList::push_back(SPCombo const& combo)
{
   this->beginInsertRows(QModelIndex(), combos_.size(), combos_.size());
   combos_.push_back(combo);
   this->endInsertRows();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboList::erase(qint32 index)
{
   this->beginRemoveRows(QModelIndex(), index, index);
   combos_.erase(combos_.begin() + index);
   this->endRemoveRows();
}


//**********************************************************************************************************************
/// \param[in] group The group
//**********************************************************************************************************************
void ComboList::eraseCombosOfGroup(SPGroup const& group)
{
   if (!group)
      return;
   QUuid const uuid = group->uuid();
   for (qint32 index = combos_.size() - 1; index >= 0; --index)
   {
      SPCombo combo = combos_[index];
      if (!combo)
         continue;
      SPGroup group = combo->group();
      if (group && (uuid == group->uuid()))
         this->erase(index);

   }
}


//**********************************************************************************************************************
/// \param[in] keyword The keyword
/// \return A constant iterator to to the combo with the specified keyword
/// \return A null shared pointer if the combo list contains no combo with the specified keyword
//**********************************************************************************************************************
ComboList::const_iterator ComboList::findByKeyword(QString const& keyword) const
{
   return std::find_if(this->begin(), this->end(),
      [&](SPCombo const& combo) -> bool { return combo->keyword() == keyword; });
}


//**********************************************************************************************************************
/// \param[in] keyword The keyword
/// \return An iterator to to the combo with the specified keyword
/// \return A null shared pointer if the combo list contains no combo with the specified keyword
//**********************************************************************************************************************
ComboList::iterator ComboList::findByKeyword(QString const& keyword)
{
   return std::find_if(this->begin(), this->end(),
      [&](SPCombo const& combo) -> bool { return combo->keyword() == keyword; });
}


//**********************************************************************************************************************
/// \param[in] uuid The UUID
/// \return An iterator to to the combo with the specified UUID
/// \return A null shared pointer if the combo list contains no combo with the specified UUID
//**********************************************************************************************************************
ComboList::iterator ComboList::findByUuid(QUuid const& uuid)
{
   return std::find_if(this->begin(), this->end(),
      [&](SPCombo const& combo) -> bool { return combo->uuid() == uuid; });
}


//**********************************************************************************************************************
/// \param[in] uuid The UUID
/// \return A constant iterator to to the combo with the specified UUID
/// \return A null shared pointer if the combo list contains no combo with the specified UUID
//**********************************************************************************************************************
ComboList::const_iterator ComboList::findByUuid(QUuid const& uuid) const
{
   return std::find_if(this->begin(), this->end(),
      [&](SPCombo const& combo) -> bool { return combo->uuid() == uuid; });
}


//**********************************************************************************************************************
/// \param[in] index The index of the combo to retrieve
/// \return A reference to the combo at the given index
//**********************************************************************************************************************
SPCombo& ComboList::operator[](qint32 index)
{
   return combos_[index];
}


//**********************************************************************************************************************
/// \param[in] index The index of the combo to retrieve
/// \return A constant reference to the combo at the given index
//**********************************************************************************************************************
SPCombo const& ComboList::operator[](qint32 index) const
{
   return combos_[index];
}


//**********************************************************************************************************************
/// \return An iterator to the beginning of the combo list
//**********************************************************************************************************************
ComboList::iterator ComboList::begin()
{
   return combos_.begin();
}


//**********************************************************************************************************************
/// \return A constant iterator to the beginning of the combo list
//**********************************************************************************************************************
ComboList::const_iterator ComboList::begin() const
{
   return combos_.begin();
}


//**********************************************************************************************************************
/// \return An iterator to the end of the combo list
//**********************************************************************************************************************
ComboList::iterator ComboList::end()
{
   return combos_.end();
}


//**********************************************************************************************************************
/// \return A constant iterator to the end of the combo list
//**********************************************************************************************************************
ComboList::const_iterator ComboList::end() const
{
   return combos_.end();
}


//**********************************************************************************************************************
/// \return A reverse iterator to the beginning of the list
//**********************************************************************************************************************
ComboList::reverse_iterator ComboList::rbegin()
{
   return combos_.rbegin();
}


//**********************************************************************************************************************
/// \return A constant reverse iterator to the beginning of the list
//**********************************************************************************************************************
ComboList::const_reverse_iterator ComboList::rbegin() const
{
   return combos_.rbegin();
}


//**********************************************************************************************************************
/// \return A reverse iterator to the end of the list
//**********************************************************************************************************************
ComboList::reverse_iterator ComboList::rend()
{
   return combos_.rend();
}


//**********************************************************************************************************************
/// \return A constant reverse iterator to the end of the list
//**********************************************************************************************************************
ComboList::const_reverse_iterator ComboList::rend() const
{
   return combos_.rend();
}


//**********************************************************************************************************************
/// \param[in] includeGroups Should the groups be saved 
/// Return a JSon document containing the combo list
//**********************************************************************************************************************
QJsonDocument ComboList::toJsonDocument(bool includeGroups) const
{
   QJsonObject rootObject;
   rootObject.insert(kKeyFileFormatVersion, kJsonComboListFileFormatVersionNumber);
   QJsonArray comboArray;
   for (SPCombo const& combo : combos_)
      comboArray.append(combo->toJsonObject(includeGroups));
   rootObject.insert(kKeyCombos, comboArray);
   rootObject.insert(kKeyGroups, includeGroups ? groups_.toJsonArray() : QJsonArray());
   return QJsonDocument(rootObject);
}


//**********************************************************************************************************************
/// If this function returns false, the content of the instance the class is undetermined on exit
///
/// \note The existing contents of the combo list is erased
///
/// \param[in] doc The JSON document
/// \param[out] outInOlderFileFormat If the function returns true and this parameter is not null, this variable
/// is true if the loaded file format version is not the latest
/// \param[out] outErrorMsg If the function return false and this parameter is not null, this variable contains a
/// description of the error when the function returns
/// \return true if and only if the parsing completed successfully
//**********************************************************************************************************************
bool ComboList::readFromJsonDocument(QJsonDocument const& doc, bool* outInOlderFileFormat, QString* outErrorMsg)
{
   try
   {
      this->clear();
      if (!doc.isObject())
         throw xmilib::Exception("The combo list file is invalid.");
      QJsonObject const rootObject = doc.object();

      // check the file format version number
      QJsonValue const versionValue = rootObject[kKeyFileFormatVersion];
      if (!versionValue.isDouble()) // the JSon format consider all numbers as double
         throw xmilib::Exception("The combo list file does not specify its version number.");
      qint32 const version = versionValue.toInt();
      if (version > kJsonComboListFileFormatVersionNumber)
         throw xmilib::Exception("The combo list file was created by a newer version of the application.");

      // parse the groups
      if (version >= 3)
      {
         QJsonValue const groupListValue = rootObject[kKeyGroups];
         if (!groupListValue.isArray())
            throw xmilib::Exception("The list of groups is not a valid array");
         QString errorMsg;
         if (!groups_.readFromJsonArray(groupListValue.toArray(), version, &errorMsg))
            throw xmilib::Exception(errorMsg);
      }

      // parse the combos
      QJsonValue const combosListValue = rootObject[kKeyCombos];
      if (!combosListValue.isArray())
         throw xmilib::Exception("The list of combos is not a valid array");
      for (QJsonValueRef const& comboValue : combosListValue.toArray())
      {
         if (!comboValue.isObject())
            throw xmilib::Exception("The combo list array contains an invalid combo.");
         SPCombo const combo = Combo::create(comboValue.toObject(), version, groups_);
         if ((!combo) || (!combo->isValid()))
            throw xmilib::Exception("One of the combo in the list is invalid");
         this->append(combo);
      }
      if (outInOlderFileFormat)
         *outInOlderFileFormat = (version < kJsonComboListFileFormatVersionNumber);
      return true;
   }
   catch (xmilib::Exception const& e)
   {
      if (outErrorMsg)
         *outErrorMsg = QString("An error occurred while parsing the combo list file: %1").arg(e.qwhat());
      return false;
   }
}


//**********************************************************************************************************************
/// \param[in] path The path of the file to read from
/// \param[out] outInOlderFileFormat If the function return true and this parameter is not null, this variable is
/// true on exit if the loaded file is in a file format that is not the latest one
/// \param[out] outErrorMessage If the function return false and this parameter is not null, the string pointed to 
/// contains a description of the error
/// \return true if and only if the combo list was successfully loaded from file
//**********************************************************************************************************************
bool ComboList::load(QString const& path, bool* outInOlderFileFormat, QString* outErrorMessage)
{
   try
   {
      this->clear();
      QFile file(path);
      if ((!file.exists()) || (!file.open(QIODevice::ReadOnly)))
         throw xmilib::Exception(QString("Could not open file for reading: '%1'").arg(QDir::toNativeSeparators(path)));
      return this->readFromJsonDocument(QJsonDocument::fromJson(file.readAll()), outInOlderFileFormat, outErrorMessage);
   }
   catch (xmilib::Exception const& e)
   {
      if (outErrorMessage)
         *outErrorMessage = e.qwhat();
      return false;
   }
}


//**********************************************************************************************************************
/// \param[in] path The path of the file to save to
/// \param[in] saveGroups Should the groups be saved
/// \param[out] outErrorMessage If the function return false and this parameter is not null, the string pointed to 
/// contains a description of the error
/// \return true if and only if the combo list was successfully saved to file
//**********************************************************************************************************************
bool ComboList::save(QString const& path, bool saveGroups, QString* outErrorMessage) const
{
   try
   {
      QFile file(path);
      if (!file.open(QIODevice::WriteOnly))
         throw xmilib::Exception(QString("Could not open file for writing: '%1'").arg(QDir::toNativeSeparators(path)));
      QByteArray const data = this->toJsonDocument(saveGroups).toJson();
      if (data.size() != file.write(data))
         throw xmilib::Exception(QString("Error writing to file: %1").arg(QDir::toNativeSeparators(path)));
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
// 
//**********************************************************************************************************************
void ComboList::markComboAsEdited(qint32 index)
{
   Q_ASSERT((index >= 0) && (index < qint32(combos_.size())));
   emit dataChanged(this->index(0, 0), this->index(0, this->rowCount() - 1), QVector<int>() << Qt::DisplayRole);
}


//**********************************************************************************************************************
/// \return false if and only if the grouping was incorrect and had to be fixed
//**********************************************************************************************************************
void ComboList::ensureCorrectGrouping(bool *outWasInvalid)
{
   bool wasInvalid = groups_.ensureNotEmpty();
   for (SPCombo combo : combos_)
   {
      if (!combo)
         continue;
      SPGroup group = combo->group();
      if ((!group) || (groups_.end() == groups_.findByUuid(group->uuid())))
      {
         combo->setGroup(groups_[0]);
         wasInvalid = true;
      }
   }
   if (outWasInvalid)
      *outWasInvalid = wasInvalid;
}


//**********************************************************************************************************************
/// \return The number of rows in the table model
//**********************************************************************************************************************
int ComboList::rowCount(QModelIndex const&) const
{
   return combos_.size();
}


//**********************************************************************************************************************
/// \return The number of columns in the table model
//**********************************************************************************************************************
int ComboList::columnCount(QModelIndex const&) const
{
   return 3;
}


//**********************************************************************************************************************
/// \param[in] index The model index of the of the data to retrieve
/// \param[in] role The role of the data to retrieve
/// \return The retrieved data
//**********************************************************************************************************************
QVariant ComboList::data(QModelIndex const& index, int role) const
{
   qint32 const row = index.row();
   if ((row < 0) || (row >= qint32(combos_.size())))
      return QVariant();

   SPCombo const combo = combos_[row];

   if (Qt::DisplayRole == role)   
      switch (index.column())
      {
      case 0: return combo->name();
      case 1: return combo->keyword();
      case 2: return combo->snippet().trimmed().simplified();
      default: return QVariant();
      }

   if (Qt::ToolTipRole == role)
   {
      switch (index.column())
      {
      case 0: return combo->name();
      case 1: return combo->keyword();
      case 2: return combo->snippet();
      default: return QVariant();
      }
   }

   if (Qt::ForegroundRole == role)
      return combo->isEnabled() ? QVariant() : QColor(160, 160, 160);

   return QVariant();
}


//**********************************************************************************************************************
/// \param[in] section The index of the section
/// \param[in] orientation the orientation of the header
/// \param[in] role The role of the header data
/// \return The retrieved header data
//**********************************************************************************************************************
QVariant ComboList::headerData(int section, Qt::Orientation orientation, int role) const
{
   if (Qt::Horizontal != orientation)
      return QVariant();

   if (Qt::DisplayRole == role)
      switch (section)
      {
         case 0: return tr("Name");
         case 1: return tr("Keyword");
         case 2: return tr("Snippet");
         default: return QVariant();
      }

   return QVariant();
}
