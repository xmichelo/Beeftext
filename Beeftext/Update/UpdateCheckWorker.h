/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of worker responsible for checking for updates
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_UPDATE_CHECK_WORKER_H
#define BEEFTEXT_UPDATE_CHECK_WORKER_H


#include "LatestVersionInfo.h"
#include <XMiLib/VersionNumber/VersionNumber.h>


//****************************************************************************************************************************************************
/// \brief A worker responsible for checking for updates
//****************************************************************************************************************************************************
class UpdateCheckWorker : public QObject {
Q_OBJECT
public: // member functions
    explicit UpdateCheckWorker(QObject *parent, xmilib::VersionNumber const &skipVersionNumber); ///< Default constructor
    UpdateCheckWorker(UpdateCheckWorker const &) = delete; ///< Disabled copy constructor
    UpdateCheckWorker(UpdateCheckWorker &&) = delete; ///< Disabled move constructor
    ~UpdateCheckWorker() override = default; ///< Default destructor
    UpdateCheckWorker &operator=(UpdateCheckWorker const &) = delete; ///< Disabled assignment operator
    UpdateCheckWorker &operator=(UpdateCheckWorker &&) = delete; ///< Disabled move assignment operator

public slots:
    void run(); // run the worker

signals:
    void finished(); ///< Signal for the finishing of the worker
    void updateIsAvailable(SpLatestVersionInfo lastestVersionInfo); ///< Signal for the existence of a newer version of the application
    void noUpdateIsAvailable(); ///< Signal for no update available
    void error(QString const &error); ///< Signal for error

private: // member functions
    void performUpdateCheck(); ///< perform the update check
    QByteArray downloadLatestVersionInformation() const; ///< Download the latest version information from the Beeftext website
    SpLatestVersionInfo parseJsonData(QString const &jsonData) const; ///< Parse the JSon data
    bool isNewVersionAvailable(SpLatestVersionInfo const &latestVersionInfo, bool *outSkipped) const; ///< Check if the installed version of the application is outdated

private: // data members
    xmilib::VersionNumber skipVersionNumber_; ///< The version number if any (can be null)
};


#endif // #ifndef BEEFTEXT_UPDATE_CHECK_WORKER_H