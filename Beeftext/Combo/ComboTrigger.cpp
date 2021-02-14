/// \file
/// \author 
///
/// \brief Implementation of functions related to combo triggers.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ComboTrigger.h"
#include "PreferencesManager.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {


   //**********************************************************************************************************************
   /// \brief Internal version of the combo trigger to string converter.
   ///
   /// \param[in] trigger The combo trigger
   /// \param[in] resolveDefault If the mode is 'Defaut', should we retrieve the name of the default trigger and include
   /// it in the description.
   /// \return A string describing the matching mode.
   //**********************************************************************************************************************
   QString comboTriggerToStringInternal(EComboTrigger trigger, bool resolveDefault)
   {
      switch (trigger)
      {
      case EComboTrigger::Default:
      {
         QString result = QObject::tr("Default");
         if (resolveDefault)
            result += QString(" (%1)").arg(comboTriggerToStringInternal(
               PreferencesManager::instance().defaultComboTrigger(), false).toLower());
         return result;
      }
      case EComboTrigger::Automatic: return QObject::tr("Automatic");
      case EComboTrigger::Shortcut: return QObject::tr("Shortcut");
      default:
         Q_ASSERT(false);
         globals::debugLog().addWarning(QString("Unknown matching mode with value %1")
            .arg(static_cast<qint32>(trigger)));
         return QObject::tr("<Unknown>");
      }
   }


}


//**********************************************************************************************************************
/// \param[in] trigger The combo trigger
/// \return A string describing the matching mode.
//**********************************************************************************************************************
QString comboTriggerToString(EComboTrigger trigger)
{
   return comboTriggerToStringInternal(trigger, true);
}


//**********************************************************************************************************************
/// \param[in] combo The combo box.
/// \param[in] includeDefault if true, an entry 'Default' will be included
//**********************************************************************************************************************
void fillComboTriggerCombo(QComboBox& combo, bool includeDefault)
{
   QSignalBlocker blocker(&combo);
   combo.clear();
   qint32 const startIndex = includeDefault ? 0 : 1;
   for (qint32 i = startIndex; i < static_cast<qint32>(EComboTrigger::Count); ++i)
      combo.addItem(comboTriggerToString(EComboTrigger(i)), i);
}


//**********************************************************************************************************************
/// \param[in] combo The combo box.
/// \return The currently selected matching mode in the combo.
//**********************************************************************************************************************
EComboTrigger selectedComboTriggerInCombo(QComboBox const& combo)
{
   try
   {
      QVariant const currentData = combo.currentData();
      if (currentData.isNull() || (!currentData.canConvert<qint32>()))
         throw xmilib::Exception("Could not find a trigger in a combo trigger combo.");
      qint32 const intValue = currentData.value<qint32>();
      if ((intValue < 0) || intValue >= static_cast<qint32>(EComboTrigger::Count))
         throw xmilib::Exception(QString("Invalid combo trigger found in combo (value %1)").arg(intValue));
      return EComboTrigger(intValue);
   }
   catch (xmilib::Exception const& e)
   {
      Q_ASSERT(false);
      globals::debugLog().addWarning(e.qwhat());
      return EComboTrigger::Default;
   }
}


//**********************************************************************************************************************
/// \param[in] combo The combo box.
/// \param[in] trigger The combo trigger.
/// \param[in] blockSignals Should the signals be block for the combo before selecting.
//**********************************************************************************************************************
void selectComboTriggerInCombo(QComboBox& combo, EComboTrigger trigger, bool blockSignals)
{
   std::unique_ptr<QSignalBlocker> blocker;
   if (blockSignals)
      blocker = std::make_unique<QSignalBlocker>(&combo);
   for (qint32 i = 0; i < combo.count(); ++i)
   {
      bool ok = false;
      qint32 const intValue = combo.itemData(i).toInt(&ok);
      if ((ok) && (trigger == static_cast<EComboTrigger>(intValue)))
      {
         combo.setCurrentIndex(i);
         return;
      }
   }
}
