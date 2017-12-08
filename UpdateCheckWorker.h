/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of worker responsible for checking for updates
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__UPDATE__CHECK__WORKER__H
#define BEEFTEXT__UPDATE__CHECK__WORKER__H


#include "LatestVersionInfo.h"


//**********************************************************************************************************************
/// \brief A worker responsible for checking for updates
//**********************************************************************************************************************
class UpdateCheckWorker: public QObject
{
   Q_OBJECT
public: // member functions
	UpdateCheckWorker(QObject* parent = nullptr); ///< Default constructor
	~UpdateCheckWorker() = default; ///< Default destructor
	
public slots:
   void run(); // run the worker
signals:
   void finished(); ///< Signal for the finishing of the worker
   void newVersionIsAvailable(SPLatestVersionInfo lastestVersionInfo); ///< Signal for the existence of a newer version of the application
   
private: // member functions
	UpdateCheckWorker(UpdateCheckWorker const&) = delete; ///< Disabled copy constructor
	UpdateCheckWorker(UpdateCheckWorker&&) = delete; ///< Disabled move constructor
	UpdateCheckWorker& operator=(UpdateCheckWorker const&) = delete; ///< Disabled assignment operator
	UpdateCheckWorker& operator=(UpdateCheckWorker&&) = delete; ///< Disabled move assignment operator
   void performUpdateCheck(); ///< perform the update check
   QByteArray downloadLatestVersionInformation() const; ///< Download the latest version information from the Beeftext website
   SPLatestVersionInfo parseJsonData(QString const& jsonData) const; ///< Parse the JSon data
   bool isNewVersionAvailable(SPLatestVersionInfo latestVersionInfo) const; ///< Check if the installed version of the application is outdated
};


#endif // #ifndef BEEFTEXT__UPDATE__CHECK__WORKER__H