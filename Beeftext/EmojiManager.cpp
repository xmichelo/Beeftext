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
#include "XMiLib/Exception.h"


//**********************************************************************************************************************
/// \return The path of the emoji files
//**********************************************************************************************************************
QString emojiFilePath()
{
   QString const fileName = "emojis.json";
   QDir const appDir(qApp->applicationDirPath());
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
/// \param[in] exeFileName The name of the executable file for the application, including the extension
//**********************************************************************************************************************
bool EmojiManager::doesAppAllowEmojis(QString const& exeFileName)
{
   QStringList const exeFiles = { "slack.exe", "discord.exe" };
   return !exeFiles.contains(exeFileName, Qt::CaseInsensitive);
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
         throw xmilib::Exception("could not find emoji list file.");
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         throw xmilib::Exception("could not open emoji list file.");
      QJsonParseError parseError {};
      QJsonDocument const doc = QJsonDocument::fromJson(file.readAll(), &parseError);
      if (QJsonParseError::NoError != parseError.error)
         throw xmilib::Exception(QString("Invalid emoji file at index %1: %2").arg(parseError.offset)
            .arg(parseError.errorString()));
      if (!doc.isObject())
         throw xmilib::Exception("The emoji list file is invalid.");
      QJsonObject const rootObject = doc.object();
      
      for (QJsonObject::const_iterator it = rootObject.begin(); it != rootObject.end(); ++it)
      {
         QJsonValue const value = it.value();
         if (!value.isObject())
            throw xmilib::Exception("The emoji list file is invalid.");
         QString const emoji = value.toObject()["char"].toString(QString());
         if (emoji.isEmpty())
            throw xmilib::Exception("The emoji list file is invalid.");
         emojis_[it.key()] = emoji;
      }
   }
   catch (xmilib::Exception const& e)
   {
      globals::debugLog().addWarning(QString("Error parsing emoji list file: %1").arg(e.qwhat()));
      return false;
   }
   return true;
}


