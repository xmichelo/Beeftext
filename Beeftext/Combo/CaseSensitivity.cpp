/// \file
/// \author 
///
/// \brief Implementation of combo case sensitivity related types and functions
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "CaseSensitivity.h"
#include "Preferences/PreferencesManager.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {


//****************************************************************************************************************************************************
/// \brief return a string describing a case sensitivity
/// 
/// \param[in] sensitivity The case sensitivity.
/// \param[in] resolveDefault If the result is default, should the actual value for the default case sensitivity be
/// added to the description.
/// \return A string containing the case sensitivity
//****************************************************************************************************************************************************
QString caseSensitivityToString(ECaseSensitivity sensitivity, bool resolveDefault) {
    switch (sensitivity) {
    case ECaseSensitivity::Default: {
        QString result = QObject::tr("Default");
        if (resolveDefault)
            result += QString(" (%1)").arg(caseSensitivityToString(
                PreferencesManager::instance().defaultCaseSensitivity(), false).toLower());
        return result;
    }
    case ECaseSensitivity::CaseSensitive:
        return QObject::tr("Case-sensitive");
    case ECaseSensitivity::CaseInsensitive:
        return QObject::tr("Case-insensitive");
    default:
        Q_ASSERT(false);
        globals::debugLog().addWarning(QString("Unknown matching mode with value %1").arg(
            caseSensitivityToInt(sensitivity)));
        return QObject::tr("<Unknown>");
    }
}


}


//****************************************************************************************************************************************************
/// \param[in] combo The combo.
/// \param[in] includeDefault Should the 'Default' value be included in the list.
//****************************************************************************************************************************************************
void fillCaseSensitivityCombo(QComboBox &combo, bool includeDefault) {
    QSignalBlocker blocker(&combo);
    combo.clear();
    qint32 const startIndex = includeDefault ? 0 : 1;
    for (qint32 i = startIndex; i < caseSensitivityToInt(ECaseSensitivity::Count); ++i)
        combo.addItem(caseSensitivityToString(intToCaseSensitivity(i, ECaseSensitivity::CaseSensitive), true), i);
}


//****************************************************************************************************************************************************
/// \param[in] combo The combo.
/// \return The case sensitivity selected in the combo.
//****************************************************************************************************************************************************
ECaseSensitivity selectedCaseSensitivityInCombo(QComboBox const &combo) {
    QVariant const currentData = combo.currentData();
    if (currentData.isNull() || (!currentData.canConvert<qint32>()))
        throw xmilib::Exception("Could not find a case sensitivity in a combo.");
    qint32 const intValue = currentData.value<qint32>();
    bool ok = false;
    ECaseSensitivity const result = intToCaseSensitivity(intValue, &ok);
    if (!ok) {
        Q_ASSERT(false);
        globals::debugLog().addWarning(QString("Invalid case sensitivity found in combo (value %1)").arg(intValue));
    }
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] combo The combo box.
/// \param[in] sensitivity The case sensitivity.
/// \param[in] blockSignals should the signals of the combo be blocked before selecting the item.
//****************************************************************************************************************************************************
void selectCaseSensitivityInCombo(QComboBox &combo, ECaseSensitivity sensitivity, bool blockSignals) {
    std::unique_ptr<QSignalBlocker> blocker;
    if (blockSignals)
        blocker = std::make_unique<QSignalBlocker>(&combo);
    for (qint32 i = 0; i < combo.count(); ++i) {
        bool ok = false;
        qint32 const intValue = combo.itemData(i).toInt(&ok);
        if (!ok)
            continue;
        ECaseSensitivity const cs = intToCaseSensitivity(intValue, &ok);
        if (!ok)
            continue;
        if (sensitivity == cs) {
            combo.setCurrentIndex(i);
            return;
        }
    }
}


//****************************************************************************************************************************************************
/// \param[in] cs The case sensitivity.
/// \return the int value of the case sensitivity.
//****************************************************************************************************************************************************
qint32 caseSensitivityToInt(ECaseSensitivity cs) {
    return static_cast<qint32>(cs);
}


//****************************************************************************************************************************************************
/// \param[in] value The integer value.
/// \param[in] defaultValue The default value to return.
/// \return The result of the conversion.
//****************************************************************************************************************************************************
ECaseSensitivity intToCaseSensitivity(qint32 value, ECaseSensitivity defaultValue) {
    return ((value >= 0) && (value < caseSensitivityToInt(ECaseSensitivity::Count)))
           ? static_cast<ECaseSensitivity>(value) : defaultValue;
}


//****************************************************************************************************************************************************
/// \param[in] value The integer value.
/// \param[out] outOk on exit, this value indicate whether or not the conversion was successful
/// \return The result of the conversion. If outOk is false, the return value is undetermined.
//****************************************************************************************************************************************************
ECaseSensitivity intToCaseSensitivity(qint32 value, bool *outOk) {
    bool const ok = ((value >= 0) && (value < caseSensitivityToInt(ECaseSensitivity::Count)));
    if (outOk)
        *outOk = ok;
    return ok ? static_cast<ECaseSensitivity>(value) : ECaseSensitivity::CaseSensitive;
}
