/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of functions related to sensitive applications


#include "stdafx.h"
#include "SensitiveApplicationManager.h"
#include "StringListEditorDialog.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>
#include "BeeftextUtils.h"


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
QStringList loadSensitiveApplicationsFromFile()
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
/// \return The only allowed instance of the class
//**********************************************************************************************************************
SensitiveApplicationManager& SensitiveApplicationManager::instance()
{
   static SensitiveApplicationManager inst;
   return inst;
}


//**********************************************************************************************************************
/// We consider that by default, applications support Ctrl+V, except a few that are individually listed
///
/// \param[in] appExeName The name of the executable, including its extension (e.g. "putty.exe")
/// \return true if and only if the application support pasting using the Ctrl+V shortcut
//**********************************************************************************************************************
bool SensitiveApplicationManager::isSensitiveApplication(QString const& appExeName) const
{
   return sensitiveApplications_.end() != std::find_if(sensitiveApplications_.begin(), sensitiveApplications_.end(),
      [&](QString const& str) -> bool
      {
         return QRegExp(str, Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(appExeName);
      });
}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
/// \return true if and only if the user validated the dialog and the list was successfully saved to file
//**********************************************************************************************************************
bool SensitiveApplicationManager::runDialog(QWidget* parent)
{
   StringListEditorDialog dlg(sensitiveApplications_, parent);
   dlg.setHeaderText(QObject::tr(R"(<html><head/><body><p>Use this dialog to list sensitive applications 
that does not work correctly with Beeftext because they do not support standard copy-paste using 
Ctrl+V.</p></body></html>)"));

   if (QDialog::Accepted != dlg.exec())
      return false;
   try
   {
      sensitiveApplications_ = dlg.stringList();
      saveSensitiveApplicationsFile(sensitiveApplications_);
      return true;
   }
   catch (xmilib::Exception const& e)
   {
      reportInternalError(QString("%1(): %2").arg(__FUNCTION__).arg(e.qwhat()));
      return false;
   }
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
SensitiveApplicationManager::SensitiveApplicationManager()
{
   try
   {
      sensitiveApplications_= loadSensitiveApplicationsFromFile();
      return;
   }
   catch (Exception const&)
   {
   }
   try
   {
      sensitiveApplications_ = kDefaultSensitiveApplications;
      saveSensitiveApplicationsFile(sensitiveApplications_);
   }
   catch (Exception const&)
   {
      globals::debugLog().addWarning("The sensitive applications file could not be saved.");
   }
}
