/// \file
/// \author 
///
/// \brief Implementation of function related to the emoji last use file.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "EmojiLastUseFile.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QString const kEmojiLastUseFileName = "emojiLastUse.json"; ///< The name of the file used to store the emojis last use date/time of combos.
QString const kPropFileFormatVersion = "fileFormatVersion"; ///< The property name for the file format version.
QString const kPropDateTimes = "dateTimes"; ///< The property name for the dates/times.
QString const kPropShortcode = "shortcode"; ///< The property name for shortcodes.
QString const kPropDateTime = "dateTime"; ///< The property name for date/time.
qint32 constexpr kFileFormatVersion = 1; ///< The file format version.


//****************************************************************************************************************************************************
/// \param[in] emojiList The emoji list.
/// \param[in] object The JSON object.
//****************************************************************************************************************************************************
void parseDateTimeObject(EmojiList const &emojiList, QJsonObject const &object) {
    QString const shortcode = object[kPropShortcode].toString(QString());
    if (shortcode.isEmpty())
        return;
    SpEmoji const emoji = emojiList.find(shortcode);
    if (!emoji)
        return;
    QString const dateStr = object[kPropDateTime].toString(QString());
    if (dateStr.isEmpty())
        return;
    emoji->setlastUseDateTime(QDateTime::fromString(dateStr, constants::kJsonExportDateFormat));
}


}


//****************************************************************************************************************************************************
/// \param[in,out] emojiList The emoji list.
//****************************************************************************************************************************************************
void loadEmojiLastUseDateTimes(EmojiList const &emojiList) {
    try {
        QString const invalidFileStr = "The emoji last use file is invalid.";
        QFile file = QDir(globals::appDataDir()).absoluteFilePath(kEmojiLastUseFileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw Exception("Could not save the emoji last use date/time file.");
        QJsonParseError jsonError {};
        QJsonDocument const doc = QJsonDocument::fromJson(file.readAll(), &jsonError);
        if (jsonError.error != QJsonParseError::NoError)
            throw Exception("The emoji last use date/time file is not a valid JSON document");
        QJsonObject const rootObject = doc.object();
        qint32 const version = rootObject[kPropFileFormatVersion].toInt();
        if (version > kFileFormatVersion)
            throw Exception("The emoji last use file format has been created with a newer version of the application.");
        QJsonValue const dateTimesValue = rootObject[kPropDateTimes];
        if (!dateTimesValue.isArray())
            throw Exception(invalidFileStr);
        for (QJsonValueRef const value: dateTimesValue.toArray()) {
            if (!value.isObject())
                throw Exception(invalidFileStr);
            parseDateTimeObject(emojiList, value.toObject());
        }
    }
    catch (Exception const &e) {
        globals::debugLog().addError(e.qwhat());
    }
}


//****************************************************************************************************************************************************
/// \param[in] emojiList The emoji list.
//****************************************************************************************************************************************************
void saveEmojiLastUseDateTimes(EmojiList const &emojiList) {
    try {
        if (emojiList.isEmpty())
            return;
        QJsonObject rootObject;
        rootObject.insert(kPropFileFormatVersion, kFileFormatVersion);
        QJsonArray dateTimes;
        for (SpEmoji const &emoji: emojiList) {
            if (!emoji)
                continue;
            QJsonObject object;
            object.insert(kPropShortcode, emoji->shortcode());
            object.insert(kPropDateTime, emoji->lastUseDateTime().toString(constants::kJsonExportDateFormat));
            dateTimes.append(object);
        }
        rootObject.insert(kPropDateTimes, dateTimes);

        QFile file = QDir(globals::appDataDir()).absoluteFilePath(kEmojiLastUseFileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            throw Exception("Could not save the emoji last use date/time file.");

        QByteArray const data = QJsonDocument(rootObject).toJson();
        if (data.size() != file.write(data))
            throw Exception("An error occurred while writing the emoji last use date/time file.");
    }
    catch (Exception const &e) {
        globals::debugLog().addError(e.qwhat());
    }
}
