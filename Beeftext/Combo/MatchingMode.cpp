/// \file
/// \author 
///
/// \brief Implementation of functions related to the matching mode.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "MatchingMode.h"
#include "BeeftextGlobals.h"
#include "Preferences/PreferencesManager.h"
#include <XMiLib/Exception.h>


namespace {


//****************************************************************************************************************************************************
/// \brief Internal version of the matching mode.
///
/// \param[in] mode The matching mode.
/// \param[in] resolveDefault If the mode is 'Defaut', should we retrieve the name of the default mode and include
/// it in the description.
/// \return A string describing the matching mode.
//****************************************************************************************************************************************************
QString matchingModeToStringInternal(EMatchingMode mode, bool resolveDefault) {
    switch (mode) {
    case EMatchingMode::Default: {
        QString result = QObject::tr("Default");
        if (resolveDefault)
            result += QString(" (%1)").arg(matchingModeToStringInternal(
                PreferencesManager::instance().defaultMatchingMode(), false).toLower());
        return result;
    }
    case EMatchingMode::Strict:
        return QObject::tr("Strict");
    case EMatchingMode::Loose:
        return QObject::tr("Loose");
    default:
        Q_ASSERT(false);
        globals::debugLog().addWarning(QString("Unknown matching mode with value %1").arg(static_cast<qint32>(mode)));
        return QObject::tr("<Unknown>");
    }
}


//****************************************************************************************************************************************************
/// \param[in] mode The matching mode.
/// \return A string describing the matching mode.
//****************************************************************************************************************************************************
QString matchingModeToString(EMatchingMode mode) {
    return matchingModeToStringInternal(mode, true);
}


}


//****************************************************************************************************************************************************
/// \param[in] combo The combo box.
/// \param[in] includeDefault if true, an entry 'Default' will be included
//****************************************************************************************************************************************************
void fillMatchingModeCombo(QComboBox &combo, bool includeDefault) {
    QSignalBlocker blocker(&combo);
    combo.clear();
    qint32 const startIndex = includeDefault ? 0 : 1;
    for (qint32 i = startIndex; i < static_cast<qint32>(EMatchingMode::Count); ++i)
        combo.addItem(matchingModeToString(static_cast<EMatchingMode>(i)), i);
}


//****************************************************************************************************************************************************
/// \param[in] combo The combo box.
/// \return The currently selected matching mode in the combo.
//****************************************************************************************************************************************************
EMatchingMode selectedMatchingModeInCombo(QComboBox const &combo) {
    try {
        QVariant const currentData = combo.currentData();
        if (currentData.isNull() || (!currentData.canConvert<qint32>()))
            throw xmilib::Exception("Could not find a matching mode in a matching mode combo.");
        qint32 const intValue = currentData.value<qint32>();
        if ((intValue < 0) || intValue >= static_cast<qint32>(EMatchingMode::Count))
            throw xmilib::Exception(QString("Invalid matching mode found in combo (value %1)").arg(intValue));
        return static_cast<EMatchingMode>(intValue);
    }
    catch (xmilib::Exception const &e) {
        Q_ASSERT(false);
        globals::debugLog().addWarning(e.qwhat());
        return EMatchingMode::Default;
    }
}


//****************************************************************************************************************************************************
/// \param[in] combo The combo box.
/// \param[in] mode The matching mode.
/// \param[in] blockSignals Should the signals be block for the combo before selecting.
//****************************************************************************************************************************************************
void selectMatchingModeInCombo(QComboBox &combo, EMatchingMode mode, bool blockSignals) {
    std::unique_ptr<QSignalBlocker> blocker;
    if (blockSignals)
        blocker = std::make_unique<QSignalBlocker>(&combo);
    for (qint32 i = 0; i < combo.count(); ++i) {
        bool ok = false;
        qint32 const intValue = combo.itemData(i).toInt(&ok);
        if ((ok) && (mode == static_cast<EMatchingMode>(intValue))) {
            combo.setCurrentIndex(i);
            return;
        }
    }
}
