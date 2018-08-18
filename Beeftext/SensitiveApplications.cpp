/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of functions related to sensitive applications


#include "stdafx.h"
#include "SensitiveApplications.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QString const kPropSensitiveApplicationList = "sensitiveApplications"; ///< The JSON key for sensitive applications
QStringList kDefaultSensitiveApplications = { "mintty.exe", "putty.exe", "powershell.exe", "kitty.exe",
"kitty_portable.exe", "ConEmu*.exe" }; ///< The list of application executables name that support only pasting using the Shift+Insert shortcut. Wildcards are supported


//**********************************************************************************************************************
/// \brief Save the list of sensitive applications.
///
/// If the file could not be saved this function will throw an Exception.
///
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
         throw Exception("cannot open file.");
      QByteArray const data = doc.toJson();
      if (data.size() != file.write(data))
         throw Exception("error writing to file.");
   }
   catch (Exception const& e)
   {
      globals::debugLog().addWarning(QString("The sensitive application list file could not be saved: %1")
         .arg(e.qwhat()));
      throw;
   }
}


//**********************************************************************************************************************
/// If the file does not exist , the function will create a default one.
/// If the file is invalid the function will throw an Exception
///
/// \return The list of sensitive applications read from the JSON file
/// \return The default list of sensitive applications if the file did not exist
//**********************************************************************************************************************
QStringList loadSensitiveApplicationsFile()
{
   QFile file(globals::sensitiveApplicationsFilePath());
   if (!file.exists())
   {
      saveSensitiveApplicationsFile(kDefaultSensitiveApplications);
      return kDefaultSensitiveApplications;
   }
   if (!file.open(QIODevice::ReadOnly))
      throw Exception("cannot open file.");
   QJsonDocument const doc(QJsonDocument::fromJson(file.readAll()));
   if (doc.isNull())
      throw Exception("invalid JSON document.");
   if (!doc.isObject())
      throw Exception("the root element is not an object.");
   QJsonObject const rootObject = doc.object();
   QJsonValue const arrayValue = rootObject.value(kPropSensitiveApplicationList);
   if (arrayValue.isNull() || !arrayValue.isArray())
      throw Exception("invalid top level object.");
   QJsonArray array = arrayValue.toArray();
   QStringList result;
   for (QJsonValueRef const& value: array)
   {
      if (!value.isString())
         throw Exception("invalid array content.");
      result.push_back(value.toString());
   }
   return result;
}


}


//**********************************************************************************************************************
/// \return The list of sensitive applications exe file names
//**********************************************************************************************************************
QStringList sensitiveApplications()
{
   try
   {
      static QStringList result = loadSensitiveApplicationsFile();
      return result;
   }
   catch (Exception const& e)
   {
   }
   if (QMessageBox::Yes == QMessageBox::question(nullptr, QObject::tr("Invalid file"), QObject::tr("The file "
      "containing the list of sensitive application is invalid. Do you want to replace it by the default one."),
      QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
   {
      try
      {
         saveSensitiveApplicationsFile(kDefaultSensitiveApplications);
      }
      catch (Exception const& e)
      {
         QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("The sensitive applications file "
            "could not be saved."));
      }
   }
   return kDefaultSensitiveApplications;
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
