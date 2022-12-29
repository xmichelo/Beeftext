/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of abstract clipboard manager interface.
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_CLIPBOARD_MANAGER_INTERFACE_H
#define BEEFTEXT_CLIPBOARD_MANAGER_INTERFACE_H


//****************************************************************************************************************************************************
/// \brief Abstract clipboard manager class used as an interface.
//****************************************************************************************************************************************************
class ClipboardManager: public QObject {
    Q_OBJECT
public: // data types
    enum class EType {
        Default, ///< The default clipboard manager.
        Legacy ///< The legacy clipboard manager.
    }; ///< Enumeration for the type of clipboard manager

public: // static members
    static ClipboardManager &instance(); ///< Return the instance of the clipboard manager
    static void setClipboardManagerType(EType type); ///< Set the clipboard manager type (default or legacy).

public: // member functions
    ClipboardManager(); ///< Default constructor.
    ClipboardManager(ClipboardManager const &) = delete; ///< Disabled copy constructor.
    ClipboardManager(ClipboardManager &&) = delete; ///< Disabled move constructor.
    ~ClipboardManager() override = default; ///< Default destructor.
    ClipboardManager &operator=(ClipboardManager const &) = delete; ///< Disabled assignment operator.
    ClipboardManager &operator=(ClipboardManager &&) = delete; ///< Disabled move assignment operator.
    virtual EType type() const = 0; ///< Return the type of clipboard manager of the instance.
    virtual bool hasBackup() const = 0; ///< Test if the clipboard is empty.
    virtual QString text() = 0; ///< Return the current text value of the clipboard.
    virtual bool setText(QString const &text) = 0; ///< Put text into the clipboard.
    virtual QString html() = 0; ///< Return the current HTML value of the clipboard.
    virtual bool setHtml(QString const &html) = 0; ///< Set the current HTML value of the clipboard.

public slots:
    virtual void backupClipboard() = 0; ///< backup the clipboard.
    virtual void restoreClipboard() = 0; ///< Restore the clipboard and delete the current backup
    virtual void clearClipboard() = 0; ///< Clear the clipboard.

};


#endif // BEEFTEXT_CLIPBOARD_MANAGER_INTERFACE_H
