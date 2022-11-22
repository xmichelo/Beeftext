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
#include "Preferences/PreferencesManager.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


namespace {
qint64 constexpr kLaunchCheckDelayMs = 1000; ///< Delay for check for update at launch in milliseconds
qint64 constexpr kUpdateCheckIntervalMs = 1000LL * 60 * 60 * 24; ///< The interval for checking for updates in milliseconds
}


//****************************************************************************************************************************************************
/// \return The only allowed instance of the class
//****************************************************************************************************************************************************
UpdateManager &UpdateManager::instance() {
    static UpdateManager instance;
    return instance;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
UpdateManager::UpdateManager() {
    PreferencesManager const &prefs = PreferencesManager::instance();
    timer_.setSingleShot(true);
    connect(&timer_, &QTimer::timeout, this, &UpdateManager::checkForUpdateWitchSkipCheck);
    connect(&prefs, &PreferencesManager::autoCheckForUpdatesChanged, this, &UpdateManager::onAutoCheckForUpdateChanged);
    this->onAutoCheckForUpdateChanged(prefs.autoCheckForUpdates());
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void UpdateManager::checkForUpdateWitchSkipCheck() {
    timer_.stop();
    startUpdateCheckWorker(true);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateManager::checkForUpdateWithoutSkipCheck() {
    timer_.stop();
    startUpdateCheckWorker(false);
}


//****************************************************************************************************************************************************
/// \param[in] enabled Is the auto check for update enabled
//****************************************************************************************************************************************************
void UpdateManager::onAutoCheckForUpdateChanged(bool enabled) {
    timer_.stop();
    if (!enabled)
        return;

    QDateTime const lastCheckDateTime = PreferencesManager::instance().lastUpdateCheckDateTime();
    qint64 const msSecsToNextCheck = lastCheckDateTime.isNull() ? kLaunchCheckDelayMs : qMax<qint64>(kLaunchCheckDelayMs, QDateTime::currentDateTime().msecsTo(lastCheckDateTime.addMSecs(kUpdateCheckIntervalMs)));
    timer_.start(static_cast<qint32>(msSecsToNextCheck));
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateManager::startUpdateCheckWorker(bool verifySkippedVersion) {
    emit startedUpdateCheck();
    QThread *thread = new QThread;
    UpdateCheckWorker *worker = new UpdateCheckWorker(nullptr, verifySkippedVersion ?
                                                               PreferencesManager::instance().getSkipVersionNumber() : xmilib::VersionNumber());
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &UpdateCheckWorker::run);
    connect(worker, &UpdateCheckWorker::finished, this, &UpdateManager::onWorkerFinished);
    connect(worker, &UpdateCheckWorker::updateIsAvailable, this, &UpdateManager::onWorkerUpdateIsAvailable);
    connect(worker, &UpdateCheckWorker::noUpdateIsAvailable, this, &UpdateManager::onWorkerNoUpdateIsAvailable);
    connect(worker, &UpdateCheckWorker::error, this, &UpdateManager::onWorkerError);
    thread->start();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateManager::onWorkerFinished() {
    UpdateCheckWorker const *worker = dynamic_cast<UpdateCheckWorker *>(this->sender());
    if (!worker)
        throw xmilib::Exception("An Internal error occurred while checking for updates.");
    QThread *thread = worker->thread();
    thread->quit();
    thread->wait();
    delete worker;
    thread->deleteLater();
    emit finishedUpdateCheck();
    PreferencesManager::instance().setLastUpdateCheckDateTime(QDateTime::currentDateTime());
    timer_.start(kUpdateCheckIntervalMs);
}


//****************************************************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
//****************************************************************************************************************************************************
void UpdateManager::onWorkerUpdateIsAvailable(SpLatestVersionInfo const &latestVersionInfo) {
    if (!latestVersionInfo)
        throw xmilib::Exception(QString("%1(): latestVersionInfo parameter is null.").arg(__FUNCTION__));
    globals::debugLog().addInfo(QString("%1 v%2 is available for download.").arg(constants::kApplicationName)
        .arg(latestVersionInfo->versionNumber().toString()));
    emit updateIsAvailable(latestVersionInfo);
    UpdateDialog(latestVersionInfo).exec();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateManager::onWorkerNoUpdateIsAvailable() {
    emit noUpdateIsAvailable();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void UpdateManager::onWorkerError(QString const &) {
    emit updateCheckFailed();
}


