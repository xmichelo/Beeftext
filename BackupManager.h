/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of backup manager
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__BACKUP__MANAGER__H
#define BEEFTEXT__BACKUP__MANAGER__H


//**********************************************************************************************************************
/// \brief Backup manager class
//**********************************************************************************************************************
class BackupManager
{
public: // static member functions
   static BackupManager& instance(); ///< Return the only allowed instance of the class

public: // member functions
   BackupManager(BackupManager const&) = delete; ///< Disabled copy-constructor
   BackupManager(BackupManager&&) = delete; ///< Disabled assignment copy-constructor
   ~BackupManager() = default; ///< Destructor
   BackupManager& operator=(BackupManager const&) = delete; ///< Disabled assignment operator
   BackupManager& operator=(BackupManager&&) = delete; ///< Disabled move assignment operator
   qint32 backupFileCount() const; ///< Return the number of backup files
   void removeAllBackups(); ///< Remove all backup files
   void cleanup(); ///< Perform backup cleanup
   void archive(QString const& filePath); ///< Move the given file to the backup folder.

private: // member functions
   BackupManager(); ///< Default constructor
   QStringList orderedBackupFilePaths() const; ///< Return the chronologically ordered list of backup file paths
   void ensureBackupFolderExists() const; ///< Ensure the backup folder exists
};


#endif // #ifndef BEEFTEXT__BACKUP__MANAGER__H