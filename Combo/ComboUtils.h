/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions related to combos
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__UTILS__H
#define BEEFTEXT__COMBO__UTILS__H


#include "Combo.h"


void comboListToJSonDocument(VecSPCombo const& comboList, QJsonDocument& outDoc); ///< Serialize a combo list to a JSon document
bool jsonDocumentToComboList(QJsonDocument const& doc, VecSPCombo& outComboList); ///< Unserialize a combo from a JSon document


#endif // #ifndef BEEFTEXT__COMBO__UTILS__H