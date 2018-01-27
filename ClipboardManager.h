/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of clipboard manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__CLIPBOARD__MANAGER__H
#define BEEFTEXT__CLIPBOARD__MANAGER__H


//**********************************************************************************************************************
/// \brief Clipboard manager class
//**********************************************************************************************************************
class ClipboardManager
{
public: // member functions
   static ClipboardManager& instance(); ///< Return the only allowed instance of the class
	~ClipboardManager() = default; ///< Default destructor
   void backupClipboard(); ///< backup the clipboard
   void restoreClipboard(); ///< Restore the clipboard and delete the current backup
   bool hasBackup() const; ///< Test if the clipboard is empty

private: // member functions
   ClipboardManager() = default; ///< Default constructor
   ClipboardManager(ClipboardManager const&) = delete; ///< Disabled copy constructor
	ClipboardManager(ClipboardManager&&) = delete; ///< Disabled move constructor
	ClipboardManager& operator=(ClipboardManager const&) = delete; ///< Disabled assignment operator
	ClipboardManager& operator=(ClipboardManager&&) = delete; ///< Disabled move assignment operator
   QMimeData* mimeDataFromBackup() const; ///< Create a MIME data instance from the current backup

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


#endif // #ifndef BEEFTEXT__CLIPBOARD__MANAGER__H
