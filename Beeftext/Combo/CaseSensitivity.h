/// \file
/// \author 
///
/// \brief Declaration of combo case sensitivity related types and functions
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_CASE_SENSITIVITY_H
#define BEEFTEXT_CASE_SENSITIVITY_H


enum class ECaseSensitivity
{
   Default = 0, ///< The default case sensitivity.
   CaseSensitive = 1, ///< Case sensitive.
   CaseInsensitive = 2, ///< Case insensisitive.
   Count = 3 ///< The number of items in the enumeration.
};


void fillCaseSensitivityCombo(QComboBox& combo, bool includeDefault); ///< Fill a combo with the case sensitivity entries.
ECaseSensitivity selectedCaseSensitivityInCombo(QComboBox const& combo); ///< Retrieve the value of the selected case sensitivity in a combo.
void selectCaseSensitivityInCombo(QComboBox& combo, ECaseSensitivity sensitivity, bool blockSignals); ///< Select a case sensitivity in a combo box.


#endif // #ifndef BEEFTEXT_CASE_SENSITIVITY_H
