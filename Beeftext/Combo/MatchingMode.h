/// \file
/// \author 
///
/// \brief Declaration of functions related to the matching mode.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_MATCHING_MODE_H
#define BEEFTEXT_MATCHING_MODE_H


//****************************************************************************************************************************************************
/// \brief An enumeration for matching mode
//****************************************************************************************************************************************************
enum class EMatchingMode : qint32 {
    Default = 0, ///< The default matching mode, defined in the application's preferences.
    Strict = 1, ///< Strict matching.
    Loose = 2, ///< Loose matching.
    Count = 3, ///< The number of entries in the matching mode list
};


void fillMatchingModeCombo(QComboBox &combo, bool includeDefault); ///< Fill a combo with the matching mode entries
EMatchingMode selectedMatchingModeInCombo(QComboBox const &combo); ///< Retrieve the value of the selected matching mode in a combo
void selectMatchingModeInCombo(QComboBox &combo, EMatchingMode mode, bool blockSignals); ///< Select a mode in a combo box.


#endif // #ifndef BEEFTEXT_MATCHING_MODE_H
