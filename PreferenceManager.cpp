/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of preference manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferenceManager.h"
#include "BeeftextConstants.h"


namespace {
   QString const kKeyGeometry = "Geometry"; ///< The settings key for storing the geometry
}


//**********************************************************************************************************************
/// \return The only allowed instance of the class
//**********************************************************************************************************************
PreferenceManager& PreferenceManager::instance()
{
   static PreferenceManager prefManager;
   return prefManager;
}


//**********************************************************************************************************************
/// \note We set the organization and application names manually in case we want to use the preferences before the call 
/// to QApplication::SetOrganizationName() and QApplication::SetApplicationName()
//**********************************************************************************************************************
PreferenceManager::PreferenceManager()
   : settings_(constants::kOrganizationName, constants::kApplicationName)
{

}


//**********************************************************************************************************************
/// \param[in] array The geometry as a byte array
//**********************************************************************************************************************
void PreferenceManager::setMainWindowGeometry(QByteArray const& array)
{
   settings_.setValue(kKeyGeometry, array);
}


//**********************************************************************************************************************
/// \return The geometry of the main window as a byte array
//**********************************************************************************************************************
QByteArray PreferenceManager::mainWindowGeometry() const
{
   return settings_.value(kKeyGeometry).toByteArray();
}

