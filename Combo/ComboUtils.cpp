/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of utility functions related to combos
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboUtils.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {

   QString const kKeyGroupName = "groupName"; ///< The JSon Key for the combo group
   QString const kDefaultGroupName = "Default"; ///< The default combo group name
   QString const kKeyComboList = "comboList"; ///< The JSon key for the combo list
   qint32 const kJsonComboListFileFormatVersionNumber = 1; ///< The version number for the combo list file format
   QString const kKeyFileFormatVersion = "fileFormatVersion"; ///< The JSon key for the file format version
   QString const kKeyCombos = "combos"; ///< The JSon key for combos
   QString const kComboGroup = "comboGroup"; ///< The JSon key for combo groups

}


void _parseJsonComboGroup(QJsonObject const& object, VecSPCombo& outComboList); ///< Parse a combo group from a JSon object


//**********************************************************************************************************************
/// \param[in] comboList The list of combo
/// \return A JSon document representing the list of combo
//**********************************************************************************************************************
void comboListToJSonDocument(VecSPCombo const& comboList, QJsonDocument& outDoc)
{
   // In a future release, we will add support for grouping combos into group. For now, in JSon export we put 
   // everything in a default hidden category.
   QJsonObject rootObject;
   rootObject.insert(kKeyFileFormatVersion, kJsonComboListFileFormatVersionNumber);

   QJsonArray comboListArray;
   QJsonObject comboGroup;
   comboGroup.insert(kKeyGroupName, kDefaultGroupName);
   QJsonArray comboArray;
   for (SPCombo const& combo : comboList)
      comboArray.append(combo->toJsonObject());
   comboGroup.insert(kKeyCombos, comboArray);
   comboListArray.append(comboGroup);
   rootObject.insert(kKeyComboList, comboListArray);
   outDoc = QJsonDocument(rootObject);
}


//**********************************************************************************************************************
/// \param[in] doc The JSon document to read from
/// \param[out] outCombo The parsed combo list
/// \return true if and only if the parsing succeeded. If the function returns false, the content of outCombo is 
/// undetermined.
//**********************************************************************************************************************
bool jsonDocumentToComboList(QJsonDocument const& doc, VecSPCombo& outComboList)
{
   DebugLog& debugLog = globals::debugLog();
   try
   {
      outComboList.clear();
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

      // parse the combo list
      QJsonValue const comboListValue = rootObject[kKeyComboList];
      if (!comboListValue.isArray())
         throw xmilib::Exception(QObject::tr("The combo list array is invalid."));
      for (QJsonValue const& comboGroupValue: comboListValue.toArray())
      {
         if (!comboGroupValue.isObject())
            throw xmilib::Exception(QObject::tr("The combo list array contains an invalid combo."));
         _parseJsonComboGroup(comboGroupValue.toObject(), outComboList);
      }
      return true;
   }
   catch (xmilib::Exception const& e)
   {
   	debugLog.addError(e.qwhat());
      return false;
   }   
}


//**********************************************************************************************************************
/// In case of failure, this function throws an xmilib::Exception
///
/// \param[in] object The JSon object containing the group
/// \param[out] outComboList the list of combo into which the parsed combos will be added
//**********************************************************************************************************************
void _parseJsonComboGroup(QJsonObject const& object, VecSPCombo& outComboList)
{
   // In the initial version of the app, only a single default group is supported
   QJsonValue combosValue = object[kKeyCombos];
   if (!combosValue.isArray())
      throw xmilib::Exception(QObject::tr("The combo list array contains an invalid combo group."));
   for (QJsonValue const& comboValue: combosValue.toArray())
   {
      if (!comboValue.isObject())
         throw xmilib::Exception(QObject::tr("The combo list array contains an invalid combo."));
      SPCombo combo = std::make_shared<Combo>(comboValue.toObject());
      if (!combo->isValid())
         throw xmilib::Exception(QObject::tr("The combo list array contains an invalid combo."));
      outComboList.push_back(combo);
   }
}
