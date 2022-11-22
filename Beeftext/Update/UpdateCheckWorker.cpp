/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of worker responsible for checking for updates
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "UpdateCheckWorker.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


namespace {
#ifdef NDEBUG
QString const kVersionFileUrl = "https://beeftext.org/latestVersionInfo.json"; ///< The URL of the file containing the version number of Beeftext
#else
QString const kVersionFileUrl = "https://beta.beeftext.org/latestVersionInfo.json"; ///< The URL of the file containing the version number of Beeftext
#endif
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent object of the instance
//****************************************************************************************************************************************************
UpdateCheckWorker::UpdateCheckWorker(QObject *parent, xmilib::VersionNumber const &skipVersionNumber)
    : QObject(parent)
    , skipVersionNumber_(skipVersionNumber) {

}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateCheckWorker::run() {
    this->performUpdateCheck();
    emit finished(); // note that finished is emitted even in case of error
}


//****************************************************************************************************************************************************
///
//****************************************************************************************************************************************************
void UpdateCheckWorker::performUpdateCheck() {
    try {
        xmilib::DebugLog &log = globals::debugLog();
        log.addInfo(QString("Update check started. Installed version is Beeftext %1.")
            .arg(constants::kVersionNumber.toString()));
        QByteArray const jsonData = this->downloadLatestVersionInformation();
        SpLatestVersionInfo const latestVersionInfo = this->parseJsonData(jsonData);
        log.addInfo(QString("Downloaded latest version information. Latest version is Beeftext %1.")
            .arg(latestVersionInfo->versionNumber().toString()));
        bool skipped = false;
        if (isNewVersionAvailable(latestVersionInfo, &skipped)) {
            log.addInfo("A new version is available.");
            emit updateIsAvailable(latestVersionInfo);
        } else {
            if (skipped)
                log.addInfo("A new version was available, but the user had previously decided to skip it.");
            else
                log.addInfo("No new version is available.");
            emit noUpdateIsAvailable();
        }
        log.addInfo("Update check ended.");
    }
    catch (xmilib::Exception const &e) {
        emit error(e.qwhat());
    }
}


//****************************************************************************************************************************************************
/// \return The downloaded data
//****************************************************************************************************************************************************
QByteArray UpdateCheckWorker::downloadLatestVersionInformation() const {
    QNetworkAccessManager nam;
    QEventLoop loop;
    QNetworkRequest const request(kVersionFileUrl);
    QNetworkReply *reply = nam.get(request);
    QString errorMessage;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::errorOccurred, [&](QNetworkReply::NetworkError code) {
        errorMessage = QString("Error %1: %2").arg(code).arg(reply->errorString());
    });
    loop.exec();
    if (!errorMessage.isEmpty())
        throw xmilib::Exception(QString("Could not retrieve version information file from the Beeftext website: %1")
            .arg(errorMessage));
    QByteArray result = reply->readAll();
    reply->deleteLater();
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] jsonData the JSON data
/// \return A shared pointer to the parsed data. 
//****************************************************************************************************************************************************
SpLatestVersionInfo UpdateCheckWorker::parseJsonData(QString const &jsonData) const {
    QString errMsg;
    QJsonParseError error {};
    QJsonDocument const doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError)
        throw xmilib::Exception(QString("Could not retrieve version information. The downloaded JSON file is "
                                        "invalid: %1").arg(error.errorString()));
    SpLatestVersionInfo result = std::make_shared<LatestVersionInfo>(doc.object());
    if (!result->isValid())
        throw xmilib::Exception(QString("Could not retrieve version information. The downloaded JSON file is "
                                        "invalid."));
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information.
/// \param[out] outSkipped If not null, and the function returns false, on exit the value pointed will be true
/// if the update was skipped by the user.
/// \return true if and only if an update is available.
//****************************************************************************************************************************************************
bool UpdateCheckWorker::isNewVersionAvailable(SpLatestVersionInfo const &latestVersionInfo, bool *outSkipped) const {
    if (outSkipped)
        *outSkipped = false;
    if (!latestVersionInfo)
        throw xmilib::Exception("Could not check for new version: the retrieved latest version information is "
                                "null.");
    if (skipVersionNumber_.isValid() && latestVersionInfo->versionNumber().isValid()
        && (skipVersionNumber_ == latestVersionInfo->versionNumber())) {
        if (outSkipped)
            *outSkipped = true;
        return false;

    }

    return latestVersionInfo->versionNumber() > constants::kVersionNumber;
}


