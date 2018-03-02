/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo group class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboGroup.h"


//**********************************************************************************************************************
/// \param[in] name The name of the group
/// \param[in] description The description of the group
//**********************************************************************************************************************
ComboGroup::ComboGroup(QString const& name, QString const& description)
   : uuid_(QUuid::createUuid())
   , name_(name)
   , description_(description)
{
   creationDate_ = modificationDate_ = QDateTime::currentDateTime();
}


//**********************************************************************************************************************
/// \return true if and only if the group is valid
//**********************************************************************************************************************
bool ComboGroup::isValid() const
{
   return (!creationDate_.isNull()) && (!modificationDate_.isNull()) && (!uuid_.isNull()) && (!name_.isEmpty());
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
// 
//**********************************************************************************************************************
void ComboGroup::touch()
{
   modificationDate_ = QDateTime::currentDateTime();
}


