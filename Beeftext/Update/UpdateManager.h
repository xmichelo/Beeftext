/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of software update manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_UPDATE_MANAGER_H
#define BEEFTEXT_UPDATE_MANAGER_H


#include "LatestVersionInfo.h"


//****************************************************************************************************************************************************
/// \brief Software update manager class
//****************************************************************************************************************************************************
class UpdateManager : public QObject {
Q_OBJECT
public: // member functions
    static UpdateManager &instance(); ///< Return The only allowed instance of the class
    UpdateManager(UpdateManager const &) = delete; ///< Disabled copy constructor
    UpdateManager(UpdateManager &&) = delete; ///< Disabled move constructor
    ~UpdateManager() override = default; ///< Default destructor
    UpdateManager &operator=(UpdateManager const &) = delete; ///< Disabled assignment operator
    UpdateManager &operator=(UpdateManager &&) = delete; ///< Disabled move assignment operator

signals:
    void startedUpdateCheck(); ///< Signal for the starting of the update check
    void finishedUpdateCheck(); ///< Signal for the finishing of the update check
    void updateIsAvailable(SpLatestVersionInfo latestVersionInfo); ///< Signal for the availability of an update
    void noUpdateIsAvailable(); ///< Signal for the notification that no update is available
    void updateCheckFailed(); ///< Signal for failures of update check

public slots:
    void checkForUpdateWitchSkipCheck(); ///< Perform an update check, and ignore it if version is marked as skip
    void checkForUpdateWithoutSkipCheck(); ///< Perform an update check, but do not check if the version is marked as skip.
    void onAutoCheckForUpdateChanged(bool enabled); ///< Signal for the changing of the 'Auto check for update

private: // member functions
    UpdateManager(); ///< Default constructor
    void startUpdateCheckWorker(bool verifySkippedVersion); ///< Start the update worker

private slots:
    void onWorkerFinished(); ///< Slot for the finishing of the worker
    void onWorkerUpdateIsAvailable(SpLatestVersionInfo const &latestVersionInfo); ///< Slot for the reporting of new version
    void onWorkerNoUpdateIsAvailable(); ///< Slot for the notification that no update is available
    void onWorkerError(QString const &); ///< Slot for worker errors

private: // data members
    QTimer timer_; /// The timer for periodical checks
};


#endif // #ifndef BEEFTEXT_UPDATE_MANAGER_H