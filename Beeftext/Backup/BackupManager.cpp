/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of backup manager


#include "stdafx.h"
#include "BackupManager.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {
QRegularExpression const kBackupFileRegExp(R"(^\d{8}_\d{9}_backup\.json$)");
qint32 constexpr kMaxBackupFileCount = 50;
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ensureBackupFolderExists() {
    QString const path = globals::backupFolderPath();
    if (QFileInfo(path).exists())
        return;
    QDir().mkpath(path);
    if (!QFileInfo(path).exists())
        throw Exception(QString("Internal error: %1(): could not create backup folder.")
            .arg(__FUNCTION__));
}


//****************************************************************************************************************************************************
/// \return The only allowed instance of the class
//****************************************************************************************************************************************************
BackupManager &BackupManager::instance() {
    static BackupManager instance;
    return instance;
}


//****************************************************************************************************************************************************
/// \return The chronologically ordered list of backup file paths
//****************************************************************************************************************************************************
QStringList BackupManager::orderedBackupFilePaths(QString const &path) {
    QFileInfo const folderInfo(path);
    if (!folderInfo.exists() || (!folderInfo.isDir()))
        return QStringList();
    QStringList result;
    for (QFileInfo const &fileInfo: QDir(path).entryInfoList(QDir::Files, QDir::Name))
        if (kBackupFileRegExp.match(fileInfo.fileName()).hasMatch())
            result.push_back(fileInfo.absoluteFilePath());
    return result;
}


//****************************************************************************************************************************************************
/// \return The chronologically ordered list of backup file paths
//****************************************************************************************************************************************************
QStringList BackupManager::orderedBackupFilePaths() {
    return orderedBackupFilePaths(globals::backupFolderPath());
}


//****************************************************************************************************************************************************
/// \param[in] oldPath The old path of the backup folder.
/// \param[in] newPath The new path of the backup folder.
/// \return true if and only if the operation was successful.
//****************************************************************************************************************************************************
bool BackupManager::moveBackupFolder(QString const &oldPath, QString const &newPath) {
    try {
        QDir const newDir(newPath);
        if ((!newDir.exists()) && !QDir().mkpath(newPath))
            throw Exception("The backup folder could not be created");
        QStringList const files = orderedBackupFilePaths();
        bool error = false;
        for (QString const &filePath: orderedBackupFilePaths(oldPath)) {
            QFileInfo file(filePath);
            if ((!file.exists()) || (QFile(filePath).rename(QDir(newPath).absoluteFilePath(file.fileName()))))
                error = true;
        }
        if (error)
            throw Exception("Some backup files could not be moved to their new location.");
        return true;
    }
    catch (Exception const &e) {
        globals::debugLog().addError(QString("An error occurred while moving the backup folder: %1").arg(e.qwhat()));
        return false;
    }
}


//****************************************************************************************************************************************************
/// \return the number of backup files in the backup folder
//****************************************************************************************************************************************************
qint32 BackupManager::backupFileCount() const {
    return qint32(this->orderedBackupFilePaths().size());
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void BackupManager::removeAllBackups() const {
    DebugLog &log = globals::debugLog();
    for (QString const &path: this->orderedBackupFilePaths()) {
        if (QFile(path).remove())
            log.addInfo(QString("Removed backup file %1").arg(QDir::toNativeSeparators(path)));
        else
            log.addWarning(QString("Could not remove %1").arg(QDir::toNativeSeparators(path)));
    }
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void BackupManager::cleanup() const {
    DebugLog &log = globals::debugLog();
    QStringList paths = this->orderedBackupFilePaths();
    qint32 const count = qint32(paths.size());
    for (int i = 0; i < count - kMaxBackupFileCount; ++i) {
        QString const path = paths[i];
        if (QFile(path).remove())
            log.addInfo(QString("Removed backup file %1").arg(QDir::toNativeSeparators(path)));
        else
            log.addWarning(QString("Could not remove %1").arg(QDir::toNativeSeparators(path)));
    }
}


//****************************************************************************************************************************************************
/// \param[in] filePath The path of the file to archive
//****************************************************************************************************************************************************
void BackupManager::archive(QString const &filePath) const {
    ensureBackupFolderExists();
    DebugLog &log = globals::debugLog();
    QString const backupFolderPath = globals::backupFolderPath();
    QFileInfo const fileInfo(filePath);
    QString const dstPath = QDir(backupFolderPath)
        .absoluteFilePath(QString("%1_backup.json").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmsszzz")));
    if ((!fileInfo.exists()) || (!QFile(filePath).rename(dstPath)))
        log.addWarning(QString("Could not archive file %1").arg(QDir::toNativeSeparators(dstPath)));
    else
        log.addInfo(QString("Backed up combo file to %1").arg(QDir::toNativeSeparators(dstPath)));
    this->cleanup();
}


