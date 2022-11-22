/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of update dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_UPDATE_DIALOG_H
#define BEEFTEXT_UPDATE_DIALOG_H


#include "ui_UpdateDialog.h"
#include "LatestVersionInfo.h"


//****************************************************************************************************************************************************
/// \brief A dialog for software updates
//****************************************************************************************************************************************************
class UpdateDialog : public QDialog {
Q_OBJECT
public: // member functions
    explicit UpdateDialog(SpLatestVersionInfo latestVersionInfo, QWidget *parent = nullptr); ///< Default constructor
    UpdateDialog(UpdateDialog const &) = delete; ///< Disabled copy constructor
    UpdateDialog(UpdateDialog &&) = delete; ///< Disabled move constructor
    ~UpdateDialog() override = default; ///< Default destructor
    UpdateDialog &operator=(UpdateDialog const &) = delete; ///< Disabled assignment operator
    UpdateDialog &operator=(UpdateDialog &&) = delete; ///< Disabled move assignment operator

private slots:
    void onInstall(); ///< Callback for the 'Install' action
    void onNotNow(); ///< Callback for the 'Not now' action
    void onSkipThisVersion(); ///< Callback for the 'Skip' action.
    void onCancel(); ///< Callback for the 'Cancel' action.
    void onDownloadFinished(); ///< Slot for the finishing of the download operation
    void onDownloadError(QNetworkReply::NetworkError error); ///< Slot
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) const; ///< Slot for download progress reports
    void onDownloadDataAvailable(); ///< Slot for the availability of downloaded data

private: // member functions
    void startDownload(); ///< Start the downloading of the new installer file
    void processDownloadedData(QByteArray const &data); ///< Process downloaded data

private: // data members
    Ui::UpdateDialog ui_; ///< The GUI for the dialog
    SpLatestVersionInfo latestVersionInfo_; ///< The latest version info
    QCryptographicHash hashCalculator_; ///< The hash calculator
    QNetworkAccessManager nam_; ///< The network access manager
    QNetworkReply *reply_; ///< Reply the network reply
    QFile file_; ///< The file for storing the downloaded data
    QString installerPath_; /// The path of the installer path
    bool downloadErrorOccurred_; ///< Did a download error occur
};


#endif // #ifndef BEEFTEXT_UPDATE_DIALOG_H

