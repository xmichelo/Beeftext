/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of functions related to sensitive applications


#include "stdafx.h"
#include "SensitiveApplicationManager.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include <XMiLib/String/StringListEditorDialog.h>
#include <XMiLib/SystemUtils.h>
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QString const kPropSensitiveApplicationList = "sensitiveApplications"; ///< The JSON key for sensitive applications
QStringList kBuiltInApps = { "mintty.exe", "putty.exe", "powershell.exe", "kitty.exe",
"kitty_portable.exe", "ConEmu*.exe" }; ///< The built-in slist executables name that support only pasting using the Shift+Insert shortcut. Wildcards are supported


//**********************************************************************************************************************
/// \brief Save the list of sensitive applications.
///
/// \param[in] list The list of sensitive application exe file names.
/// \return true if and only if the list was saved.
//**********************************************************************************************************************
bool saveSensitiveApplicationsFile(QStringList const& list)
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
      return true;
   }
   catch (Exception const& e)
   {
      globals::debugLog().addError(QString("%1(): %2").arg(__FUNCTION__).arg(e.qwhat()));
      return false;
   }
}


//**********************************************************************************************************************
/// If the file does not exist, an empty string list is returned.
///
/// \param[out] outApps The list of sensitive applications read from the JSON file.
/// \return true if and only if the file was loaded or there was no file.
//**********************************************************************************************************************
bool loadSensitiveApplicationsFromFile(QStringList &outApps)
{
   try
   {
      outApps.clear();
      QFile file(globals::sensitiveApplicationsFilePath());
      if (!file.exists())
         return true; // We do not consider the absence of file as an error
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
      for (QJsonValueRef const& value: array)
      {
         if (!value.isString())
            throw Exception("invalid array content.");
         outApps.push_back(value.toString());
      }
      return true;
   }
   catch (Exception const& e)
   {
      globals::debugLog().addError(QString("%1(): %2").arg(__FUNCTION__).arg(e.qwhat()));
      return false;
   }
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
   std::function<bool(QString const&)> const predicate = [&](QString const& str) -> bool
   {
      return QRegExp(str, Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(appExeName);
   };
   return (kBuiltInApps.end() != std::find_if(kBuiltInApps.begin(), kBuiltInApps.end(), predicate)) ||
      (sensitiveApps_.end() != std::find_if(sensitiveApps_.begin(), sensitiveApps_.end(), predicate));
}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
/// \return true if and only if the user validated the dialog and the list was successfully saved to file
//**********************************************************************************************************************
bool SensitiveApplicationManager::runDialog(QWidget* parent)
{
   StringListEditorDialog dlg(sensitiveApps_, parent);
   dlg.setHeaderText(QObject::tr(R"(<html><head/><body><p>Use this dialog to list sensitive applications 
that does not work correctly with Beeftext because they do not support standard copy-paste using 
Ctrl+V.</p></body></html>)"));

   QPushButton* button = new QPushButton(QObject::tr("&Reset"), &dlg);
   QObject::connect(button, &QPushButton::clicked, [&dlg]()
   {
      if (QMessageBox::Yes == QMessageBox::question(&dlg, QObject::tr("Reset List"), 
         QObject::tr("Are you sure you want to reset the list of sensitive applications?"),
         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
         dlg.setStringList(QStringList());
   });
   dlg.addCustomButton(button);
   if (QDialog::Accepted != dlg.exec())
      return false;
   sensitiveApps_ = dlg.stringList();
   bool const result = saveSensitiveApplicationsFile(sensitiveApps_);
   if (!result)
      QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("The sensitive application file could not "
         "be saved."));
   return result;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
SensitiveApplicationManager::SensitiveApplicationManager()
{
   if (!loadSensitiveApplicationsFromFile(sensitiveApps_))
      sensitiveApps_;
}
