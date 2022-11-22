/// \file
/// \author 
///
/// \brief Declaration of combo case sensitivity related types and functions
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_CASE_SENSITIVITY_H
#define BEEFTEXT_CASE_SENSITIVITY_H


enum class ECaseSensitivity {
    Default = 0, ///< The default case sensitivity.
    CaseSensitive = 1, ///< Case-sensitive.
    CaseInsensitive = 2, ///< Case-insensitive.
    Count = 3 ///< The number of items in the enumeration.
};


void fillCaseSensitivityCombo(QComboBox &combo, bool includeDefault); ///< Fill a combo with the case sensitivity entries.
ECaseSensitivity selectedCaseSensitivityInCombo(QComboBox const &combo); ///< Retrieve the value of the selected case sensitivity in a combo.
void selectCaseSensitivityInCombo(QComboBox &combo, ECaseSensitivity sensitivity, bool blockSignals); ///< Select a case sensitivity in a combo box.
qint32 caseSensitivityToInt(ECaseSensitivity cs); ///< Convert a case sensitivity to an integer.
ECaseSensitivity intToCaseSensitivity(qint32 value, ECaseSensitivity defaultValue); ///< Convert an integer to a case sensitivity, with a default value if conversion is not possible.
ECaseSensitivity intToCaseSensitivity(qint32 value, bool *outOk = nullptr); ///< Convert an integer to a case sensitivity.


#endif // #ifndef BEEFTEXT_CASE_SENSITIVITY_H
