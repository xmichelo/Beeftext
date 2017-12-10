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


namespace {
   QString const kVersionFileUrl = "https://beeftext.org/latestVersionInfo.json"; ///< The URL of the file containing the version number of Beeftext
}


//**********************************************************************************************************************
/// \param[in] parent The parent object of the instance
//**********************************************************************************************************************
UpdateCheckWorker::UpdateCheckWorker(QObject* parent)
   : QObject(parent)
{

}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateCheckWorker::run()
{
   this->performUpdateCheck();
   emit finished();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateCheckWorker::performUpdateCheck()
{
   QByteArray jsonData = this->downloadLatestVersionInformation();
   if (jsonData.isEmpty())
      return;
   SPLatestVersionInfo latestVersionInfo = this->parseJsonData(jsonData);
   if ((!latestVersionInfo) || (!latestVersionInfo->isValid()))
      return;
   if (this->isNewVersionAvailable(latestVersionInfo))
      emit newVersionIsAvailable(latestVersionInfo);
}


//**********************************************************************************************************************
/// \return The downloaded data
//**********************************************************************************************************************
QByteArray UpdateCheckWorker::downloadLatestVersionInformation() const
{
   QNetworkAccessManager nam;
   QEventLoop loop;
   QNetworkRequest request(kVersionFileUrl);
   QNetworkReply* reply = nam.get(request);
   QString errorMessage;
   connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
   connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
      [&](QNetworkReply::NetworkError code) {
      errorMessage = QString("Error %1: %2").arg(code).arg(reply->errorString()); });
   loop.exec();
   if (!errorMessage.isEmpty())
   {
      globals::debugLog().addError(QString("Could not retrieve version information file from the Beeftext "
         "website: %1").arg(errorMessage));
      return QByteArray();
   }
   QByteArray const result = reply->readAll();
   reply->deleteLater();
   return result;
}


//**********************************************************************************************************************
/// \param[in] jsonData the JSON data
/// \return A shared pointer to the parsed 
//**********************************************************************************************************************
SPLatestVersionInfo UpdateCheckWorker::parseJsonData(QString const& jsonData) const
{
   QString errMsg;
   QJsonParseError error;
   QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);
   if (error.error != QJsonParseError::NoError)
   {
      globals::debugLog().addError(QString("Could not retrieve version information. The downloaded JSON file is "
         "invalid: %1").arg(error.errorString()));
      return SPLatestVersionInfo();
   }
   SPLatestVersionInfo result = std::make_shared<LatestVersionInfo>(doc.object());
   if (result->isValid())
      return result;
   globals::debugLog().addError(QString("Could not retrieve version information. The downloaded JSON file is "
      "invalid."));
   return SPLatestVersionInfo();
}


//**********************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
//**********************************************************************************************************************
bool UpdateCheckWorker::isNewVersionAvailable(SPLatestVersionInfo latestVersionInfo) const
{
   if (!latestVersionInfo)
   {
      globals::debugLog().addError("Could not check for new version: the retrieved latest version information is "
         "null.");
      return false;
   }
   qint32 const major = latestVersionInfo->versionMajor();
   qint32 const minor = latestVersionInfo->versionMinor();
   bool const hasNewVersion = (major > constants::kVersionMajor)
      || ((major == constants::kVersionMajor) && (minor > constants::kVersionMinor));
   if (hasNewVersion)
      globals::debugLog().addInfo(QString("%1 v%2.%3 is available for download.").arg(constants::kApplicationName)
         .arg(major).arg(minor));
   else
      globals::debugLog().addInfo("The application is up to date.");
   return hasNewVersion;
}


