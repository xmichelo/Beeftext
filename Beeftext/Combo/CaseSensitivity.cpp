/// \file
/// \author 
///
/// \brief Implementation of combo case sensitivity related types and functions
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "CaseSensitivity.h"
#include "PreferencesManager.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {


//**********************************************************************************************************************
/// \brief return a string describing a case sensitivity
/// 
/// \param[in] sensitivity The case sensitivity.
/// \param[in] resolveDefault If the result is default, should the actual value for the default case sensitivity be
/// added to the description.
/// \return A string containing the case sensitivity
//**********************************************************************************************************************
QString caseSensitivityToString(ECaseSensitivity sensitivity, bool resolveDefault)
{
   switch (sensitivity)
   {
   case ECaseSensitivity::Default:
   {
      QString result = QObject::tr("Default");
      if (resolveDefault)
         result += QString(" (%1)").arg(caseSensitivityToString(
            PreferencesManager::instance().defaultCaseSensitivity(), false).toLower());
      return result;
   }
   case ECaseSensitivity::CaseSensitive: return QObject::tr("Case sensitive");
   case ECaseSensitivity::CaseInsensitive: return QObject::tr("Case insensitive");
   default:
      Q_ASSERT(false);
      globals::debugLog().addWarning(QString("Unknown matching mode with value %1").arg(
         static_cast<qint32>(sensitivity)));
      return QObject::tr("<Unknown>");
   }
}



}


//**********************************************************************************************************************
/// \param[in] combo The combo.
/// \param[in] includeDefault Should the 'Default' value be included in the list.
//**********************************************************************************************************************
void fillCaseSensitivityCombo(QComboBox& combo, bool includeDefault)
{
   QSignalBlocker blocker(&combo);
   combo.clear();
   qint32 const startIndex = includeDefault ? 0 : 1;
   for (qint32 i = startIndex; i < static_cast<qint32>(ECaseSensitivity::Count); ++i)
      combo.addItem(caseSensitivityToString(static_cast<ECaseSensitivity>(i), true), i);
}


//**********************************************************************************************************************
/// \param[in] combo The combo.
/// \return The case sensitivity selected in the combo.
//**********************************************************************************************************************
ECaseSensitivity selectedCaseSensitivityInCombo(QComboBox const& combo)
{
   try
   {
      QVariant const currentData = combo.currentData();
      if (currentData.isNull() || (!currentData.canConvert<qint32>()))
         throw xmilib::Exception("Could not find a case sensitivity in a combo.");
      qint32 const intValue = currentData.value<qint32>();
      if ((intValue < 0) || intValue >= static_cast<qint32>(EMatchingMode::Count))
         throw xmilib::Exception(QString("Invalid case sensitivity found in combo (value %1)").arg(intValue));
      return static_cast<ECaseSensitivity>(intValue);
   }
   catch (xmilib::Exception const& e)
   {
      Q_ASSERT(false);
      globals::debugLog().addWarning(e.qwhat());
      return ECaseSensitivity::Default;
   }
}


//**********************************************************************************************************************
/// \param[in] combo The combo box.
/// \param[in] sensitivity The case sensitivity.
/// \param[in] blockSignals should the signals of the combo be blocked before selecting the item.
//**********************************************************************************************************************
void selectCaseSensitivityInCombo(QComboBox& combo, ECaseSensitivity sensitivity, bool blockSignals)
{
   std::unique_ptr<QSignalBlocker> blocker;
   if (blockSignals)
      blocker = std::make_unique<QSignalBlocker>(&combo);
   for (qint32 i = 0; i < combo.count(); ++i)
   {
      bool ok = false;
      qint32 const intValue = combo.itemData(i).toInt(&ok);
      if ((ok) && (sensitivity == static_cast<ECaseSensitivity>(intValue)))
      {
         combo.setCurrentIndex(i);
         return;
      }
   }
}
