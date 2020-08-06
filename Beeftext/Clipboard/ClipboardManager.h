/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of abstract clipboard manager interface.


#ifndef BEEFTEXT_CLIPBOARD_MANAGER_INTERFACE_H
#define BEEFTEXT_CLIPBOARD_MANAGER_INTERFACE_H


//**********************************************************************************************************************
/// \brief Clipboard manager interface
//**********************************************************************************************************************
class ClipboardManager
{
public: // member functions
   ClipboardManager() = default; ///< Default constructor.
   ClipboardManager(ClipboardManager const&) = delete; ///< Disabled copy constructor.
   ClipboardManager(ClipboardManager&&) = delete; ///< Disabled move constructor.
   virtual ~ClipboardManager() {} ; ///< Default destructor.
   ClipboardManager& operator=(ClipboardManager const&) = delete; ///< Disabled assignment operator.
   ClipboardManager& operator=(ClipboardManager&&) = delete; ///< Disabled move assignment operator.
   virtual void backupClipboard() = 0; ///< backup the clipboard.
   virtual void restoreClipboard() = 0; ///< Restore the clipboard and delete the current backup
   virtual bool hasBackup() const = 0; ///< Test if the clipboard is empty.
   virtual QString text() = 0; ///< Return the current text value of the clipboard.
   virtual bool setText(QString const& text) = 0; ///< Put text into the clipboard.
   virtual QString html() = 0; ///< Return the current HTML value of the clipboard.
   virtual bool setHtml(QString const& html) = 0; ///< Set the current HTML value of the clipboard.
};


#endif // BEEFTEXT_CLIPBOARD_MANAGER_INTERFACE_H
