/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of clipboard manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_CLIPBOARD_MANAGER_DEFAULT_H
#define BEEFTEXT_CLIPBOARD_MANAGER_DEFAULT_H


#include "ClipboardManager.h"


//****************************************************************************************************************************************************
/// \brief Clipboard manager class
//****************************************************************************************************************************************************
class ClipboardManagerDefault : public ClipboardManager {
    Q_OBJECT
public: // member functions
    ClipboardManagerDefault(); ///< Default constructor.
    ClipboardManagerDefault(ClipboardManagerDefault const &) = delete; ///< Disabled copy constructor.
    ClipboardManagerDefault(ClipboardManagerDefault &&) = delete; ///< Disabled move constructor.
    ~ClipboardManagerDefault() override = default; ///< Default destructor.
    ClipboardManagerDefault &operator=(ClipboardManagerDefault const &) = delete; ///< Disabled assignment operator.
    ClipboardManagerDefault &operator=(ClipboardManagerDefault &&) = delete; ///< Disabled move assignment operator.
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
    void setBitmapBackup(HANDLE bitmap); ///< Backup a bitmap.
    void freeBitmapBackup(); ///< Free a bitmap backup.

private: // data structures
    struct ClipBoardFormatData {
        quint32 format { 0 }; ///< The format.
        QByteArray data; ///< The data.
    }; ///< A structure associating a clipboard format with its data.

    typedef std::shared_ptr<ClipBoardFormatData> SpClipBoardFormatData; ///< Type definition for shared pointer to ClipboardFormatData.
    typedef std::vector<SpClipBoardFormatData> VecSpClipBoardFormatData; ///< Type definition for vector of SpClipBoardFormatData.

private: // data members
    VecSpClipBoardFormatData backup_; ///< The clipboard backup.
    HBITMAP bitmapBackup_ { nullptr }; ///< The backup of the bitmap object in the clipboard.
};


#endif // #ifndef BEEFTEXT_CLIPBOARD_MANAGER_DEFAULT_H
