/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextUtils.h"
#include "SensitiveApplicationManager.h"
#include "InputManager.h"
#include "ClipboardManager.h"
#include "PreferencesManager.h"
#include "BeeftextGlobals.h"
#include <Psapi.h>
#include <XMiLib/SystemUtils.h>
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QString const kPortableModeBeaconFileName = "Portable.bin"; ///< The name of the 'beacon' file used to detect if the application should run in portable mode
QString const kPortableAppsModeBeaconFileName = "PortableApps.bin"; ///< The name of the 'beacon file used to detect if the app is in PortableApps mode
QList<quint16> const modifierKeys = {  VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU, VK_LSHIFT, VK_RSHIFT, VK_LWIN,
   VK_RWIN }; ///< The modifier keys


//**********************************************************************************************************************
/// \brief Test if the application is running in portable mode
/// 
/// \return true if and only if the application is running in portable mode
//**********************************************************************************************************************
bool isInPortableModeInternal()
{
   QDir const appDir(QCoreApplication::applicationDirPath());
   return QFileInfo(appDir.absoluteFilePath(kPortableModeBeaconFileName)).exists() ||
      QFileInfo(appDir.absoluteFilePath(kPortableAppsModeBeaconFileName)).exists();
}


//**********************************************************************************************************************
/// \brief Retrieve the list of currently pressed modifier key and synthesize a key release event for each of them
///
/// \return The list of modifier keys that are pressed
//**********************************************************************************************************************
QList<quint16> backupAndReleaseModifierKeys()
{
   QList<quint16> result;
   for (quint16 key: modifierKeys)
      if (GetKeyState(key) < 0)
      {
         result.append(key);
         synthesizeKeyUp(key);
      }
   return result;
}


//**********************************************************************************************************************
/// \brief Restore the specified modifier keys state by generating a key press event for each of them
///
/// \param[in] keys The list of modifiers key to restore by generating a key press event
//**********************************************************************************************************************
void restoreModifierKeys(QList<quint16> const& keys)
{
   for (quint16 key: keys)
      synthesizeKeyDown(key);
}


//**********************************************************************************************************************
/// \brief Wait between keystroke, for an amount time defined in the preferences.
//**********************************************************************************************************************
void waitBetweenKeystrokes()
{
   qint32 const delayMs = PreferencesManager::instance().delayBetweenKeystrokesMs();
   if (delayMs > 0)
      qApp->thread()->msleep(delayMs);
}


}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void openLogFile()
{
   QDesktopServices::openUrl(QUrl::fromLocalFile(globals::logFilePath()));
}


//**********************************************************************************************************************
/// \return true if and only if the application is in portable mode
//**********************************************************************************************************************
bool isInPortableMode()
{
   // portable mode state cannot change during the execution of an instance of the application, so we 'cache' the
   // value using a static variable
   static bool result = isInPortableModeInternal();
   return result;
}


//**********************************************************************************************************************
/// \return true if the application is running as part of the PortableApps.com distribution
//**********************************************************************************************************************
bool usePortableAppsFolderLayout()
{
   return QFileInfo(QDir(QCoreApplication::applicationDirPath())
      .absoluteFilePath(kPortableAppsModeBeaconFileName)).exists();
}


//**********************************************************************************************************************
/// \return The name of the currently active application, including its extension (e.g. "explorer.exe")
/// \return A null string in case of failure
//**********************************************************************************************************************
QString getActiveExecutableFileName()
{
   WCHAR buffer[MAX_PATH + 1] = { 0 };
   DWORD processId = 0;
   GetWindowThreadProcessId(GetForegroundWindow(), &processId);
   // ReSharper disable once CppLocalVariableMayBeConst
   HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
   if (!processHandle)
      return QString();
   bool const ok = GetModuleFileNameEx(processHandle, nullptr, buffer, MAX_PATH);
   CloseHandle(processHandle);
   return ok ? QFileInfo(QDir::fromNativeSeparators(QString::fromWCharArray(buffer))).fileName() : QString();
}


//**********************************************************************************************************************
/// \param[in] charCount The number of characters to substitute
/// \param[in] newText The new text
/// \param[in] cursorPos The position of the cursor in the new text. The value is -1 if the cursor does not need 
/// repositionning.
//**********************************************************************************************************************
void performTextSubstitution(qint32 charCount, QString const& newText, qint32 cursorPos)
{
   InputManager& inputManager = InputManager::instance();
   bool const wasKeyboardHookEnabled = inputManager.setKeyboardHookEnabled(false);
   // we disable the hook to prevent endless recursive substitution
   try
   {
      // we erase the combo
      synthesizeBackspaces(qMax<qint32>(charCount, 0));
      if (!SensitiveApplicationManager::instance().isSensitiveApplication(getActiveExecutableFileName()))
      {
         // we use the clipboard to and copy/paste the snippet
         ClipboardManager& clipboardManager = ClipboardManager::instance();
         clipboardManager.backupClipboard();
         QApplication::clipboard()->setText(newText);
         QList<quint16> const pressedModifiers = backupAndReleaseModifierKeys(); ///< We artificially depress the current modifier keys
         synthesizeKeyDown(VK_LCONTROL);
         synthesizeKeyDownAndUp('V');
         synthesizeKeyUp(VK_LCONTROL);
         restoreModifierKeys(pressedModifiers);
         QTimer::singleShot(1000, []() { ClipboardManager::instance().restoreClipboard(); });
         ///< We need to delay clipboard restoration to avoid unexpected behaviours
      }
      else
      {
         QList<quint16> pressedModifiers;
         // we simulate the typing of the snippet text
         for (QChar c: newText)
         {
            if (c == QChar::LineFeed)
               // synthesizeUnicode key down does not handle line feed properly (the problem actually comes from Windows API's SendInput())
            {
               pressedModifiers = backupAndReleaseModifierKeys();
               synthesizeKeyDownAndUp(VK_RETURN);
               restoreModifierKeys(pressedModifiers);
               waitBetweenKeystrokes();
            }
            else
            {
               pressedModifiers = backupAndReleaseModifierKeys();
               synthesizeUnicodeKeyDownAndUp(c.unicode());
               restoreModifierKeys(pressedModifiers);
               waitBetweenKeystrokes();
            }
         }
      }

      // position the cursor if needed by typing the right amount of left key strokes
      if (cursorPos >= 0)
      {
         QList<quint16> const pressedModifiers = backupAndReleaseModifierKeys(); ///< We artificially depress the current modifier keys
         for (qint32 i = 0; i < qMax<qint32>(0, newText.size() - cursorPos); ++i)
            synthesizeKeyDownAndUp(VK_LEFT);
         restoreModifierKeys(pressedModifiers);
      }

      ///< We restore the modifiers that we deactivated at the beginning of the function
   }
   catch (Exception const&)
   {
      inputManager.setKeyboardHookEnabled(wasKeyboardHookEnabled);
      throw;
   }
   inputManager.setKeyboardHookEnabled(wasKeyboardHookEnabled);
}


//**********************************************************************************************************************
/// \brief The report constists in writting logMessage to the debug log
//**********************************************************************************************************************
void reportError(QWidget* parent, QString const& logMessage, QString const& userMessage)
{
   globals::debugLog().addError(logMessage);
   QMessageBox::critical(parent, QObject::tr("Error"), userMessage);
}

