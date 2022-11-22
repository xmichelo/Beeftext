/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo group class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "Group.h"
#include <utility>
#include "BeeftextConstants.h"


namespace {
QString const kPropUuid = "uuid"; ///< The JSon property name for the UUID
QString const kPropName = "name"; ///< The JSON property name for the name
QString const kPropDescription = "description"; ///< The JSON property name for the description
QString const kPropCreationDateTime = "creationDateTime"; ///< The JSON property name for the created date/time
QString const kPropModificationDateTime = "modificationDateTime"; ///< The JSON property name for the modification date/time
QString const kPropEnabled = "enabled"; ///< The JSON property for the enabled/disabled state of the group
}


//****************************************************************************************************************************************************
/// \param[in] name The name of the group
/// \param[in] description The description of the group
//****************************************************************************************************************************************************
Group::Group(QString name, QString description)
    : uuid_(QUuid::createUuid())
    , name_(std::move(name))
    , description_(std::move(description)) {
    creationDateTime_ = modificationDateTime_ = QDateTime::currentDateTime();
}


//****************************************************************************************************************************************************
/// \param[in] object The JSON object
/// \param[in] formatVersion The version number of the JSON file format
//****************************************************************************************************************************************************
Group::Group(QJsonObject const &object, qint32 formatVersion)
    : uuid_(QUuid(object[kPropUuid].toString()))
    , name_(object[kPropName].toString())
    , description_(object[kPropDescription].toString())
    , creationDateTime_(QDateTime::fromString(object[kPropCreationDateTime].toString(), constants::kJsonExportDateFormat))
    , modificationDateTime_(QDateTime::fromString(object[kPropModificationDateTime].toString(), constants::kJsonExportDateFormat)) {
    (void) formatVersion; // avoid warning in MinGW. We will for sure use this variable later
    if (object.contains(kPropEnabled))
        enabled_ = object[kPropEnabled].toBool(true);
}


//****************************************************************************************************************************************************
/// \return true if and only if the group is valid
//****************************************************************************************************************************************************
bool Group::isValid() const {
    return (!creationDateTime_.isNull()) && (!modificationDateTime_.isNull()) && (!uuid_.isNull()) && (!name_.isEmpty());
}


//****************************************************************************************************************************************************
/// \return The UUID of the combo group
//****************************************************************************************************************************************************
QUuid Group::uuid() const {
    return uuid_;
}


//****************************************************************************************************************************************************
/// \return The name of the group
//****************************************************************************************************************************************************
QString Group::name() const {
    return name_;
}


//****************************************************************************************************************************************************
/// \param[in] name The name of the group
//****************************************************************************************************************************************************
void Group::setName(QString const &name) {
    if (name_ != name) {
        name_ = name;
        this->touch();
    }
}


//****************************************************************************************************************************************************
/// \return The description of the group
//****************************************************************************************************************************************************
QString Group::description() const {
    return description_;
}


//****************************************************************************************************************************************************
/// \param[in] description The description of the group
//****************************************************************************************************************************************************
void Group::setDescription(QString const &description) {
    if (description_ != description) {
        description_ = description;
        this->touch();
    }
}


//****************************************************************************************************************************************************
/// \return true if and only if the group is enabled.
//****************************************************************************************************************************************************
bool Group::enabled() const {
    return enabled_;
}


//****************************************************************************************************************************************************
/// \param[in] enable The enabled/disabled state of the group.
//****************************************************************************************************************************************************
void Group::setEnabled(bool enable) {
    enabled_ = enable;
    this->touch();
}


//****************************************************************************************************************************************************
/// \return A JSON object representing the group
//****************************************************************************************************************************************************
QJsonObject Group::toJsonObject() const {
    QJsonObject result;
    result.insert(kPropUuid, uuid_.toString());
    result.insert(kPropName, name_);
    result.insert(kPropDescription, description_);
    result.insert(kPropCreationDateTime, creationDateTime_.toString(constants::kJsonExportDateFormat));
    result.insert(kPropModificationDateTime, modificationDateTime_.toString(constants::kJsonExportDateFormat));
    result.insert(kPropEnabled, enabled_);
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] name The name of the group
/// \param[in] description The description of the group
//****************************************************************************************************************************************************
SpGroup Group::create(QString const &name, QString const &description) {
    return std::make_shared<Group>(name, description);
}


//****************************************************************************************************************************************************
/// \param[in] object The JSON object
/// \param[in] formatVersion The version number of the JSON file format
//****************************************************************************************************************************************************
SpGroup Group::create(QJsonObject const &object, qint32 formatVersion) {
    return std::make_shared<Group>(object, formatVersion);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void Group::touch() {
    modificationDateTime_ = QDateTime::currentDateTime();
}


