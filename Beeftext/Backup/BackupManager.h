/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of backup manager
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_BACKUP_MANAGER_H
#define BEEFTEXT_BACKUP_MANAGER_H


//****************************************************************************************************************************************************
/// \brief Backup manager class
//****************************************************************************************************************************************************
class BackupManager {
public: // static member functions
    static BackupManager &instance(); ///< Return the only allowed instance of the class
    static QStringList orderedBackupFilePaths(QString const &path); ///< Return the chronologically ordered list of backup file paths in the application backup folder.
    static QStringList orderedBackupFilePaths(); ///< Return the chronologically ordered list of backup file paths in the application backup folder.
    static bool moveBackupFolder(QString const &oldPath, QString const &newPath); ///< Move the backup folder from oldPath to newPath

public: // member functions
    BackupManager(BackupManager const &) = delete; ///< Disabled copy-constructor
    BackupManager(BackupManager &&) = delete; ///< Disabled assignment copy-constructor
    ~BackupManager() = default; ///< Destructor
    BackupManager &operator=(BackupManager const &) = delete; ///< Disabled assignment operator
    BackupManager &operator=(BackupManager &&) = delete; ///< Disabled move assignment operator
    qint32 backupFileCount() const; ///< Return the number of backup files
    void removeAllBackups() const; ///< Remove all backup files
    void cleanup() const; ///< Perform backup cleanup
    void archive(QString const &filePath) const; ///< Move the given file to the backup folder.

private: // member functions
    BackupManager() = default; ///< Default constructor
};


#endif // #ifndef BEEFTEXT_BACKUP_MANAGER_H