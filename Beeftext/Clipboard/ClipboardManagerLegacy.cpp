/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of legacy clipboard manager class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ClipboardManagerLegacy.h"
#include "BeeftextUtils.h"


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
ClipboardManagerLegacy::ClipboardManagerLegacy()
    : ClipboardManager() {
}


//****************************************************************************************************************************************************
/// \return The type of clipboard manager.
//****************************************************************************************************************************************************
ClipboardManager::EType ClipboardManagerLegacy::type() const {
    return EType::Legacy;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ClipboardManagerLegacy::backupClipboard() {
    backup_.reset();
    QMimeData const *mimeData = QApplication::clipboard()->mimeData();
    if ((!mimeData) || ((!mimeData->hasUrls()) && (!mimeData->hasImage()) && (!mimeData->hasColor())
                        && (!mimeData->hasHtml()) && (!mimeData->hasText())))
        return;

    backup_ = std::make_unique<Backup>();
    if (mimeData->hasImage()) {
        backup_->imageData = mimeData->imageData();
        return;
    }
    if (mimeData->hasColor()) {
        backup_->colorData = mimeData->colorData();
        return;
    }
    if (mimeData->hasUrls()) {
        backup_->urls = mimeData->urls();
        return;
    }
    if (mimeData->hasHtml())
        backup_->html = mimeData->html(); // note that we do not return here, has we allow to have both html and text at the same time
    if (mimeData->hasText())
        backup_->text = mimeData->text();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ClipboardManagerLegacy::restoreClipboard() {
    if (!this->hasBackup())
        return;

    QMimeData *mimeData = this->mimeDataFromBackup(); // ownership will be transfered to Qt when calling QClipboard::setMimeData()
    QClipboard *clipboard = QApplication::clipboard();
    if (!mimeData)
        clipboard->clear();
    else
        clipboard->setMimeData(mimeData);
    backup_.reset();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ClipboardManagerLegacy::clearClipboard() {
    QApplication::clipboard()->clear();
}


//****************************************************************************************************************************************************
/// \return true if and only if the clipboard manager contains a clipboard backup
//****************************************************************************************************************************************************
bool ClipboardManagerLegacy::hasBackup() const {
    return backup_.get();
}


//****************************************************************************************************************************************************
/// \return The content of the clipboard as text.
//****************************************************************************************************************************************************
QString ClipboardManagerLegacy::text() {
    QClipboard const *clipboard = QGuiApplication::clipboard();
    if (!clipboard)
        return QString();
    QMimeData const *mimeData = clipboard->mimeData();
    return mimeData ? clipboard->text() : QString();
}


//****************************************************************************************************************************************************
/// \param[in] text The text to put in the clipboard.
/// \return true if and only if the operation was successful.
//****************************************************************************************************************************************************
bool ClipboardManagerLegacy::setText(QString const &text) {
    QClipboard *clipboard = QApplication::clipboard();
    if (!clipboard)
        return false;
    clipboard->setMimeData(mimeDataFromText(text));
    return true;
}


//****************************************************************************************************************************************************
/// \return The content of the clipboard as HTML.
//****************************************************************************************************************************************************
QString ClipboardManagerLegacy::html() {
    QClipboard const *clipboard = QGuiApplication::clipboard();
    if (!clipboard)
        return QString();
    QMimeData const *mimeData = clipboard->mimeData();
    return mimeData->hasHtml() ? mimeData->html() : clipboard->text();
}


//****************************************************************************************************************************************************
/// \param[in] html The html content.
/// \return true if and only if the operation was successful.
//****************************************************************************************************************************************************
bool ClipboardManagerLegacy::setHtml(QString const &html) {
    QClipboard *clipboard = QApplication::clipboard();
    if (!clipboard)
        return false;
    clipboard->setMimeData(mimeDataFromHtml(html));
    return true;
}


//****************************************************************************************************************************************************
/// \return The MIME data
//****************************************************************************************************************************************************
QMimeData *ClipboardManagerLegacy::mimeDataFromBackup() const {
    if (!hasBackup())
        return nullptr;
    QMimeData *mimeData = new QMimeData;
    if (!backup_->imageData.isNull()) {
        mimeData->setImageData(backup_->imageData);
        return mimeData;
    }
    if (!backup_->colorData.isNull()) {
        mimeData->setColorData(backup_->colorData);
        return mimeData;
    }
    if (!backup_->urls.isEmpty()) {
        mimeData->setUrls(backup_->urls);
        return mimeData;
    }
    if (!backup_->html.isEmpty())
        mimeData->setHtml(backup_->html); // we do not return here to allow having both text and HTML in the clipboard
    if (!backup_->text.isEmpty())
        mimeData->setText(backup_->text);
    return mimeData;
}
