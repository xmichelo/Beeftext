/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of software update manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "UpdateManager.h"
#include "UpdateDialog.h"
#include "UpdateCheckWorker.h"
#include "PreferencesManager.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


namespace {
   qint64 const kLaunchCheckDelayMs = 3000; ///< Delay for check for update at launch in milliseconds
   qint64 const kUpdateCheckIntervalMs = 1000 * 60 * 60 * 24; ///< The interval for checking for updates in milliseconds
}


//**********************************************************************************************************************
/// \return The only allowed instance of the class
//**********************************************************************************************************************
UpdateManager& UpdateManager::instance()
{
   static UpdateManager instance;
   return instance;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
UpdateManager::UpdateManager()
   : QObject()
   , timer_()
{
   timer_.setSingleShot(true);
   connect(&timer_, &QTimer::timeout, this, &UpdateManager::checkForUpdate);
   QDateTime lastCheckDateTime = PreferencesManager::instance().lastUpdateCheckDateTime();
   qint64 msSecsToNextCheck = lastCheckDateTime.isNull() ? kLaunchCheckDelayMs :
      qMax<qint64>(kLaunchCheckDelayMs, 
         QDateTime::currentDateTime().msecsTo(lastCheckDateTime.addMSecs(kUpdateCheckIntervalMs)));
   QTimer::singleShot(msSecsToNextCheck, this, &UpdateManager::checkForUpdate);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateManager::checkForUpdate()
{
   timer_.stop();
   startUpdateCheckWorker();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateManager::startUpdateCheckWorker()
{
   emit startedUpdateCheck();
   QThread *thread = new QThread;
   UpdateCheckWorker* worker = new UpdateCheckWorker;
   worker->moveToThread(thread);
   connect(thread, &QThread::started, worker, &UpdateCheckWorker::run);
   connect(worker, &UpdateCheckWorker::finished, this, &UpdateManager::onWorkerFinished);
   connect(worker, &UpdateCheckWorker::updateIsAvailable, this, &UpdateManager::onWorkerUpdateIsAvailable);
   connect(worker, &UpdateCheckWorker::noUpdateIsAvailable, this, &UpdateManager::onWorkerNoUpdateIsAvailable);
   connect(worker, &UpdateCheckWorker::error, this, &UpdateManager::onWorkerError);
   thread->start();

}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateManager::onWorkerFinished()
{
   UpdateCheckWorker *worker = dynamic_cast<UpdateCheckWorker*>(this->sender());
   if (!worker)
      throw xmilib::Exception("An Internal error occurred while checking for updates.");
   QThread *thread = worker->thread();
   thread->quit();
   thread->wait();
   worker->deleteLater();
   thread->deleteLater();
   emit finishedUpdateCheck();
   PreferencesManager::instance().setLastUpdateCheckDateTime(QDateTime::currentDateTime());
   timer_.start(kUpdateCheckIntervalMs);
}


//**********************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
//**********************************************************************************************************************
void UpdateManager::onWorkerUpdateIsAvailable(SPLatestVersionInfo latestVersionInfo)
{
   if (!latestVersionInfo)
      throw xmilib::Exception(QString("%1(): latestVersionInfo parameter is null.").arg(__FUNCTION__));
   globals::debugLog().addInfo(QString("%1 v%2.%3 is available for download.").arg(constants::kApplicationName)
      .arg(latestVersionInfo->versionMajor()).arg(latestVersionInfo->versionMinor()));
   emit updateIsAvailable(latestVersionInfo);
   UpdateDialog(latestVersionInfo).exec();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateManager::onWorkerNoUpdateIsAvailable()
{
   globals::debugLog().addInfo("The application is up to date.");
   emit noUpdateIsAvailable();
}


//**********************************************************************************************************************
/// \param[in] error The error message
//**********************************************************************************************************************
void UpdateManager::onWorkerError(QString const& error)
{
   globals::debugLog().addError(QString("Error during update check: %1").arg(error));
   emit updateCheckFailed();
}


