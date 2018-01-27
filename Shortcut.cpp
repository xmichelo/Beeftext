/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of shortcut class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "Shortcut.h"


QString getKeyName(qint32 virtualKeyCode); ///< Get the display name for a key


//**********************************************************************************************************************
/// \param[in] scanCode The scan code for the key
/// \return The name of the key
//**********************************************************************************************************************
QString getKeyName(qint32 scanCode)
{
   qint32 const kBufferSize = 32;
   wchar_t buffer[kBufferSize];
   if (!GetKeyNameText(scanCode << 16, buffer, kBufferSize - 1))
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
/// \note Having the virtual scan code is not an absolute necessity, however, retrieving the display name of a key
/// with the exact scan code gives more result than retrieving it after getting the scan code using MapVirtualKey.
/// 
/// \param[in] modifiers The modifiers for the shortcut
/// \param[in] nativeVirtualKey The native virtual key code
/// \param[in] nativeScanCode The native scan code of the key
//**********************************************************************************************************************
Shortcut::Shortcut(Qt::KeyboardModifiers const& modifiers, quint32 nativeVirtualKey, quint32 nativeScanCode)
   : modifiers_(modifiers)
   , nativeVirtualKey_(nativeVirtualKey)
   , nativeScanCode_(nativeScanCode)
{

}


//**********************************************************************************************************************
/// \return A string describing the shortcut
//**********************************************************************************************************************
QString Shortcut::toString() const
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


//**********************************************************************************************************************
/// \return true if and only if the shortcut is valid
//**********************************************************************************************************************
bool Shortcut::isValid() const
{
   QList<quint32> const forbiddenKeys = { 0, VK_CONTROL, VK_MENU, VK_SHIFT, VK_LWIN, VK_RWIN, VK_APPS };
   if (forbiddenKeys.contains(nativeVirtualKey_))
      return false;
   if (!modifiers_.testFlag(Qt::ControlModifier) && !modifiers_.testFlag(Qt::AltModifier)
      && !modifiers_.testFlag(Qt::MetaModifier))
      return false; // The controls needs at least one Control, Alt of Windows modifier
   return true;
   }  


//**********************************************************************************************************************
/// \return the modifiers field for the shortcut
//**********************************************************************************************************************
Qt::KeyboardModifiers Shortcut::nativeModifiers() const
{
   return modifiers_; 
}


//**********************************************************************************************************************
/// \return The native virtual key code of the shortcut
//**********************************************************************************************************************
quint32 Shortcut::nativeVirtualKey() const
{
   return nativeVirtualKey_;
}


//**********************************************************************************************************************
/// \return The native scan code of the shortcut
//**********************************************************************************************************************
quint32 Shortcut::nativeScanCode() const
{
   return nativeScanCode_;
}

