/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of legacy clipboard manager class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_CLIPBOARD_MANAGER_LEGACY_H
#define BEEFTEXT_CLIPBOARD_MANAGER_LEGACY_H


#include "ClipboardManager.h"


//****************************************************************************************************************************************************
/// \brief Legacy clipboard manager class. This exists because it seems that for a couple of users the default
/// clipboard manager crashes.
//****************************************************************************************************************************************************
class ClipboardManagerLegacy : public ClipboardManager {
    Q_OBJECT
public: // member functions
    ClipboardManagerLegacy(); ///< Default constructor.
    ClipboardManagerLegacy(ClipboardManagerLegacy const &) = delete; ///< Disabled copy-constructor.
    ClipboardManagerLegacy(ClipboardManagerLegacy &&) = delete; ///< Disabled assignment copy-constructor.
    ~ClipboardManagerLegacy() override = default; ///< Destructor.
    ClipboardManagerLegacy &operator=(ClipboardManagerLegacy const &) = delete; ///< Disabled assignment operator.
    ClipboardManagerLegacy &operator=(ClipboardManagerLegacy &&) = delete; ///< Disabled move assignment operator.
    EType type() const override; ///< Return the type of clipboard manager of the instance.
    bool hasBackup() const override; ///< Test if the clipboard is empty.
    QString text() override; ///< Return the current text value of the clipboard.
    bool setText(QString const &text) override; ///< Put text into the clipboard.
    QString html() override; ///< Return the current HTML value of the clipboard.
    bool setHtml(QString const &html) override; ///< Set the current HTML value of the clipboard.

public slots:
    void backupClipboard() override; ///< backup the clipboard.
    void restoreClipboard() override; ///< Restore the clipboard and delete the current backup
    void clearClipboard() override; ///< Clear the clipboard.

private: // member functions
    QMimeData *mimeDataFromBackup() const; ///< Create a MIME data instance from the current backup

private: // data structures
    struct Backup {
        QVariant imageData; ///< The image data
        QVariant colorData; ///< The color data
        QList<QUrl> urls; ///< The urls
        QString html; ///< The HTML text
        QString text; ///< The plain text
    }; ///< A structure containing clipboard backup data

private: // data members
    std::unique_ptr<Backup> backup_; ///< The backup
};


#endif // BEEFTEXT_CLIPBOARD_MANAGER_LEGACY_H
