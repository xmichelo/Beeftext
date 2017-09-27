/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of utility functions related to combos
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboUtils.h"

namespace {

   QString const kKeyGroupName = "groupName"; ///< The JSon Key for the combo group
   QString const kDefaultGroupName = "Default"; ///< The default combo group name
   QString const kKeyComboList = "comboList"; ///< The JSon key for the combo list

}


//**********************************************************************************************************************
/// \param[in] comboList The list of combo
/// \return A JSon array representing the list of combo
//**********************************************************************************************************************
QJsonArray comboListToJSonArray(VecSPCombo const& comboList)
{
   // In a future release, we will add support for grouping combos into group. For now, in JSon export we put 
   // everything in a default hidden category.
   QJsonArray result;
   QJsonObject group;
   group.insert(kKeyGroupName, kDefaultGroupName);
   QJsonArray comboListArray;
   for (SPCombo const& combo : comboList)
      comboListArray.append(combo->toJsonObject());
   group.insert(kKeyComboList, comboListArray);
   result.append(group);
   return result;
}