/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of update dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "UpdateDialog.h"
#include "UpdateManager.h"
#include "PreferencesManager.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include <XMiLib/FileUtils.h>
#include <XMiLib/Exception.h>


using namespace xmilib;


//**********************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
UpdateDialog::UpdateDialog(SPLatestVersionInfo latestVersionInfo, QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , latestVersionInfo_(latestVersionInfo)
   , hashCalculator_(QCryptographicHash::Sha256)
   , nam_()
   , reply_(nullptr)
   , file_()
   , installerPath_()
   , downloadErrorOccurred_(false)
{
   ui_.setupUi(this);
   if (latestVersionInfo_)
   {
      ui_.labelHeader->setText(ui_.labelHeader->text().arg(constants::kApplicationName)
         .arg(latestVersionInfo_->versionMajor()).arg(latestVersionInfo_->versionMinor()));
      ui_.editReleaseNotes->setHtml(latestVersionInfo_->releaseNotes());
   }
   // If you're AFK for days, we want to ensure we display only one notification windows, for the latest release, so:
   connect(&UpdateManager::instance(), &UpdateManager::updateIsAvailable, this, &UpdateDialog::reject); 
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateDialog::startDownload()
{
   if (reply_)
      throw xmilib::Exception("Error during software update: an ongoing download task already exists.");
   installerPath_ = createTempFile(file_, "exe");
   if (installerPath_.isEmpty() || !file_.isOpen())
      throw xmilib::Exception("Error during software update: could save installer file.");
   QNetworkRequest request(latestVersionInfo_->downloadUrl());
   reply_ = nam_.get(request);
   connect(reply_, &QNetworkReply::finished, this, &UpdateDialog::onDownloadFinished);
   connect(reply_, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, 
      &UpdateDialog::onDownloadError);
   connect(reply_, &QNetworkReply::downloadProgress, this, &UpdateDialog::onDownloadProgress);
   connect(reply_, &QNetworkReply::readyRead, this, &UpdateDialog::onDownloadDataAvailable);
   ui_.progressBar->setEnabled(true);
   ui_.progressBar->setRange(0, 100);
   ui_.progressBar->setValue(0);
   ui_.buttonSkip->setText(tr("&Cancel"));
   ui_.buttonInstall->setEnabled(false);
   hashCalculator_.reset();
   downloadErrorOccurred_ = false;
}


//**********************************************************************************************************************
/// \param[in] data The downloaded data
//**********************************************************************************************************************
void UpdateDialog::processDownloadedData(QByteArray const& data)
{
   if (!data.size())
      return;
   hashCalculator_.addData(data);
   if (data.size() != file_.write(data))
      throw xmilib::Exception("An error occurred while trying to save the update installation file.");
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateDialog::onActionInstall()
{
   this->startDownload();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateDialog::onActionSkip()
{
   if (reply_)
      reply_->abort();
   else
      this->reject();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateDialog::onDownloadFinished()
{
   if (!reply_)
      throw xmilib::Exception("Internal casting error during download finish callback.");
   if (!downloadErrorOccurred_) // note that error include user cancellation
   {
      this->processDownloadedData(reply_->readAll());
      if (hashCalculator_.result() != latestVersionInfo_->sha256Hash())
         QMessageBox::critical(this, tr("Error"), tr("The update cannot be installed: the checksum does not match."));
      else
      {
         file_.close();
         QDesktopServices::openUrl(QUrl::fromLocalFile(installerPath_));
         PreferencesManager::instance().setFileMarkedForDeletionOnStartup(installerPath_);
         qApp->exit(0);
      }
   }
   else // an error or cancellation occurred, we clean up
   {
      file_.close();
      file_.remove();
   }
   reply_->deleteLater();
   reply_ = nullptr;
   this->accept();
}


//**********************************************************************************************************************
/// \param[in] error The error
//**********************************************************************************************************************
void UpdateDialog::onDownloadError(QNetworkReply::NetworkError error)
{
   if (!reply_)
      throw xmilib::Exception("Internal casting error during download error callback.");
   downloadErrorOccurred_ = true;
   if (error != QNetworkReply::OperationCanceledError)
   {
      QString const errorMsg = reply_->errorString();
      globals::debugLog().addError(QString("A network error occurred while downloading software update: %1")
         .arg(errorMsg));
      QMessageBox::critical(this, tr("Error"), tr("An error occurred while downloading the software update:\n%1")
         .arg(errorMsg));
   }
}


//**********************************************************************************************************************
/// \param[in] bytesReceived The number of bytes already received
/// \param[in] bytesTotal The total number of bytes to download
//**********************************************************************************************************************
void UpdateDialog::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
   if (!reply_)
      throw xmilib::Exception("Internal casting error during download progress callback.");
   ui_.progressBar->setValue(float(bytesReceived) / float(bytesTotal) * 100.0f);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateDialog::onDownloadDataAvailable()
{
   if (!reply_)
      throw xmilib::Exception("Internal casting error during download data availability callback.");
   this->processDownloadedData(reply_->readAll());
}


