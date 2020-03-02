/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of clipboard manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ClipboardManager.h"
#include "BeeftextUtils.h"
#include <XMiLib/Scoped/ScopedClipboardAccess.h>
#include <XMiLib/Scoped/ScopedGlobalMemoryLock.h>
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QString const kHtmlFormatName = "HTML Format"; ///< The name of the format used for HTML clipboard content. This a a Microsoft convention, do no change it
QString const kRegExpHtmlFormatField = R"(^\s*%1:(-?[\d]+)\s*$)"; ///The regular expression for the 
QString const kHtmlClipboardDescription = R"(Version:0.9
StartHTML:%1
EndHTML:%2
StartFragment:%3
EndFragment:%4
)"; ///< The description of the HTML Clipboard format (see https://docs.microsoft.com/en-us/windows/win32/dataxchg/html-clipboard-format )
} // namespace 


//**********************************************************************************************************************
/// \return The clipboard format for HTML.
//**********************************************************************************************************************
qint32 htmlClipboardFormat()
{
   static qint32 result = RegisterClipboardFormatA(kHtmlFormatName.toLocal8Bit());
   return result;
}


//**********************************************************************************************************************
/// \return The only allowed instance of the class
//**********************************************************************************************************************
ClipboardManager& ClipboardManager::instance()
{
   static ClipboardManager instance;
   return instance;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ClipboardManager::backupClipboard()
{
   backup_.clear();
   ScopedClipboardAccess const sca(nullptr);
   qint32 format = 0;

   while ((format = EnumClipboardFormats(format)))
   {
      SpClipBoardFormatData const cbData = std::make_shared<ClipBoardFormatData>();
      cbData->format = format;
      HANDLE const handle = GetClipboardData(format);
      if (!handle)
         continue;
      ScopedGlobalMemoryLock memLock(handle);
      quint32 const* const data = reinterpret_cast<quint32 const*>(memLock.pointer());
      if (!data)
         continue;

      qint32 const size = GlobalSize(handle);
      if (!size)
         continue;

      cbData->data = QByteArray(size, 0);
      memcpy(cbData->data.data(), data, size);

      backup_.push_back(cbData);
   }
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ClipboardManager::restoreClipboard()
{
   if (!this->hasBackup())
      return;

   ScopedClipboardAccess const sca(nullptr);
   if (!sca.isOpen())
      return;

   EmptyClipboard();
   for (SpClipBoardFormatData const& cbData: backup_)
   {
      if ((!cbData) || (!cbData->data.size()))
         continue;
      qint32 const size = cbData->data.size();
      HANDLE const handle = GlobalAlloc(GMEM_MOVEABLE, size);
      if (!handle)
         continue;
      try
      {
         ScopedGlobalMemoryLock memLock(handle);
         quint32* const data = reinterpret_cast<quint32*>(memLock.pointer());
         if (!data)
            throw Exception();
         memcpy(data, cbData->data.data(), size);
         if (!SetClipboardData(cbData->format, handle))
            throw Exception();
      }
      catch (Exception const&)
      {
         ScopedGlobalMemoryLock const memLock(handle);
         GlobalFree(handle);
      }
   }

   backup_.clear();
}


//**********************************************************************************************************************
/// \return true if and only if the clipboard manager contains a clipboard backup
//**********************************************************************************************************************
bool ClipboardManager::hasBackup() const
{
   return !backup_.empty();
}


//**********************************************************************************************************************
/// \ return The text value of the clipboard. If the clipboard does not contain text, an empty string is returned.
//**********************************************************************************************************************
QString ClipboardManager::text()
{
   ScopedClipboardAccess const sca(nullptr);
   if ((!sca.isOpen()) || (!IsClipboardFormatAvailable(CF_UNICODETEXT))) // Note system does automatic conversion from CF_OEMTEXT and CF_TEXT to CF_UNICODETEXT
      return QString();
   HANDLE const handle = GetClipboardData(CF_UNICODETEXT);
   if (!handle)
      return QString();
   ScopedGlobalMemoryLock const memlock(handle);
   quint32 const* const data = reinterpret_cast<quint32 const*>(memlock.pointer());
   if (!data)
      return QString();
   qint32 const size = GlobalSize(handle);
   if (!size)
      return QString();
   QByteArray array(size, 0);
   memcpy(array.data(), data, size);
   return QString::fromUtf16(reinterpret_cast<quint16 const*>(array.data()), (size - 1) / 2); // (size - 1) because we discard the final `0x0000`
}


//**********************************************************************************************************************
/// \brief Allocate space in global memory and put the given text in UTF-16 format in it.
///
/// \param[in] text The plain text.
/// \return A handle to the global memory containing the text in UTF-8 format.
/// \return 
//**********************************************************************************************************************
HANDLE putUtf16InGlobalMemory(QString const& text)
{
   if (text.isEmpty())
      return nullptr;
   quint16 const* const data = text.utf16();
   quint32 const size = (text.length() + 1) * 2;
   HANDLE const handle = GlobalAlloc(GMEM_MOVEABLE, (text.length() + 1) * 2);
   if (!handle)
      return nullptr;
   ScopedGlobalMemoryLock const memLock(handle);
   quint8* const pointer = reinterpret_cast<quint8*>(memLock.pointer());
   if (!pointer)
   {
      GlobalFree(handle);
      return nullptr;
   }
   memcpy(pointer, data, size);
   pointer[size - 1] = 0;
   pointer[size - 2] = 0; // for safety
   return handle;
}


//**********************************************************************************************************************
/// \brief Allocate space in global memory and put the given text in UTF-8 format in it.
///
/// \param[in] text The text
/// \return A handle to the global memory containing the text in UTF-8 format.
/// \return A null handle if an error occurred
//**********************************************************************************************************************
HANDLE putUtf8InGlobalMemory(QString const& text)
{
   if (text.isEmpty())
      return nullptr;
   QByteArray const data = text.toUtf8() + char(0);
   qint32 const size = data.size();
   HANDLE const handle = GlobalAlloc(GMEM_MOVEABLE, size);
   if (!handle)
      return nullptr;
   ScopedGlobalMemoryLock const memLock(handle);
   quint8* const pointer = reinterpret_cast<quint8*>(memLock.pointer());
   if (!pointer)
   {
      GlobalFree(handle);
      return nullptr;
   }
   memcpy(pointer, data, size);
   return handle;
}


//**********************************************************************************************************************
/// \param[in] text The text to put in the clipboard.
/// \return true if and only if the operation was successful.
//**********************************************************************************************************************
bool ClipboardManager::setText(QString const& text)
{
   HANDLE const handle = putUtf16InGlobalMemory(text);
   if (!handle)
      return false;
   try
   {
      ScopedClipboardAccess const sca(nullptr);
      if (!sca.isOpen())
         throw Exception();

      EmptyClipboard();
      bool const result = SetClipboardData(CF_UNICODETEXT, handle);
      if (!result)
         throw Exception();
      return true;
   }
   catch (Exception const&)
   {
      ScopedGlobalMemoryLock const memLock(handle);
      GlobalFree(handle);
      return false;
   }
}


//**********************************************************************************************************************
/// \extract a number from an HTML clipboard data description field (which have the form 'fieldName:00000123'
/// \param[in] clipboardData The HTML format data read from the clipboard
/// \param[in] fieldName The name of the field.
/// \return The value read from the field.
/// \return -1 if the field could not be found
//**********************************************************************************************************************
qint32 numberFieldFromHtmlClipboardData(QString const& clipboardData, QString const& fieldName)
{
   QRegularExpression const regExp(kRegExpHtmlFormatField.arg(fieldName), 
      QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
   QRegularExpressionMatch const match = regExp.match(clipboardData);
   if (!match.hasMatch())
      return -1;
   bool ok = false;
   quint32 result = 0;
   result = match.captured(1).toInt(&ok);
   return ok ? result : -1;
}


//**********************************************************************************************************************
/// \brief Extract the HTML content for HTML clipboard data. The clipboard format for HTML is detailed here:
/// https://docs.microsoft.com/en-us/windows/win32/dataxchg/html-clipboard-format
/// 
/// \param[in] clipboardData The raw HTML data obtained from the clipboard.
/// \return The HTML content. If extraction fails, the function returns an empty string.
//**********************************************************************************************************************
QString extractHtmlFromClipboardData(QString const& clipboardData)
{
   qint32 const startHtml = numberFieldFromHtmlClipboardData(clipboardData, "StartHTML");
   qint32 const endHtml = numberFieldFromHtmlClipboardData(clipboardData, "EndHTML");
   qint32 const startFrag = numberFieldFromHtmlClipboardData(clipboardData, "StartFragment");
   qint32 const endFrag = numberFieldFromHtmlClipboardData(clipboardData, "EndFragment");
   if ((startHtml >= 0) && (endHtml >= 0))
      return   clipboardData.mid(startHtml, endHtml - startHtml + 1);
   return (startFrag < 0) || (endFrag < 0) ? QString() : clipboardData.mid(startFrag, endFrag - startFrag + 1);
}


//**********************************************************************************************************************
/// \ return The HTML value of the clipboard. If the clipboard does not contain HTML, an empty string is returned.
//**********************************************************************************************************************
QString ClipboardManager::html()
{
   ScopedClipboardAccess const sca(nullptr);
   quint32 const htmlFormat = htmlClipboardFormat();
   if ((!sca.isOpen()) || (!IsClipboardFormatAvailable(htmlFormat))) // Note system does automatic conversion from CF_OEMTEXT and CF_TEXT to CF_UNICODETEXT
      return QString();
   HANDLE const handle = GetClipboardData(htmlFormat);
   if (!handle)
      return QString();
   ScopedGlobalMemoryLock const memlock(handle);
   quint32 const* const data = reinterpret_cast<quint32 const*>(memlock.pointer());
   if (!data)
      return QString();
   qint32 const size = GlobalSize(handle);
   if (!size)
      return QString();
   QByteArray array(size, 0);
   memcpy(array.data(), data, size);
   return extractHtmlFromClipboardData(QString::fromUtf8(array.data(), size - 1)); // (size - 1) because we discard the final `0x0000`

}


//**********************************************************************************************************************
/// \brief Create a 0-padded 10 characters long string containing the given number.
///
/// \param[in] number The number
/// \return a zero-padded ten characters long string containing the number
//**********************************************************************************************************************
QString numberTo10CharsString(qint32 number)
{
   if (number < 0)
      return "-000000001";
   return QString("%1").arg(number, 10, 10, QChar('0'));
}


//**********************************************************************************************************************
/// \brief Generate the HTML format for the clipboard.
/// For details, see https://docs.microsoft.com/en-us/windows/win32/dataxchg/html-clipboard-format
///
/// \param[in] html The HTML content.
/// \return The HTML format for the clipboard.
//**********************************************************************************************************************
QString generateHtmlFormatContent(QString const& html)
{
   qint32 const htmlClipboardDescriptionSize = kHtmlClipboardDescription.size() + 4 * 8; /// The size of the clipboard description, knowing that the 6 '%n' arguments will be replace by 10 digits (e.g. 0000000123)
   QString result = kHtmlClipboardDescription + html;
   result = result.arg(numberTo10CharsString(htmlClipboardDescriptionSize))
      .arg(numberTo10CharsString(htmlClipboardDescriptionSize + html.size() + 1)); // positions of StartHTML and EndHTML
   QRegularExpression const regExp(R"(<body.*>)", QRegularExpression::CaseInsensitiveOption | 
      QRegularExpression::InvertedGreedinessOption);
   QRegularExpressionMatch const match = regExp.match(html);
   if (!match.hasMatch())
      return QString();
   qDebug() << QString("Match: %1").arg(match.captured(0));
   qint32 const indexStartFrag = htmlClipboardDescriptionSize + match.capturedEnd(0);
   qint32 const indexEndFrag = html.indexOf("</body", 0, Qt::CaseInsensitive) + htmlClipboardDescriptionSize;

   result = result.arg(numberTo10CharsString(indexStartFrag)).arg(numberTo10CharsString(indexEndFrag));
   qDebug() << "Full Message:" + result;
   QString temp = result.left(htmlClipboardDescriptionSize + html.size());
   temp = temp.right(temp.size() - htmlClipboardDescriptionSize);
   qDebug() << "HTML:" + temp;

   temp = result.left(indexEndFrag);
   temp = temp.right(indexStartFrag);
   qDebug() << "Frag:" + temp;
   return result;

}


//**********************************************************************************************************************
/// \param[in] html The HTML data.
/// \return true if and only if the HTML data was successfully copied to the clipboard
//**********************************************************************************************************************
bool ClipboardManager::setHtml(QString const& html)
{
   QString const htmlContent = generateHtmlFormatContent(html);
   QString const textContent = snippetToPlainText(html, true);
   HANDLE htmlHandle = nullptr, textHandle = nullptr;

   try
   {
      htmlHandle = putUtf8InGlobalMemory(htmlContent);
      if (!htmlHandle)
         throw Exception();
      textHandle = putUtf16InGlobalMemory(textContent);
      if (!textHandle)
         throw Exception();
      ScopedClipboardAccess sca(nullptr);
      EmptyClipboard();
      if (!SetClipboardData(htmlClipboardFormat(), htmlHandle))
         throw Exception();
      htmlHandle = nullptr; // Clipboard now owns the global memory storing the HTML content
      if (!SetClipboardData(CF_UNICODETEXT, textHandle))
         throw Exception();
      return true;
   }
   catch (Exception const&)
   {
      if (htmlHandle)
      {
         ScopedGlobalMemoryLock htmlLock(htmlHandle);
         GlobalFree(htmlHandle);
      }
      if (textHandle)
      {
         ScopedGlobalMemoryLock textLock(htmlHandle);
         GlobalFree(textHandle);
      }
      return false;
   }
}
