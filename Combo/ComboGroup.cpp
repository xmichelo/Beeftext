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
/// \param[in] name The name of the combo group
/// \param[in] description The description of the combo group
//**********************************************************************************************************************
ComboGroup::ComboGroup(QString const& name, QString const& description)
   :  name_(name)
   , description_(description)
{
}


//**********************************************************************************************************************
/// A group is considered valid if its name is not empty
///
/// \return true if and only if the group is valid
//**********************************************************************************************************************
bool ComboGroup::isValid() const
{
   return !name_.isEmpty();
}


//**********************************************************************************************************************
/// \return The name of the combo group
//**********************************************************************************************************************
QString ComboGroup::name() const
{
   return name_;
}


//**********************************************************************************************************************
/// \param[in] name The name of the combo group
//**********************************************************************************************************************
void ComboGroup::setName(QString const& name)
{
   name_ = name;
}


//**********************************************************************************************************************
/// \return The description of the combo group
//**********************************************************************************************************************
QString ComboGroup::description() const
{
   return description_;
}


//**********************************************************************************************************************
/// \param[in] description The description of the combo group
//**********************************************************************************************************************
void ComboGroup::setDescription(QString const& description)
{
   description_ = description;
}


