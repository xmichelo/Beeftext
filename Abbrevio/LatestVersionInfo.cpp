/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of class holding information about the latest version of the application
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "LatestVersionInfo.h"


namespace {
qint32 constexpr kHashLength = 32; /// The length in bytes of the checksum
QString const kPropVersionMajor = "versionMajor"; ///< The JSON property name for the major version number
QString const kPropVersionMinor = "versionMinor"; ///< The JSON property name for the minor version number
QString const kPropDownloadUrl = "downloadUrl"; ///< The JSON property name for the download URL
QString const kPropReleaseUrl = "releaseUrl"; ///< The JSON property name for the release URL
QString const kPropSha256Sum = "sha256Sum"; ///< The JSON property name for the SHA256 sum
QString const kPropReleaseNotes = "releaseNotes"; ///< The JSON property name for the release notes
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
LatestVersionInfo::LatestVersionInfo()
    : versionNumber_(0, 0) {
}


//****************************************************************************************************************************************************
/// If the JSON object does not contain valid info, the constructed LatestVersionInfo instance is not valid
///
/// \param[in] object The JSON document containing the version information
//****************************************************************************************************************************************************
LatestVersionInfo::LatestVersionInfo(QJsonObject const &object)
    : LatestVersionInfo() {
    this->parseFromJsonObject(object);
}


//****************************************************************************************************************************************************
/// \return true if and only if the latest version information is not valid
//****************************************************************************************************************************************************
bool LatestVersionInfo::isValid() const {
    return versionNumber_.isValid() && (!downloadUrl_.isEmpty()) && (!releaseNotes_.isEmpty()) &&
           (sha256Hash_.size() == kHashLength);
}


//****************************************************************************************************************************************************
/// \return The version number.
//****************************************************************************************************************************************************
xmilib::VersionNumber LatestVersionInfo::versionNumber() const {
    return versionNumber_;
}


//****************************************************************************************************************************************************
/// \param[in] versionNumber The version number.
//****************************************************************************************************************************************************
void LatestVersionInfo::setVersionNumber(xmilib::VersionNumber const &versionNumber) {
    versionNumber_ = versionNumber;
}


//****************************************************************************************************************************************************
/// \return The download URL for the installer
//****************************************************************************************************************************************************
QString LatestVersionInfo::downloadUrl() const {
    return downloadUrl_;
}


//****************************************************************************************************************************************************
/// \param[in] downloadUrl The download download URL for the installer
//****************************************************************************************************************************************************
void LatestVersionInfo::setDownloadUrl(QString const &downloadUrl) {
    downloadUrl_ = downloadUrl;
}


//****************************************************************************************************************************************************
/// \return The release URL
//****************************************************************************************************************************************************
QString LatestVersionInfo::releaseUrl() const {
    return releaseUrl_;
}


//****************************************************************************************************************************************************
/// \param[in] url The release url
//****************************************************************************************************************************************************
void LatestVersionInfo::setReleaseUrl(QString const &url) {
    releaseUrl_ = url;
}


//****************************************************************************************************************************************************
/// \return the SHA256 has for the installer
//****************************************************************************************************************************************************
QByteArray LatestVersionInfo::sha256Hash() const {
    return sha256Hash_;
}


//****************************************************************************************************************************************************
/// \param[in] sha256Hash The SHA256 hash
//****************************************************************************************************************************************************
void LatestVersionInfo::setSha256Hash(QByteArray const &sha256Hash) {
    sha256Hash_ = sha256Hash;
}


//****************************************************************************************************************************************************
/// \return The release notes
//****************************************************************************************************************************************************
QString LatestVersionInfo::releaseNotes() const {
    return releaseNotes_;
}


//****************************************************************************************************************************************************
/// \param[in] releaseNotes The release notes
//****************************************************************************************************************************************************
void LatestVersionInfo::setReleaseNotes(QString const &releaseNotes) {
    releaseNotes_ = releaseNotes;
}


//****************************************************************************************************************************************************
/// If the JSON object does not contain valid info, the constructed LatestVersionInfo instance is not valid
///
/// \param[in] object The JSON object
//****************************************************************************************************************************************************
void LatestVersionInfo::parseFromJsonObject(QJsonObject const &object) {
    versionNumber_ = xmilib::VersionNumber(quint32(object[kPropVersionMajor].toInt(0)), quint32(object[kPropVersionMinor].toInt(0)));
    downloadUrl_ = object[kPropDownloadUrl].toString();
    releaseUrl_ = object[kPropReleaseUrl].toString();
    sha256Hash_ = QByteArray::fromHex(object[kPropSha256Sum].toString().toLocal8Bit());
    releaseNotes_ = object[kPropReleaseNotes].toString();
}


