/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of functions related to sensitive applications


#include "stdafx.h"
#include "SensitiveApplications.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {


QString const kPropSensitiveApplicationList = "sensitiveApplications"; ///< The JSON key for sensitive applications
QStringList kDefaultSensitiveApplications = { "mintty.exe", "putty.exe", "powershell.exe", "kitty.exe",
"kitty_portable.exe", "ConEmu*.exe" }; ///< The list of application executables name that support only pasting using the Shift+Insert shortcut. Wildcards are supported


//**********************************************************************************************************************
/// \param[in] list The list of sensitive application exe file names
//**********************************************************************************************************************
void saveSensitiveApplicationsFile(QStringList const& list)
{
   try
   {
      QJsonObject rootObject;
      QJsonArray array;
      for (QString const& exeName: list)
         array.append(exeName);
      rootObject.insert(kPropSensitiveApplicationList, array);
      QJsonDocument doc(rootObject);
      QFile file(globals::sensitiveApplicationsFilePath());
      if (!file.open(QIODevice::WriteOnly))
         throw xmilib::Exception("cannot open file.");
      QByteArray const data = doc.toJson();
      if (data.size() != file.write(data))
         throw xmilib::Exception("error writing to file.");
   }
   catch (xmilib::Exception const& e)
   {
      globals::debugLog().addWarning(QString("The sensitive application list file could not be saved: %1")
         .arg(e.qwhat()));
   }
}


//**********************************************************************************************************************
/// If the file does not exist or is invalid, this function will attempt to create a file with default values and will
/// return it.
///
/// \return The list of sensitive applications read from the JSON file
/// \return The default list of sensitive applications 
//**********************************************************************************************************************
QStringList loadSensitiveApplicationsFile()
{
   try
   {
      QFile file(globals::sensitiveApplicationsFilePath());
      if (!file.exists())
      {
         saveSensitiveApplicationsFile(kDefaultSensitiveApplications);
         return kDefaultSensitiveApplications;
      }
      if (!file.open(QIODevice::ReadOnly))
         throw xmilib::Exception("cannot open file.");
      QJsonDocument const doc(QJsonDocument::fromJson(file.readAll()));
      if (doc.isNull())
         throw xmilib::Exception("invalid JSON document.");
      if (!doc.isObject())
         throw xmilib::Exception("the root element is not an object.");
      QJsonObject const rootObject = doc.object();
      QJsonValue const arrayValue = rootObject.value(kPropSensitiveApplicationList);
      if (arrayValue.isNull() || !arrayValue.isArray())
         throw xmilib::Exception("invalid top level object.");
      QJsonArray array = arrayValue.toArray();
      QStringList result;
      for (QJsonValueRef const& value: array)
      {
         if (!value.isString())
            throw xmilib::Exception("invalid array content.");
         result.push_back(value.toString());
      }
      return result;
   }
   catch (xmilib::Exception const& e)
   {
      saveSensitiveApplicationsFile(kDefaultSensitiveApplications);
      return kDefaultSensitiveApplications;
   }
}


//**********************************************************************************************************************
/// \brief Retrieve the list of sensitive applications executable file names
//**********************************************************************************************************************
QStringList sensitiveApplications()
{
   static QStringList result = loadSensitiveApplicationsFile();
   return result;
}


}


//**********************************************************************************************************************
/// We consider that by default, applications support Ctrl+V, except a few that are individually listed
///
/// \param[in] appExeName The name of the executable, including its extension (e.g. "putty.exe")
/// \return true if and only if the application support pasting using the Ctrl+V shortcut
//**********************************************************************************************************************
bool isSensitiveApplication(QString const& appExeName)
{
   QStringList const sensitiveApps = sensitiveApplications();
   return sensitiveApps.end() != std::find_if(sensitiveApps.begin(), sensitiveApps.end(), 
      [&](QString const& str) -> bool 
      { return QRegExp(str, Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(appExeName); });
}
