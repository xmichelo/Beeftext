/// \file
/// \author 
///
/// \brief Implementation of function related to the last use file
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "LastUseFile.h"
#include "PreferencesManager.h"
#include "ComboManager.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QString const kLastUseFileName = "lastUse.json";
///< The name of the file used to store the last use date/time of combos.
QString const kPropFileFormatVersion = "fileFormatVersion"; ///< The property name for the file format version.
QString const kPropDateTimes = "dateTimes"; ///< The property name for the date/time.
QString const kPropUuid = "uuid"; ///< The property name for uuid.
QString const kPropDateTime = "dateTime"; ///< The property name for date/time.
qint32 const kFileFormatVersion = 1; ///< The file format version.


//**********************************************************************************************************************
/// \param[in] comboList The combo list.
/// \param[in] object The JSON object.
//**********************************************************************************************************************
void parseDateTimeObject(ComboList& comboList, QJsonObject const& object)
{
   QUuid const uuid = QUuid::fromString(object[kPropUuid].toString(QString()));
   if (uuid.isNull())
      return;
   ComboList::iterator const it = comboList.findByUuid(uuid);
   if (it == comboList.end())
      return;
   SpCombo const combo = *it;
   QString const dateStr = object[kPropDateTime].toString(QString());
   if (dateStr.isEmpty())
      return;
   combo->setLastUseDateTime(QDateTime::fromString(dateStr, constants::kJsonExportDateFormat));
}


} // namespace


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void loadLastUseDateTimes(ComboList& comboList)
{
   try
   {
      QString const invalidFileStr = "The last use file is invalid.";
      QFile file = QDir(PreferencesManager::instance().comboListFolderPath()).absoluteFilePath(kLastUseFileName);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         throw Exception("Could not save last use date/time file.");
      QJsonParseError jsonError {};
      QJsonDocument const doc = QJsonDocument::fromJson(file.readAll(), &jsonError);
      if (jsonError.error != QJsonParseError::NoError)
         throw Exception("The last use date/time file is not a valid JSON document");
      QJsonObject const rootObject = doc.object();
      qint32 const version = rootObject[kPropFileFormatVersion].toInt();
      if (version > kFileFormatVersion)
         throw Exception("The last use file format has been created with a newer version of the application.");
      QJsonValue const dateTimesValue = rootObject[kPropDateTimes];
      if (!dateTimesValue.isArray())
         throw Exception(invalidFileStr);
      for (QJsonValueRef const value: dateTimesValue.toArray())
      {
         if (!value.isObject())
            throw Exception(invalidFileStr);
         parseDateTimeObject(comboList, value.toObject());
      }
   }
   catch (xmilib::Exception const& e)
   {
      globals::debugLog().addError(e.qwhat());
   }
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void saveLastUseDateTimes(ComboList const& comboList)
{
   try
   {
      QJsonObject rootObject;
      rootObject.insert(kPropFileFormatVersion, kFileFormatVersion);
      QJsonArray dateTimes;
      for (SpCombo const& combo: comboList)
      {
         if (!combo)
            continue;
         QJsonObject object;
         object.insert(kPropUuid, combo->uuid().toString());
         object.insert(kPropDateTime, combo->lastUseDateTime().toString(constants::kJsonExportDateFormat));
         dateTimes.append(object);
      }
      rootObject.insert(kPropDateTimes, dateTimes);

      QFile file = QDir(PreferencesManager::instance().comboListFolderPath()).absoluteFilePath(kLastUseFileName);
      if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         throw Exception("Could not save last use date/time file.");

      QByteArray const data = QJsonDocument(rootObject).toJson();
      if (data.size() != file.write(data))
         throw Exception("An error occurred while writing the last use date/time file.");
   }
   catch (xmilib::Exception const& e)
   {
      globals::debugLog().addError(e.qwhat());
   }
}
