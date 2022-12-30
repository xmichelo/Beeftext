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
#include "Preferences/PreferencesManager.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"
#include <XMiLib/File/FileUtils.h>
#include <XMiLib/Exception.h>
#include <XMiLib/XMiLibConstants.h>
#include <utility>


using namespace xmilib;


//****************************************************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
/// \param[in] parent The parent widget of the dialog
//****************************************************************************************************************************************************
UpdateDialog::UpdateDialog(SpLatestVersionInfo latestVersionInfo, QWidget *parent)
    : QDialog(parent, xmilib::constants::kDefaultDialogFlags), ui_(), latestVersionInfo_(std::move(latestVersionInfo)), hashCalculator_(QCryptographicHash::Sha256), reply_(nullptr), downloadErrorOccurred_(false) {
    bool const portableMode = isInPortableMode();
    ui_.setupUi(this);
    ui_.progressBar->setVisible(!portableMode);
    if (latestVersionInfo_) {
        ui_.labelHeader->setText(ui_.labelHeader->text().arg(::constants::kApplicationName)
            .arg(latestVersionInfo_->versionNumber().toString()));
        ui_.editReleaseNotes->setHtml(latestVersionInfo_->releaseNotes());
    }
    ui_.buttonInstall->setText(portableMode ? tr("&Download Page") : tr("&Install"));
    ui_.buttonCancel->setVisible(false);

    connect(ui_.buttonCancel, &QPushButton::clicked, this, &UpdateDialog::onCancel);
    connect(ui_.buttonInstall, &QPushButton::clicked, this, &UpdateDialog::onInstall);
    connect(ui_.buttonNotNow, &QPushButton::clicked, this, &UpdateDialog::onNotNow);
    connect(ui_.buttonSkipThisVersion, &QPushButton::clicked, this, &UpdateDialog::onSkipThisVersion);
    connect(&UpdateManager::instance(), &UpdateManager::updateIsAvailable, this, &UpdateDialog::reject); // If you're AFK for days, we want to ensure we display only one notification windows, for the latest release
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateDialog::startDownload() {
    if (reply_)
        throw Exception("Error during software update: an ongoing download task already exists.");
    installerPath_ = createTempFile(file_, "BeeftextUpdater_", "exe");
    if (installerPath_.isEmpty() || !file_.isOpen())
        throw Exception("Error during software update: could save installer file.");
    QNetworkRequest request(latestVersionInfo_->downloadUrl());
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    request.setMaximumRedirectsAllowed(10);
    reply_ = nam_.get(request);
    connect(reply_, &QNetworkReply::finished, this, &UpdateDialog::onDownloadFinished);
    connect(reply_, &QNetworkReply::errorOccurred, this, &UpdateDialog::onDownloadError);
    connect(reply_, &QNetworkReply::downloadProgress, this, &UpdateDialog::onDownloadProgress);
    connect(reply_, &QNetworkReply::readyRead, this, &UpdateDialog::onDownloadDataAvailable);
    ui_.progressBar->setEnabled(true);
    ui_.progressBar->setRange(0, 100);
    ui_.progressBar->setValue(0);
    ui_.buttonInstall->setVisible(false);
    ui_.buttonSkipThisVersion->setVisible(false);
    ui_.buttonNotNow->setVisible(false);
    ui_.buttonCancel->setVisible(true);
    hashCalculator_.reset();
    downloadErrorOccurred_ = false;
}


//****************************************************************************************************************************************************
/// \param[in] data The downloaded data
//****************************************************************************************************************************************************
void UpdateDialog::processDownloadedData(QByteArray const &data) {
    if (!data.size())
        return;
    hashCalculator_.addData(data);
    if (data.size() != file_.write(data))
        throw Exception("An error occurred while trying to save the update installation file.");
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateDialog::onInstall() {
    if (isInPortableMode()) {
        QDesktopServices::openUrl(QUrl(latestVersionInfo_->releaseUrl()));
        this->accept();
    } else {
        PreferencesManager::instance().removeSkipVersionNumber();
        this->startDownload();
    }
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateDialog::onNotNow() {
    PreferencesManager::instance().removeSkipVersionNumber();
    this->reject();
}


//****************************************************************************************************************************************************
//
//*********************************eg*************************************************************************************
void UpdateDialog::onSkipThisVersion() {
    qDebug() << QString("%1()").arg(__FUNCTION__);
    PreferencesManager::instance().setSkipVersionNumber(latestVersionInfo_->versionNumber());
    this->reject();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void UpdateDialog::onCancel() {
    if (reply_)
        reply_->abort();
    else
        this->reject();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateDialog::onDownloadFinished() {
    if (!reply_)
        throw Exception("Internal casting error during download finish callback.");
    if (!downloadErrorOccurred_) // note that error include user cancellation
    {
        this->processDownloadedData(reply_->readAll());
        if (hashCalculator_.result() != latestVersionInfo_->sha256Hash())
            QMessageBox::critical(this, tr("Error"), tr("The update cannot be installed: the checksum does not match."));
        else {
            file_.close();
            QDesktopServices::openUrl(QUrl::fromLocalFile(installerPath_));
            PreferencesManager::instance().setFileMarkedForDeletionOnStartup(installerPath_);
            QCoreApplication::exit(0);
        }
    } else // an error or cancellation occurred, we clean up
    {
        file_.close();
        file_.remove();
    }
    reply_->deleteLater();
    reply_ = nullptr;
    this->accept();
}


//****************************************************************************************************************************************************
/// \param[in] error The error
//****************************************************************************************************************************************************
void UpdateDialog::onDownloadError(QNetworkReply::NetworkError error) {
    if (!reply_)
        throw Exception("Internal casting error during download error callback.");
    downloadErrorOccurred_ = true;
    if (QNetworkReply::OperationCanceledError == error) {
        globals::debugLog().addInfo("Software update download was cancelled");
        return;
    }
    QString const errorMsg = reply_->errorString();
    globals::debugLog().addError(QString("A network error occurred while downloading software update: %1")
        .arg(errorMsg));
    QMessageBox::critical(this, tr("Error"), tr("An error occurred while downloading the software update:\n%1")
        .arg(errorMsg));
}


//****************************************************************************************************************************************************
/// \param[in] bytesReceived The number of bytes already received
/// \param[in] bytesTotal The total number of bytes to download
//****************************************************************************************************************************************************
void UpdateDialog::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) const {
    if (!reply_)
        throw Exception("Internal casting error during download progress callback.");
    ui_.progressBar->setValue(qint32(float(bytesReceived) / float(bytesTotal) * 100.0f));
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void UpdateDialog::onDownloadDataAvailable() {
    if (!reply_)
        throw Exception("Internal casting error during download data availability callback.");
    this->processDownloadedData(reply_->readAll());
}
