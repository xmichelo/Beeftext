/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of clipboard manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ClipboardManagerDefault.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Scoped/ScopedClipboardAccess.h>
#include <XMiLib/Scoped/ScopedGlobalMemoryLock.h>
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QList<quint32> const kIgnoredClipboardFormats { CF_ENHMETAFILE }; ///< File formats that we ignore when backing up the clipboard.
QString const kHtmlFormatName = "HTML Format"; ///< The name of the format used for HTML clipboard content. This a a Microsoft convention, do no change it
QString const kRegExpHtmlFormatField = R"(^\s*%1:(-?[\d]+)\s*$)"; ///The regular expression for the HTML format field.


} // namespace




//****************************************************************************************************************************************************
/// \return The clipboard format for HTML.
//****************************************************************************************************************************************************
quint32 htmlClipboardFormat() {
    static quint32 result = RegisterClipboardFormatA(kHtmlFormatName.toLocal8Bit());
    return result;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
ClipboardManagerDefault::ClipboardManagerDefault()
   : ClipboardManager()
{
}

//****************************************************************************************************************************************************
/// \return The type of the clipboard manager
//****************************************************************************************************************************************************
ClipboardManager::EType ClipboardManagerDefault::type() const {
    return EType::Default;
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ClipboardManagerDefault::backupClipboard() {
    try {
        backup_.clear();
        this->freeBitmapBackup();

        ScopedClipboardAccess const sca(nullptr);
        quint32 format = 0;

        while ((format = EnumClipboardFormats(format))) {
            if (kIgnoredClipboardFormats.contains(format))
                continue;
            
            SpClipBoardFormatData const cbData = std::make_shared<ClipBoardFormatData>();
            cbData->format = format;
            HANDLE const handle = GetClipboardData(format);
            if (!handle)
                continue;

            if (format == CF_BITMAP) { // Bitmaps require a special treatment (crashes on Windows 11). See https://devblogs.microsoft.com/oldnewthing/20071026-00/?p=24683
                setBitmapBackup(handle);
                continue;
            }

            ScopedGlobalMemoryLock memLock(handle);
            quint32 const *const data = static_cast<quint32 const *>(memLock.pointer());
            if (!data)
                continue;


            SIZE_T const size = GlobalSize(handle);
            if (!size)
                continue;

            cbData->data = QByteArray(qsizetype(size), 0);
            memcpy(cbData->data.data(), data, size_t(size));

            backup_.push_back(cbData);
        }
    } catch (Exception const &e) {
        globals::debugLog().addError(QString("%1: %2").arg(__FUNCTION__, e.qwhat()));
    }
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ClipboardManagerDefault::restoreClipboard() {
    try {
        if (!this->hasBackup())
            return;

        ScopedClipboardAccess const sca(nullptr);
        if (!sca.isOpen())
            return;

        EmptyClipboard();
        for (SpClipBoardFormatData const &cbData: backup_) {
            if ((!cbData) || (!cbData->data.size()))
                continue;
            quint32 const size = static_cast<quint32>(cbData->data.size());
            HANDLE const handle = GlobalAlloc(GMEM_MOVEABLE, size);
            if (!handle)
                throw Exception(QString("Could allocate global memory for format %1.").arg(cbData->format));
            try {
                ScopedGlobalMemoryLock memLock(handle);
                quint32 *const data = static_cast<quint32 *>(memLock.pointer());
                if (!data)
                    throw Exception(QString("Could not lock allocated global memory for format %1.").arg(cbData->format));
                memcpy(data, cbData->data.data(), size);
                if (!SetClipboardData(cbData->format, handle))
                    throw Exception(QString("Could not restore clipboard data for format %1.").arg(cbData->format));
            }
            catch (Exception const &) {
                ScopedGlobalMemoryLock const memLock(handle);
                GlobalFree(handle);
            }
        }

        backup_.clear();

        if (bitmapBackup_) {
            if (!SetClipboardData(CF_BITMAP, bitmapBackup_))
                throw Exception("Could not set set bitmap data.");
            bitmapBackup_ = nullptr; // bitmap is now owned by the clipboard.
        }

    } catch (Exception const &e) {
        globals::debugLog().addError(QString("%1: %2").arg(__FUNCTION__, e.qwhat()));
    }
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ClipboardManagerDefault::clearClipboard() {
    try {
        ScopedClipboardAccess const sca(nullptr);
        if (!sca.isOpen())
            throw Exception("Could not clear the clipboard.");

        if (!EmptyClipboard())
            throw Exception("The clipboard could not be cleared.");
    } catch (Exception const & e) {
        globals::debugLog().addError(QString("%1: %2").arg(__FUNCTION__, e.qwhat()));
    }
}

//****************************************************************************************************************************************************
/// \return true if and only if the clipboard manager contains a clipboard backup
//****************************************************************************************************************************************************
bool ClipboardManagerDefault::hasBackup() const {
    return (!backup_.empty()) || bitmapBackup_;
}


//****************************************************************************************************************************************************
/// \ return The text value of the clipboard. If the clipboard does not contain text, an empty string is returned.
//****************************************************************************************************************************************************
QString ClipboardManagerDefault::text() {
    ScopedClipboardAccess const sca(nullptr);
    if ((!sca.isOpen()) || (!IsClipboardFormatAvailable(CF_UNICODETEXT))) // Note system does automatic conversion from CF_OEMTEXT and CF_TEXT to CF_UNICODETEXT
        return QString();
    HANDLE const handle = GetClipboardData(CF_UNICODETEXT);
    if (!handle)
        return QString();
    ScopedGlobalMemoryLock const memLock(handle);
    quint32 const *const data = static_cast<quint32 const *>(memLock.pointer());
    if (!data)
        return QString();
    SIZE_T const size = GlobalSize(handle);
    if (!size)
        return QString();
    QByteArray array(static_cast<qsizetype>(size), 0);
    memcpy(array.data(), data, size);
    return QString::fromUtf16(reinterpret_cast<char16_t const *>(array.data()), (qsizetype(size) - 1) / 2); // (size - 1) because we discard the final `0x0000`
}


//****************************************************************************************************************************************************
/// \brief Allocate space in global memory and put the given text in UTF-16 format in it.
///
/// \param[in] text The plain text.
/// \return A handle to the global memory containing the text in UTF-8 format.
//****************************************************************************************************************************************************
HANDLE putUtf16InGlobalMemory(QString const &text) {
    if (text.isEmpty())
        return nullptr;
    quint16 const *const data = text.utf16();
    quint32 const size = 2 * (static_cast<quint32>(text.length()) + 1);
    HANDLE const handle = GlobalAlloc(GMEM_MOVEABLE, 2 * (static_cast<quint32>(text.length()) + 1));  // NOLINT(bugprone-implicit-widening-of-multiplication-result)
    if (!handle)
        return nullptr;
    ScopedGlobalMemoryLock const memLock(handle);
    quint8 *const pointer = static_cast<quint8 *>(memLock.pointer());
    if (!pointer) {
        GlobalFree(handle);
        return nullptr;
    }
    memcpy(pointer, data, size);
    pointer[size - 1] = 0;
    pointer[size - 2] = 0; // for safety
    return handle;
}


//****************************************************************************************************************************************************
/// \param[in] text The text to put in the clipboard.
/// \return true if and only if the operation was successful.
//****************************************************************************************************************************************************
bool ClipboardManagerDefault::setText(QString const &text) {
    HANDLE const handle = putUtf16InGlobalMemory(text);
    if (!handle)
        return false;
    try {
        ScopedClipboardAccess const sca(nullptr);
        if (!sca.isOpen())
            throw Exception();

        EmptyClipboard();
        bool const result = SetClipboardData(CF_UNICODETEXT, handle);
        if (!result)
            throw Exception();
        return true;
    }
    catch (Exception const &) {
        ScopedGlobalMemoryLock const memLock(handle);
        GlobalFree(handle);
        return false;
    }
}


//****************************************************************************************************************************************************
/// \brief extract a number from an HTML clipboard data description field (which have the form 'fieldName:00000123'
/// \param[in] clipboardData The HTML format data read from the clipboard
/// \param[in] fieldName The name of the field.
/// \return The value read from the field.
/// \return -1 if the field could not be found
//****************************************************************************************************************************************************
qint32 numberFieldFromHtmlClipboardData(QString const &clipboardData, QString const &fieldName) {
    QRegularExpression const regExp(kRegExpHtmlFormatField.arg(fieldName), QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
    QRegularExpressionMatch const match = regExp.match(clipboardData);
    if (!match.hasMatch())
        return -1;
    bool ok = false;
    qint32 result = match.captured(1).toInt(&ok);
    return ok ? result : -1;
}


//****************************************************************************************************************************************************
/// \brief Extract the HTML content for HTML clipboard data. The clipboard format for HTML is detailed here:
/// https://docs.microsoft.com/en-us/windows/win32/dataxchg/html-clipboard-format
/// 
/// \param[in] clipboardData The raw HTML data obtained from the clipboard.
/// \return The HTML content. If extraction fails, the function returns an empty string.
//****************************************************************************************************************************************************
QString extractHtmlFromClipboardData(QString const &clipboardData) {
    qint32 const startHtml = numberFieldFromHtmlClipboardData(clipboardData, "StartHTML");
    qint32 const endHtml = numberFieldFromHtmlClipboardData(clipboardData, "EndHTML");
    qint32 const startFrag = numberFieldFromHtmlClipboardData(clipboardData, "StartFragment");
    qint32 const endFrag = numberFieldFromHtmlClipboardData(clipboardData, "EndFragment");
    if ((startHtml >= 0) && (endHtml >= 0))
        return clipboardData.mid(startHtml, endHtml - startHtml + 1);
    return (startFrag < 0) || (endFrag < 0) ? QString() : clipboardData.mid(startFrag, endFrag - startFrag + 1);
}


//****************************************************************************************************************************************************
/// \ return The HTML value of the clipboard. If the clipboard does not contain HTML, an empty string is returned.
//****************************************************************************************************************************************************
QString ClipboardManagerDefault::html() {
    ScopedClipboardAccess const sca(nullptr);
    quint32 const htmlFormat = htmlClipboardFormat();
    if ((!sca.isOpen()) || (!IsClipboardFormatAvailable(htmlFormat))) // Note system does automatic conversion from CF_OEMTEXT and CF_TEXT to CF_UNICODETEXT
        return QString();
    HANDLE const handle = GetClipboardData(htmlFormat);
    if (!handle)
        return QString();
    ScopedGlobalMemoryLock const memLock(handle);
    quint32 const *const data = static_cast<quint32 const *>(memLock.pointer());
    if (!data)
        return QString();
    SIZE_T const size = GlobalSize(handle);
    if (!size)
        return QString();
    QByteArray array(static_cast<qint32>(size), 0);
    memcpy(array.data(), data, size);
    return extractHtmlFromClipboardData(QString::fromUtf8(array.data(), static_cast<qint32>(size) - 1)); // (size - 1) because we discard the final `0x0000`

}


//****************************************************************************************************************************************************
/// \param[in] html The HTML data.
/// \return true if and only if the HTML data was successfully copied to the clipboard
//****************************************************************************************************************************************************
bool ClipboardManagerDefault::setHtml(QString const &html) {
    // Later, this function should use the lower level clipboard API for HTML, which is not really easy at the moment.
    QApplication::clipboard()->setMimeData(mimeDataFromHtml(html)); // Ownership of data is transferred to the clipboard
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] bitmap The bitmap.
//****************************************************************************************************************************************************
void ClipboardManagerDefault::setBitmapBackup(HANDLE bitmap) {
    this->freeBitmapBackup();
    if (!CopyImage(bitmap, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE))
        throw Exception("Could not copy bitmap from clipboard");
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ClipboardManagerDefault::freeBitmapBackup() {
    if (!bitmapBackup_)
        return;

    if (!DeleteObject(bitmapBackup_))
        throw Exception("Could not delete clipboard bitmap backup");
    bitmapBackup_ = nullptr;
}


