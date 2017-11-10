/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of Combo class that associate a combo text and its substitution text
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "Combo.h"
#include <XMiLib/SystemUtils.h>


namespace {
   QString const kPropUuid = "uuid"; ///< The JSon property name for the UUID
   QString const kPropName = "name"; ///< The JSON property name for the name
   QString const kPropComboText = "comboText"; ///< The JSON property for the combo text
   QString const kPropSubstitutionText = "substitutionText"; ///< The JSON property name for the substitution text
   QString const kPropCreated = "created"; ///< The JSON property name for the created date/time
   QString const kPropLastModified = "lastModified"; ///< The JSON property name for the modification date/time
   Qt::DateFormat const kJsonExportDateFormat = Qt::ISODateWithMs; ///< The date/time export format used for JSon docs
}


using namespace xmilib;


//**********************************************************************************************************************
/// \param[in] name The display name of the combo
/// \param[in] comboText The combo text
/// \param[in] substitutionText The text that will replace the combo
//**********************************************************************************************************************
Combo::Combo(QString const& name, QString const& comboText, QString const& substitutionText)
   : uuid_(QUuid::createUuid())
   , name_(name)
   , comboText_(comboText)
   , substitutionText_(substitutionText)
{
   lastModified_ = created_ = QDateTime::currentDateTime();
}


//**********************************************************************************************************************
/// If the JSon object is not a valid combo, the constructed combo will be invalid
/// \param[in] object The object to read from
//**********************************************************************************************************************
Combo::Combo(QJsonObject const& object)
   : uuid_(QUuid(object[kPropUuid].toString()))
   , name_(object[kPropName].toString())
   , comboText_(object[kPropComboText].toString())
   , substitutionText_(object[kPropSubstitutionText].toString())
   , created_(QDateTime::fromString(object[kPropCreated].toString(), kJsonExportDateFormat))
   , lastModified_(QDateTime::fromString(object[kPropLastModified].toString(), kJsonExportDateFormat))
{      
}


//**********************************************************************************************************************
/// \return true if and only if the combo is valid
//**********************************************************************************************************************
bool Combo::isValid() const
{
   return (!created_.isNull()) && (!lastModified_.isNull()) && (!uuid_.isNull()) && (!name_.isEmpty()) 
      && (!comboText_.isEmpty()) && (!substitutionText_.isEmpty());
}


//**********************************************************************************************************************
/// \return The display name of the combo
//**********************************************************************************************************************
QString Combo::name() const
{
   return name_;
}


//**********************************************************************************************************************
/// \param[in] name The display name of the combo
//**********************************************************************************************************************
void Combo::setName(QString const& name)
{
   if (name_ != name)
   {
      name_ = name;
      this->touch();
   }
}


//**********************************************************************************************************************
/// \return The combo text
//**********************************************************************************************************************
QString Combo::comboText() const
{
   return comboText_;
}


//**********************************************************************************************************************
/// \param[in] comboText The combo text
//**********************************************************************************************************************
void Combo::setComboText(QString const& comboText)
{
   if (comboText_ != comboText)
   {
      comboText_ = comboText;
      this->touch();
   }
}


//**********************************************************************************************************************
/// \return The substitution text
//**********************************************************************************************************************
QString Combo::substitutionText() const
{
   return substitutionText_;
}


//**********************************************************************************************************************
/// \param[in] substitutionText the substitution text
//**********************************************************************************************************************
void Combo::setSubstitutionText(QString const& substitutionText)
{
   if (substitutionText_ != substitutionText)
   {
      substitutionText_ = substitutionText;
      this->touch();
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void Combo::performSubstitution()
{
   // select the typed combo by pressing Shift + Left n times (n being the number of characters in the combo)
   synthesizeKeyDown(VK_LSHIFT);
   for (qint32 i = 0; i < comboText_.size(); ++i)
   {
      synthesizeKeyDown(VK_LEFT);
      synthesizeKeyUp(VK_LEFT);
   }
   synthesizeKeyUp(VK_LSHIFT);

   // put the substitution text in the clipboard
   qApp->clipboard()->setText(substitutionText_);

   // send a paste command (Ctrl+V)
   synthesizeKeyDown(VK_LCONTROL);
   synthesizeKeyDown('V');
   synthesizeKeyUp('V');
   synthesizeKeyUp(VK_LCONTROL);
}


//**********************************************************************************************************************
/// \return A JSon object representing this Combo instance
//**********************************************************************************************************************
QJsonObject Combo::toJsonObject()
{
   QJsonObject result;
   result.insert(kPropUuid, uuid_.toString());
   result.insert(kPropName, name_);
   result.insert(kPropComboText, comboText_);
   result.insert(kPropSubstitutionText, substitutionText_);
   result.insert(kPropCreated, created_.toString(kJsonExportDateFormat));
   result.insert(kPropLastModified, lastModified_.toString(kJsonExportDateFormat));
   return result;
}


//**********************************************************************************************************************
/// This function is named after the UNIX touch command.
//**********************************************************************************************************************
void Combo::touch()
{
   lastModified_ = QDateTime::currentDateTime();
}


