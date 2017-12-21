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
   emit finished(); // note that finished is emitted even in case of error
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateCheckWorker::performUpdateCheck()
{
   try
   {
      QByteArray jsonData = this->downloadLatestVersionInformation();
      SPLatestVersionInfo latestVersionInfo = this->parseJsonData(jsonData);
      if (this->isNewVersionAvailable(latestVersionInfo))
         emit updateIsAvailable(latestVersionInfo);
      else
         emit noUpdateIsAvailable();
   }
   catch (xmilib::Exception const& e)
   {
      emit error(e.qwhat());
   }
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
      throw xmilib::Exception(QString("Could not retrieve version information file from the Beeftext website: %1")
         .arg(errorMessage));
   QByteArray const result = reply->readAll();
   reply->deleteLater();
   return result;
}


//**********************************************************************************************************************
/// \param[in] jsonData the JSON data
/// \return A shared pointer to the parsed data. 
//**********************************************************************************************************************
SPLatestVersionInfo UpdateCheckWorker::parseJsonData(QString const& jsonData) const
{
   QString errMsg;
   QJsonParseError error;
   QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);
   if (error.error != QJsonParseError::NoError)
      throw xmilib::Exception(QString("Could not retrieve version information. The downloaded JSON file is "
         "invalid: %1").arg(error.errorString()));
   SPLatestVersionInfo result = std::make_shared<LatestVersionInfo>(doc.object());
   if (!result->isValid())
      throw xmilib::Exception(QString("Could not retrieve version information. The downloaded JSON file is "
      "invalid."));
   return result;
}


//**********************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
//**********************************************************************************************************************
bool UpdateCheckWorker::isNewVersionAvailable(SPLatestVersionInfo latestVersionInfo) const
{
   if (!latestVersionInfo)
      throw xmilib::Exception("Could not check for new version: the retrieved latest version information is "
         "null.");
   qint32 const major = latestVersionInfo->versionMajor();
   qint32 const minor = latestVersionInfo->versionMinor();
   return (major > constants::kVersionMajor)
      || ((major == constants::kVersionMajor) && (minor > constants::kVersionMinor));
}


