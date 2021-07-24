/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextUtils.h"
#include "Combo/ComboManager.h"
#include "SensitiveApplicationManager.h"
#include "InputManager.h"
#include "Preferences/PreferencesManager.h"
#include "BeeftextGlobals.h"
#include "Clipboard/ClipboardManagerDefault.h"
#include <Psapi.h>
#include <XMiLib/SystemUtils.h>
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QString const kPortableModeBeaconFileName = "Portable.bin"; ///< The name of the 'beacon' file used to detect if the application should run in portable mode
QString const kPortableAppsModeBeaconFileName = "PortableApps.bin"; ///< The name of the 'beacon file used to detect if the app is in PortableApps mode
QList<quint16> const modifierKeys = {  VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU, VK_LSHIFT, VK_RSHIFT, VK_LWIN,
   VK_RWIN }; ///< The modifier keys
QChar const kObjectReplacementChar(0xfffc); ///< The unicode object replacement character.



QString richTextMessage() {
   return QObject::tr(R"(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
<html><head><meta name="qrichtext" content="1" /><style type="text/css">
p, li { white-space: pre-wrap; }
</style></head><body style=" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;">
<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">Your combo list contains rich text combos. Beeftext does not support rich text anymore.</p>
<p style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><br /></p>
<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">- Click on the 'Convert' button to convert your rich text combos to plain text.</p>
<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">- Click on the 'Exit' button to quit the application. If you want to keep using rich text combos, you can re-install <a href="https://github.com/xmichelo/Beeftext/releases/tag/v7.2"><span style=" text-decoration: underline; color:#0000ff;">Beeftext v7.2</span></a>.</p></body></html>
)");
}


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
      qApp->thread()->msleep(static_cast<quint32>(delayMs));
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
/// \return true if and only if Beeftext is the application currently in the foreground
//**********************************************************************************************************************
bool isBeeftextTheForegroundApplication()
{
   DWORD processId = 0;
   GetWindowThreadProcessId(GetForegroundWindow(), &processId);
   return QCoreApplication::applicationPid() == processId;
}


//**********************************************************************************************************************
/// \param[in] snippet The snippet.
/// \return The snippet in plain text format.
//**********************************************************************************************************************
QString htmlToPlainText(QString const& snippet)
{
   QTextDocumentFragment const fragment = QTextDocumentFragment::fromHtml(snippet);
   QString plainText = fragment.toPlainText();
   plainText.remove(kObjectReplacementChar); // Remove the 'Object replacement character' that replaced images during conversion to plain text
   return plainText;
}


//**********************************************************************************************************************
/// \param[in] charCount The number of characters to substitute.
/// \param[in] newText The new text.
/// \param[in] cursorPos The position of the cursor in the new text. The value is -1 if the cursor does not need
/// \param[in] source The source that triggered the combo
/// repositionning.
//**********************************************************************************************************************
void performTextSubstitution(qint32 charCount, QString const& newText, qint32 cursorPos,
   ETriggerSource source)
{
   InputManager& inputManager = InputManager::instance();
   PreferencesManager const& prefs = PreferencesManager::instance();
   bool const wasKeyboardHookEnabled = inputManager.setKeyboardHookEnabled(false);
   // we disable the hook to prevent endless recursive substitution

   try
   {
      // we erase the combo
      bool const triggeredByPicker = (ETriggerSource::ComboPicker == source);
      bool const triggersOnSpace = prefs.useAutomaticSubstitution() && prefs.comboTriggersOnSpace();
      QString text = newText + (triggersOnSpace && prefs.keepFinalSpaceCharacter() && (!triggeredByPicker) 
         ? " " : QString());
      QList<quint16> pressedModifiers;
      if (!triggeredByPicker)
      {
         pressedModifiers = backupAndReleaseModifierKeys();
         synthesizeBackspaces(qMax<qint32>(charCount + (triggersOnSpace ? 1 : 0), 0));
         restoreModifierKeys(pressedModifiers);
      }
      if (!SensitiveApplicationManager::instance().isSensitiveApplication(getActiveExecutableFileName()))
      {
         // we use the clipboard to and copy/paste the snippet
         ClipboardManager& clipboardManager = ClipboardManager::instance();
         clipboardManager.backupClipboard();
         clipboardManager.setText(text);
         pressedModifiers = backupAndReleaseModifierKeys(); ///< We artificially depress the current modifier keys
         if (prefs.useShiftInsertForPasting())
         {
            synthesizeKeyDown(VK_LSHIFT);
            synthesizeKeyDownAndUp(VK_INSERT);
            synthesizeKeyUp(VK_LSHIFT);
         }
         else
         {
            synthesizeKeyDown(VK_LCONTROL);
            synthesizeKeyDownAndUp('V');
            synthesizeKeyUp(VK_LCONTROL);
         }
         restoreModifierKeys(pressedModifiers);
         QTimer::singleShot(1000, []() { ClipboardManager::instance().restoreClipboard(); });
         ///< We need to delay clipboard restoration to avoid unexpected behaviours
      }
      else
      {
         // we simulate the typing of the snippet text
         for (QChar c: text)
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
         pressedModifiers = backupAndReleaseModifierKeys(); ///< We artificially depress the current modifier keys
         for (qint32 i = 0; i < qMax<qint32>(0, printableCharacterCount(text) - cursorPos); ++i)
            synthesizeKeyDownAndUp(VK_LEFT);
         restoreModifierKeys(pressedModifiers);
      }
   }
   catch (Exception const&)
   {
      inputManager.setKeyboardHookEnabled(wasKeyboardHookEnabled);
      throw;
   }
   inputManager.setKeyboardHookEnabled(wasKeyboardHookEnabled);
   // debugDisplayModifiersStates();
}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the message box.
/// \param[in] logMessage The message to add to the log.
/// \param[in] userMessage The message to display to the user. if empty, log message is displayed to the user
/// \brief The report contists in writting logMessage to the debug log
//**********************************************************************************************************************
void reportError(QWidget* parent, QString const& logMessage, QString const& userMessage)
{
   globals::debugLog().addError(logMessage);
   QString const usrMsg = userMessage.isEmpty() ? logMessage : userMessage;
   QMessageBox::critical(parent, QObject::tr("Error"), usrMsg);
}


//**********************************************************************************************************************
/// \return true if and only if the application is running on Windows 10 or higher.
//**********************************************************************************************************************
bool isAppRunningOnWindows10OrHigher()
{
   QRegularExpression const re(R"(^(\d+)\.)");
   QRegularExpressionMatch const match = re.match(QSysInfo::kernelVersion());
   return match.hasMatch() ? match.captured(1).toInt() >= 10 : false;
}


//**********************************************************************************************************************
/// The number of printable character can only be estimated because it depends on the way
/// application edit, most notably when it comes to the behaviour of the left arrow key supporting compound emojis
/// (https://eclecticlight.co/2018/03/15/compound-emoji-can-confuse/)
///
/// \param[in] str the string
/// \return The estimated number of characters for the string.
//**********************************************************************************************************************
qint32 printableCharacterCount(QString const& str)
{
   // The easiest way to count characters is to convert the string to UTF-32 (a.k.a. UCS-4)
   
   QVector<quint32> const ucs4 = str.toUcs4();
   // Now we assume that if you use the Zero Width Joiner (U+200D), it is resolved properly
   // We also account for compound emojis made with the skin color tones (Fitzpatrick type)
   qint32 result = qint32(ucs4.size());
   for (quint32 c: ucs4)
   {
      if (c == 0x200d) // zero width joiner
         result -= 2;
      if ((c >= 0x1f3fb) && (c <= 0x1f3ff)) // fitzpatrick scale range
         result -= 1; 
   }
   return qMax<qint32>(0, result);
}


//**********************************************************************************************************************
/// \brief Get the MIME data for a text snippet.
/// 
/// \param[in] text The snippet's text.
/// \return A pointer to heap-allocated MIME data representing the snippet. The caller is responsible for
/// release the allocated memory.
//**********************************************************************************************************************
QMimeData* mimeDataFromText(QString const& text)
{
   QMimeData* result = new QMimeData;
   result->setText(text);
   return result;
}


//**********************************************************************************************************************
/// \param[in] html The HTML content
//**********************************************************************************************************************
QMimeData* mimeDataFromHtml(QString const& html)
{
   QMimeData* result = new QMimeData;
   // we filter the HTML through a QTextDocumentFragment as is prevent errouneous insertion of new line
   // at the beginning and end of the snippet
   result->setHtml(QTextDocumentFragment::fromHtml(html).toHtml());
   result->setText(htmlToPlainText(html));
   return result;
}


//**********************************************************************************************************************
/// \return false if the user opted not to convert combos to plain text and want to exit the application.
//**********************************************************************************************************************
bool warnAndConvertHtmlCombos()
{
   QMessageBox dlg;
   dlg.setWindowTitle(QObject::tr("Warning"));
   dlg.setIcon(QMessageBox::Warning);
   dlg.setText(richTextMessage());
   QPushButton* convertButton = dlg.addButton(QObject::tr("&Convert"), QMessageBox::AcceptRole);
   dlg.addButton(QObject::tr("E&xit"), QMessageBox::RejectRole);
   dlg.setDefaultButton(convertButton);
   return QMessageBox::AcceptRole == dlg.exec();
}


//**********************************************************************************************************************
/// \param[in] color The color.
/// \param[in] includeAlpha Should the alpha channel be included in the hexadecimal string.
/// \return The hex representation of the color.
//**********************************************************************************************************************
QString colorToHex(QColor const& color, bool includeAlpha)
{
   QChar const zero('0');
   QString result = QString("%1%2%3").arg(color.red(), 2, 16, zero).arg(color.green(), 2, 16, zero)
      .arg(color.blue(), 2, 16, zero);
   if (includeAlpha)
      result += QString("%1").arg(color.alpha(), 2, 16, zero);
   return result;
}
