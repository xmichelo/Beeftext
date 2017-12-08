/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of class holding information about the latest version of the application
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__LATEST__VERSION__INFO__H
#define BEEFTEXT__LATEST__VERSION__INFO__H


#include <memory>


class LatestVersionInfo;


//**********************************************************************************************************************
/// \brief A class holding information about the latest version of the application
//**********************************************************************************************************************
class LatestVersionInfo
{
public: // member functions
   LatestVersionInfo(); ///< Default constructor
   LatestVersionInfo(QJsonObject const& object); ///< Constructor from a JSON object
	~LatestVersionInfo() = default; ///< Default destructor
   bool isValid() const; ///< Check whether the instance is valid
   qint32 versionMajor() const; ///< Get the major version number
   void setVersionMajor(qint32 versionMajor); ///< Get he major version number
   qint32 versionMinor() const; ///< Get the minor version number
   void setVersionMinor(qint32 versionMinor); ///< Get he minor version number
   QString downloadUrl() const; ///< Get the download URL
   void setDownloadUrl(QString const& url); ///< Set the download URL
   QByteArray sha256Hash() const; ///< Get the SHA256 hash for the installer
   void setSha256Hash(QByteArray const& sha256Hash); ///< Set the SHA256 hash for the installer
   QString releaseNotes() const; ///< Get the release notes
   void setReleaseNotes(QString const& releaseNotes); ///< Set the release notes

private: // member functions
   LatestVersionInfo(LatestVersionInfo const&) = delete; ///< Disabled copy constructor
   LatestVersionInfo(LatestVersionInfo&&) = delete; ///< Disabled move constructor
   LatestVersionInfo& operator=(LatestVersionInfo const&) = delete; ///< Disabled assignment operator
	LatestVersionInfo& operator=(LatestVersionInfo&&) = delete; ///< Disabled move assignment operator
   void parseFromJsonObject(QJsonObject const& object); ///< Parse the latest version info from a JSON object

private: // data members
   qint32 versionMajor_; ///< The major version number
   qint32 versionMinor_; ///< The minor version number
   QString downloadUrl_; ///< The download url of the installer file
   QByteArray sha256Hash_; ///< The SHA256 hash for the installer 
   QString releaseNotes_; ///< The release notes for the version
};


typedef std::shared_ptr<LatestVersionInfo> SPLatestVersionInfo; ///< Type declaration for shared pointer to LatestVersionInfo
Q_DECLARE_METATYPE(SPLatestVersionInfo)


#endif // #ifndef BEEFTEXT__LATEST__VERSION__INFO__H