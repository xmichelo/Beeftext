/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of shortcut class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "Shortcut.h"


QString getKeyName(qint32 scanCode); ///< Get the display name for a key


//**********************************************************************************************************************
/// \param[in] scanCode The scan code for the key
/// \return The name of the key
//**********************************************************************************************************************
QString getKeyName(qint32 scanCode)
{
   wchar_t buffer[32];
   if (!GetKeyNameText(scanCode << 16, buffer, 31))
      return QString();
   QString raw = QString::fromWCharArray(buffer);
   QStringList words = raw.split(QRegularExpression("\\s"), QString::SkipEmptyParts);
   for (QString& wrd : words)
   {
      wrd = wrd.toLower();
      if (wrd.size() > 0)
         wrd[0] = wrd[0].toUpper();
   }
   return words.join(' ');
}


//**********************************************************************************************************************
/// \param[in] modifiers The modifiers for the shortcut
/// \param[in] nativeVirtualKey The native virtual key code
/// \param[in] nativeScanCode The native scan code
//**********************************************************************************************************************
Shortcut::Shortcut(Qt::KeyboardModifiers const& modifiers, quint32 nativeVirtualKey, quint32 nativeScanCode)
   : modifiers_(modifiers)
   , nativeVirtualKey_(nativeVirtualKey)
   , nativeScanCode_(nativeScanCode)
{

}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
QString Shortcut::toString()
{
   QString result;
   if (modifiers_ & Qt::ControlModifier)
      result = QObject::tr("Ctrl") + "+";
   if (modifiers_ & Qt::AltModifier)
      result += QObject::tr("Alt") + "+";
   if (modifiers_ & Qt::ShiftModifier)
      result += QObject::tr("Shift") + "+";
   if (modifiers_ & Qt::MetaModifier)
      result += QObject::tr("Win") + "+";
   return result + getKeyName(nativeScanCode_);
}


