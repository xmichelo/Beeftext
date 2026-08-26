/// \file
/// \author 
///
/// \brief Declaration of combo case sensitivity related types and functions
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef ABBREVIO_CASE_SENSITIVITY_H
#define ABBREVIO_CASE_SENSITIVITY_H


enum class ECaseSensitivity {
    Default = 0, ///< The default case sensitivity.
    CaseSensitive = 1, ///< Case-sensitive.
    CaseInsensitive = 2, ///< Case-insensitive.
    CaseInsensitiveMatchCase = 3, ///< Case-insensitive, and the case of the snippet is adjusted to match how the keyword was typed.
    Count = 4 ///< The number of items in the enumeration.
};


/// \brief The case 'shape' detected in a piece of typed text, used to reproduce it in the expanded snippet.
enum class ETypedCaseStyle {
    Verbatim = 0, ///< Use the snippet exactly as stored (e.g. keyword was typed in lower case, or in a case pattern that does not fit the other styles).
    AllUpper = 1, ///< The keyword was typed in all upper case (e.g. "HWOR"); the snippet should be upper-cased.
    FirstUpper = 2, ///< The keyword was typed with an upper-case first letter and no other upper-case letter (e.g. "Hwor"); only the snippet's first letter should be upper-cased.
};


void fillCaseSensitivityCombo(QComboBox &combo, bool includeDefault); ///< Fill a combo with the case sensitivity entries.
ECaseSensitivity selectedCaseSensitivityInCombo(QComboBox const &combo); ///< Retrieve the value of the selected case sensitivity in a combo.
void selectCaseSensitivityInCombo(QComboBox &combo, ECaseSensitivity sensitivity, bool blockSignals); ///< Select a case sensitivity in a combo box.
qint32 caseSensitivityToInt(ECaseSensitivity cs); ///< Convert a case sensitivity to an integer.
ECaseSensitivity intToCaseSensitivity(qint32 value, ECaseSensitivity defaultValue); ///< Convert an integer to a case sensitivity, with a default value if conversion is not possible.
ECaseSensitivity intToCaseSensitivity(qint32 value, bool *outOk = nullptr); ///< Convert an integer to a case sensitivity.
ETypedCaseStyle detectTypedCaseStyle(QString const &typedText); ///< Detect the case style of a piece of typed text.
QString applyCaseStyle(QString const &text, ETypedCaseStyle style); ///< Apply a case style to a piece of text.


#endif // #ifndef ABBREVIO_CASE_SENSITIVITY_H
