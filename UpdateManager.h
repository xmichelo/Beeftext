/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of software update manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__UPDATE__MANAGER__H
#define BEEFTEXT__UPDATE__MANAGER__H


#include "LatestVersionInfo.h"


//**********************************************************************************************************************
/// \brief Software update manager class
//**********************************************************************************************************************
class UpdateManager: public QObject
{
   Q_OBJECT
public: // member functions
   static UpdateManager& instance(); ///< Return The only allowed instance of the class
	~UpdateManager() = default; ///< Default destructor
	
signals: 
   void startedUpdateCheck(); ///< Signal for the starting of the update check
   void finishedUpdateCheck(); ///< Signal for the finishing of the update check
   void updateIsAvailable(SPLatestVersionInfo latestVersionInfo); ///< Signal for the availability of an update
   void noUpdateIsAvailable(); ///< Signal for the notification that no update is available
   void updateCheckFailed(); ///< Signal for failures of update check

public slots:
   void checkForUpdate(); ///< Force a check for update
   void onAutoCheckForUpdateChanged(bool enabled); ///< Signal for the changing of the 'Auto check for update

private: // member functions
   UpdateManager(); ///< Default constructor
   UpdateManager(UpdateManager const&) = delete; ///< Disabled copy constructor
	UpdateManager(UpdateManager&&) = delete; ///< Disabled move constructor
	UpdateManager& operator=(UpdateManager const&) = delete; ///< Disabled assignment operator
	UpdateManager& operator=(UpdateManager&&) = delete; ///< Disabled move assignment operator
   void startUpdateCheckWorker(); ///< Start the update worker

private slots:
   void onWorkerFinished(); ///< Slot for the finishing of the worker
   void onWorkerUpdateIsAvailable(SPLatestVersionInfo latestVersionInfo); ///< Slot for the reporting of new version
   void onWorkerNoUpdateIsAvailable(); ///< Slot for the notification that no update is available
   void onWorkerError(QString const& error); ///< Slot for worker errors

private: // data members
   QTimer timer_; /// The timer for periodical checks
};


#endif // #ifndef BEEFTEXT__UPDATE__MANAGER__H