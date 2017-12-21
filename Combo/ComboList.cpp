/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of Combo list class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboList.h"
#include <XMiLib/Exception.h>


namespace {
qint32 const kJsonComboListFileFormatVersionNumber = 1; ///< The version number for the combo list file format
QString const kDefaultGroupName = "Default"; ///< The default combo group name
QString const kKeyFileFormatVersion = "fileFormatVersion"; ///< The JSon key for the file format version
QString const kKeyCombos = "combos"; ///< The JSon key for combos
}


QString const ComboList::defaultFileName = "comboList.json";


//**********************************************************************************************************************
/// \param[in] parent The parent object of the model
//**********************************************************************************************************************
ComboList::ComboList(QObject* parent)
   : QAbstractTableModel(parent)
{

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
   this->endResetModel();
}


//**********************************************************************************************************************
/// \param[in] combo The combo to append
//**********************************************************************************************************************
void ComboList::append(SPCombo const& combo)
{
   /// \todo Check if the combo is already in the list (UUID check)
   /// \todo Check if the combo text is not already used
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
/// Return a JSon document containing the combo list
//**********************************************************************************************************************
QJsonDocument ComboList::toJsonDocument() const
{
   QJsonObject rootObject;
   rootObject.insert(kKeyFileFormatVersion, kJsonComboListFileFormatVersionNumber);
   QJsonArray comboArray;
   for (SPCombo const& combo : combos_)
      comboArray.append(combo->toJsonObject());
   rootObject.insert(kKeyCombos, comboArray);
   return QJsonDocument(rootObject);
}


//**********************************************************************************************************************
/// If this function returns false, the content of the instance the class is undetermined on exit
///
/// \note The existing contents of the combo list is erased
///
/// \param[in] doc The JSON document
/// \param[out] outErrorMsg If the function return false and this parameter is not null, this variable contains a
/// description of the error when the function returns
/// \return true if and only if the parsing completed successfully
//**********************************************************************************************************************
bool ComboList::readFromJsonDocument(QJsonDocument const& doc, QString* outErrorMsg)
{
   try
   {
      this->clear();
      if (!doc.isObject())
         throw xmilib::Exception(QObject::tr("The combo list file is invalid."));
      QJsonObject const rootObject = doc.object();

      // check the file format version number
      QJsonValue const versionValue = rootObject[kKeyFileFormatVersion];
      if (!versionValue.isDouble()) // the JSon format consider all numbers as double
         throw xmilib::Exception(QObject::tr("The combo list file does not specify its version number."));
      qint32 const version = versionValue.toInt();
      if (version > kKeyFileFormatVersion)
         throw xmilib::Exception(QObject::tr("The combo list file was created by a newer version of the "
            "application."));

      // parse the combos
      QJsonValue const combosListValue = rootObject[kKeyCombos];
      if (!combosListValue.isArray())
         throw xmilib::Exception(QObject::tr("The list of combos is not a valid array"));
      for (QJsonValue const& comboGroupValue : combosListValue.toArray())
      {
         if (!comboGroupValue.isObject())
            throw xmilib::Exception(QObject::tr("The combo list array contains an invalid combo."));
         SPCombo combo = Combo::create(comboGroupValue.toObject());
         if ((!combo) || (!combo->isValid()))
            throw xmilib::Exception(QObject::tr("One of the combo in the list is invalid"));
         this->append(combo);
      }
      return true;
   }
   catch (xmilib::Exception const& e)
   {
      if (outErrorMsg)
         *outErrorMsg = QObject::tr("An error occurred while parsing the combo list file: %1").arg(e.qwhat());
      return false;
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboList::markComboAsEdited(qint32 index)
{
   Q_ASSERT((index >= 0) && (index < combos_.size()));
   emit dataChanged(this->index(0, 0), this->index(0, this->rowCount() - 1), QVector<int>() << Qt::DisplayRole);
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
   if ((row < 0) || (row >= combos_.size()))
      return QVariant();

   SPCombo combo = combos_[row];

   if (Qt::DisplayRole == role)   
      switch (index.column())
      {
      case 0: return combo->name();
      case 1: return combo->comboText();
      case 2: return combo->substitutionText().trimmed().simplified();
      default: return QVariant();
      }

   if (Qt::ToolTipRole == role)
   {
      switch (index.column())
      {
      case 0: return combo->name();
      case 1: return combo->comboText();
      case 2: return combo->substitutionText();
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
         case 1: return tr("Combo");
         case 2: return tr("Substitution text");
         default: return QVariant();
      }

   return QVariant();
}


