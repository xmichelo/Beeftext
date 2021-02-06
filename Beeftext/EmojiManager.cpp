/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of Emoji manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "EmojiManager.h"
#include "BeeftextGlobals.h"
#include <XMiLib/String/StringListEditorDialog.h>
#include <XMiLib/String/StringUtils.h>
#include <XMiLib/Exception.h>


using namespace xmilib;




namespace {


QStringList const kBuiltInExcludedApps { };


//**********************************************************************************************************************
/// \brief Load the user-provided list of excluded applications from file.
/// 
/// \param[in] exceptions The list of exceptions.
/// \return true if and only if the user exceptions file could not be loaded or do not exists.
//**********************************************************************************************************************
bool saveExcludedApplicationToFile(QStringList const& exceptions)
{
   QString errorMsg;
   bool const result = saveStringListToJsonFile(exceptions, globals::emojiExcludedAppsFilePath(), &errorMsg);
   if (!result)
      globals::debugLog().addError(QString("%1(): %2").arg(__FUNCTION__).arg(errorMsg));
   return result;
}


//**********************************************************************************************************************
/// \brief Load the user-provided emoji exceptions
/// \param[out] outExceptions The list of exceptions loaded from file. If the file does not exist the list is empty.
/// 
/// \return true if and only if the user exceptions file could not be loaded or do not exists
//**********************************************************************************************************************
bool loadExcludedApplicationFromFile(QStringList& outExceptions)
{
   QString errorMsg;
   bool const result = loadStringListFromJsonFile(globals::emojiExcludedAppsFilePath(), outExceptions, &errorMsg);
   if (!result)
   {
      globals::debugLog().addError(QString("%1(): %2").arg(__FUNCTION__).arg(errorMsg));
      outExceptions.clear();
   }
   return result;
}


}


//**********************************************************************************************************************
/// \return The path of the emoji files
//**********************************************************************************************************************
QString emojiFilePath()
{
   QString const fileName = "emojis.json";
   QDir const appDir(qApp->applicationDirPath());
   qDebug() << QString("appDir: %1").arg(appDir.absolutePath());
   QString filePath = appDir.absoluteFilePath("emojis/" + fileName);
   if (QFile(filePath).exists())
      return filePath;
   filePath = QDir(appDir.absoluteFilePath("../../../Submodules/emojilib/" + fileName))
      .canonicalPath();
   return QFile(filePath).exists()? filePath : QString();
}


//**********************************************************************************************************************
/// \return The only allowed instance of the emoji manager
//**********************************************************************************************************************
EmojiManager& EmojiManager::instance()
{
   static EmojiManager instance;
   return instance;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void EmojiManager::loadEmojis()
{
   this->unloadEmojis();
   QString const filePath = emojiFilePath();
   if (filePath.isEmpty())
   {
      globals::debugLog().addWarning("Could not find the emoji list file.");
      return;
   }
   this->load(filePath);

}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void EmojiManager::unloadEmojis()
{
   emojis_.clear();
}


//**********************************************************************************************************************
/// \param[in] keyword The keyword
/// \return The associated emoji
/// \return A Null string if no emoji is associated with this keyword
//**********************************************************************************************************************
QString EmojiManager::emoji(QString const& keyword) const
{
   return emojis_.contains(keyword) ? emojis_[keyword] : QString();
}


//**********************************************************************************************************************
/// \param[in] appExeName The application executable name.
//**********************************************************************************************************************
bool EmojiManager::isExcludedApplication(QString const& appExeName) const
{
   std::function<bool(QString const&)> const predicate = [&](QString const& str) -> bool
   {
      return QRegularExpression(QRegularExpression::wildcardToRegularExpression(str), 
         QRegularExpression::CaseInsensitiveOption).match(appExeName).hasMatch();
   };
   return (kBuiltInExcludedApps.end() != std::find_if(kBuiltInExcludedApps.begin(), kBuiltInExcludedApps.end(), 
      predicate)) || (excludedApps_.end() != std::find_if(excludedApps_.begin(), excludedApps_.end(), predicate));
}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog.
/// \return true if and only if the user validated the dialog and the list was successfully saved to file.
//**********************************************************************************************************************
bool EmojiManager::runDialog(QWidget* parent)
{
   StringListEditorDialog dlg(excludedApps_, parent);
   dlg.setHeaderText(QObject::tr("<html><head/><body><p>Use this dialog to list applications "
      "in which emoji substitution should be disabled.</p><p>List applications using their process name (e.g, "
      "notepad.exe). Wildcards are accepted.</p></body></html>"));
   QPushButton* button = new QPushButton(QObject::tr("&Reset"), &dlg);
   QObject::connect(button, &QPushButton::clicked, [&dlg]()
   {
      if (QMessageBox::Yes == QMessageBox::question(&dlg, QObject::tr("Reset List"), 
         QObject::tr("Are you sure you want to reset the list of excluded applications?"),
         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
         dlg.setStringList(QStringList());
   });
   dlg.addCustomButton(button);
   if (QDialog::Accepted != dlg.exec())
      return false;
   excludedApps_ = dlg.stringList();
   bool const result = saveExcludedApplicationToFile(excludedApps_);
   if (!result)
      QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("The emoji excluded applications file could "
         "not be saved."));
   return result;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
EmojiManager::EmojiManager()
{
   if (!loadExcludedApplicationFromFile(excludedApps_))
   {
      excludedApps_.clear();
      saveExcludedApplicationToFile(excludedApps_);
   }
}


//**********************************************************************************************************************
/// \param[in] path The path of the file to load
/// \return true if and only if the file was successfully loaded
//**********************************************************************************************************************
bool EmojiManager::load(QString const& path)
{
   try
   {
      QFile file(path);
      if (!file.exists())
         throw Exception("could not find emoji list file.");
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         throw Exception("could not open emoji list file.");
      QJsonParseError parseError {};
      QJsonDocument const doc = QJsonDocument::fromJson(file.readAll(), &parseError);
      if (QJsonParseError::NoError != parseError.error)
         throw Exception(QString("Invalid emoji file at index %1: %2").arg(parseError.offset)
            .arg(parseError.errorString()));
      if (!doc.isObject())
         throw Exception("The emoji list file is invalid.");
      QJsonObject const rootObject = doc.object();
      
      for (QJsonObject::const_iterator it = rootObject.begin(); it != rootObject.end(); ++it)
      {
         QJsonValue const value = it.value();
         if (!value.isObject())
            throw Exception("The emoji list file is invalid.");
         QString const emoji = value.toObject()["char"].toString(QString());
         if (emoji.isEmpty())
            throw Exception("The emoji list file is invalid.");
         emojis_[it.key()] = emoji;
      }
   }
   catch (Exception const& e)
   {
      globals::debugLog().addWarning(QString("Error parsing emoji list file: %1").arg(e.qwhat()));
      return false;
   }
   return true;
}


