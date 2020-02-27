/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of clipboard manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_CLIPBOARD_MANAGER_H
#define BEEFTEXT_CLIPBOARD_MANAGER_H


//**********************************************************************************************************************
/// \brief Clipboard manager class
//**********************************************************************************************************************
class ClipboardManager
{
public: // member functions
   static ClipboardManager& instance(); ///< Return the only allowed instance of the class.
   ClipboardManager(ClipboardManager const&) = delete; ///< Disabled copy constructor.
	ClipboardManager(ClipboardManager&&) = delete; ///< Disabled move constructor.
	~ClipboardManager() = default; ///< Default destructor.
	ClipboardManager& operator=(ClipboardManager const&) = delete; ///< Disabled assignment operator.
	ClipboardManager& operator=(ClipboardManager&&) = delete; ///< Disabled move assignment operator.
   void backupClipboard(); ///< backup the clipboard.
   void restoreClipboard(); ///< Restore the clipboard and delete the current backup
   bool hasBackup() const; ///< Test if the clipboard is empty.
   static QString text(); ///< Return the current text value of the clipboard.

private: // member functions
   ClipboardManager() = default; ///< Default constructor.

private: // data structures
   struct ClipBoardFormatData
   {
      qint32 format { 0 }; ///< The format.
      QByteArray data; ///< The data.
   }; ///< A structure associating a clipboard format with its data.

   typedef std::shared_ptr<ClipBoardFormatData> SpClipBoardFormatData; ///< Type definition for shared pointer to ClipboardFormatData.
   typedef std::vector<SpClipBoardFormatData> VecSpClipBoardFormatData; ///< Type definition for vector of SpClipBoardFormatData.

private: // data members
   VecSpClipBoardFormatData backup_; ///< The clipboard backup.
};


#endif // #ifndef BEEFTEXT_CLIPBOARD_MANAGER_H
