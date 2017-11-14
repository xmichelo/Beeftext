/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preference manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__PREFERENCE__MANAGER__H
#define BEEFTEXT__PREFERENCE__MANAGER__H


//**********************************************************************************************************************
/// \brief Preference manager class
//**********************************************************************************************************************
class PreferenceManager
{
public: // static member functions
   static PreferenceManager& instance(); ///< Return the only allowed instance of the class

public: // member functions
	~PreferenceManager() = default; ///< Default destructor
   void setMainWindowGeometry(QByteArray const& array); ///< Set the geometry of the main window
   QByteArray mainWindowGeometry() const; ///< Get the geometry of the main window

private: // member functions
   PreferenceManager(); ///< Default constructor
   PreferenceManager(PreferenceManager const&) = delete; ///< Disabled copy constructor
	PreferenceManager(PreferenceManager&&) = delete; ///< Disabled move constructor
	PreferenceManager& operator=(PreferenceManager const&) = delete; ///< Disabled assignment operator
	PreferenceManager& operator=(PreferenceManager&&) = delete; ///< Disabled move assignment operator

private: // data members
   QSettings settings_; ///< The Qt settings instance
};


#endif // #ifndef BEEFTEXT__PREFERENCE__MANAGER__H