/// \file
/// \author 
///
/// \brief Declaration of functions related to combo triggers.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_COMBO_TRIGGER_H
#define BEEFTEXT_COMBO_TRIGGER_H


//**********************************************************************************************************************
/// \brief An enumeration for combo triggers.
//**********************************************************************************************************************
enum class EComboTrigger: qint32 {
   Default = 0, ///< The default combo trigger.
   Automatic = 1, ///< Automatic trigger.
   Shortcut = 2, ///< Trigger by shortcut.
   Count = 3, ///< The number of triggers.
};


QString comboTriggerToString(EComboTrigger trigger); ///< Retrieve a string describing a combo trigger.
void fillComboTriggerCombo(QComboBox& combo, bool includeDefault); ///< Fill a combo with the combo trigger entries.
EComboTrigger selectedComboTriggerInCombo(QComboBox const& combo); ///< Retrieve the value of the selected combo trigger in a combo box.
void selectComboTriggerInCombo(QComboBox& combo, EComboTrigger trigger, bool blockSignals); ///< Select a combo trigger in a combo box.


#endif // #ifndef BEEFTEXT_COMBO_TRIGGER_H
