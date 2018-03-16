/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of backup manager


#include "stdafx.h"
#include "BackupManager.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {
   QRegularExpression const kBackupFileRegExp(R"(^\d{8}_\d{9}_backup\.json$)");
   qint32 kMaxBackupFileCount = 30;
}


//**********************************************************************************************************************
/// \return The only allowed instance of the class
//**********************************************************************************************************************
BackupManager& BackupManager::instance()
{
   static BackupManager instance;
   return instance;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
BackupManager::BackupManager()
{
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void BackupManager::cleanup()
{
   xmilib::DebugLog& log = globals::debugLog();
   QStringList paths = this->getOrderedBackupFilePaths();
   qint32 const count = paths.size();
   for (int i = 0; i < count - kMaxBackupFileCount; ++i)
   {
      QString const path = paths[i];
      if (QFile(path).remove())
         log.addInfo(QString("Removed backup file %1").arg(QDir::toNativeSeparators(path)));
      else
         log.addWarning(QString("Could not remove %1").arg(QDir::toNativeSeparators(path)));
   }
}


//**********************************************************************************************************************
/// \param[in] filePath The path of the file to archive
//**********************************************************************************************************************
void BackupManager::archive(QString const& filePath)
{
   this->ensureBackupFolderExists();
   xmilib::DebugLog& log = globals::debugLog();
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


//**********************************************************************************************************************
/// \return The chronologically ordered list of backup file paths
//**********************************************************************************************************************
QStringList BackupManager::getOrderedBackupFilePaths() const
{
   QString const path = globals::backupFolderPath();
   QFileInfo folderInfo(path);
   if (!folderInfo.exists() || (!folderInfo.isDir()))
      return QStringList();
   QStringList result;
   for (QFileInfo fileInfo : QDir(path).entryInfoList(QDir::Files, QDir::Name))
      if (kBackupFileRegExp.match(fileInfo.fileName()).hasMatch())
         result.push_back(fileInfo.absoluteFilePath());
   return result;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void BackupManager::ensureBackupFolderExists() const
{
   QString const path = globals::backupFolderPath();
   if (QFileInfo(path).exists())
      return;
   QDir().mkpath(path);
   if (!QFileInfo(path).exists())
      return throw xmilib::Exception(QString("Internal error: %1(): could not create backup folder.")
         .arg(__FUNCTION__));
}

