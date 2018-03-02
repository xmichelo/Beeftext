/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo group class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboGroup.h"
#include "BeeftextConstants.h"


namespace {
QString const kPropUuid = "uuid"; ///< The JSon property name for the UUID
QString const kPropName = "name"; ///< The JSON property name for the name
QString const kPropDescription = "description"; ///< The JSON property name for the description
QString const kPropCreationDateTime = "creationDateTime"; ///< The JSON property name for the created date/time
QString const kPropModificationDateTime = "modificationDateTime"; ///< The JSON property name for the modification date/time
}


//**********************************************************************************************************************
/// \param[in] name The name of the group
/// \param[in] description The description of the group
//**********************************************************************************************************************
ComboGroup::ComboGroup(QString const& name, QString const& description)
   : uuid_(QUuid::createUuid())
   , name_(name)
   , description_(description)
{
   creationDateTime_ = modificationDateTime_ = QDateTime::currentDateTime();
}


//**********************************************************************************************************************
/// \param[in] object The JSON object
/// \param[in] formatVersion The version number of the JSON file format
//**********************************************************************************************************************
ComboGroup::ComboGroup(QJsonObject const& object, qint32 formatVersion)
   : uuid_(QUuid(object[kPropUuid].toString()))
   , name_(object[kPropName].toString())
   , description_(object[kPropDescription].toString())
   , creationDateTime_(QDateTime::fromString(object[kPropCreationDateTime].toString(), 
      constants::kJsonExportDateFormat))
   , modificationDateTime_(QDateTime::fromString(object[kPropModificationDateTime].toString(), 
      constants::kJsonExportDateFormat))
{

}


//**********************************************************************************************************************
/// \return true if and only if the group is valid
//**********************************************************************************************************************
bool ComboGroup::isValid() const
{
   return (!creationDateTime_.isNull()) && (!modificationDateTime_.isNull()) && (!uuid_.isNull()) && (!name_.isEmpty());
}


//**********************************************************************************************************************
/// \return The UUID of the combo group
//**********************************************************************************************************************
QUuid ComboGroup::uuid() const
{
   return uuid_;
}


//**********************************************************************************************************************
/// \return The name of the group
//**********************************************************************************************************************
QString ComboGroup::name() const
{
   return name_;
}


//**********************************************************************************************************************
/// \param[in] name The name of the group
//**********************************************************************************************************************
void ComboGroup::setName(QString const& name)
{
   if (name_ != name)
   {
      name_ = name;
      this->touch();
   }
}


//**********************************************************************************************************************
/// \return The description of the group
//**********************************************************************************************************************
QString ComboGroup::description() const
{
   return description_;
}


//**********************************************************************************************************************
/// \param[in] description The description of the group
//**********************************************************************************************************************
void ComboGroup::setDescription(QString const& description)
{
   if (description_ != description)
   {
      description_ = description;
      this->touch();
   }
}


//**********************************************************************************************************************
/// \return A JSON object representing the group
//**********************************************************************************************************************
QJsonObject ComboGroup::toJsonObject() const
{
   QJsonObject result;
   result.insert(kPropUuid, uuid_.toString());
   result.insert(kPropName, name_);
   result.insert(kPropDescription, description_);
   result.insert(kPropCreationDateTime, creationDateTime_.toString(constants::kJsonExportDateFormat));
   result.insert(kPropModificationDateTime, modificationDateTime_.toString(constants::kJsonExportDateFormat));
   return result;
}


//**********************************************************************************************************************
/// \param[in] name The name of the group
/// \param[in] description The description of the group
//**********************************************************************************************************************
SPComboGroup ComboGroup::create(QString const& name, QString const& description)
{
   return std::make_shared<ComboGroup>(name, description);
}


//**********************************************************************************************************************
/// \param[in] object The JSON object
/// \param[in] formatVersion The version number of the JSON file format
//**********************************************************************************************************************
SPComboGroup ComboGroup::create(QJsonObject const& object, qint32 formatVersion)
{
   return std::make_shared<ComboGroup>(object, formatVersion);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroup::touch()
{
   modificationDateTime_ = QDateTime::currentDateTime();
}


