/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of clipboard manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ClipboardManager.h"


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
   backup_.reset();
   QMimeData const* mimeData = QApplication::clipboard()->mimeData();
   if ((!mimeData) || ((!mimeData->hasUrls()) && (!mimeData->hasImage()) && (!mimeData->hasColor()) 
      && (!mimeData->hasHtml()) && (!mimeData->hasText())))
      return;

   backup_ = std::make_unique<Backup>();
   if (mimeData->hasImage())
   {
      backup_->imageData = mimeData->imageData();
      return;
   }
   if (mimeData->hasColor())
   {
      backup_->colorData = mimeData->colorData();
      return;
   }
   if (mimeData->hasUrls())
   {
      backup_->urls = mimeData->urls();
      return;
   }
   if (mimeData->hasHtml())
      backup_->html = mimeData->html(); // note that we do not return here, has we allow to have both html and text at the same time
   if (mimeData->hasText())
      backup_->text = mimeData->text();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ClipboardManager::restoreClipboard()
{
   if (!this->hasBackup())
      return;

   QMimeData* mimeData = this->mimeDataFromBackup(); // ownership will be transfered to Qt when calling QClipboard::setMimeData()
   QClipboard* clipboard = QApplication::clipboard();
   if (!mimeData)
      clipboard->clear();
   else
      clipboard->setMimeData(mimeData);
   backup_.reset();
}


//**********************************************************************************************************************
/// \return true if and only if the clipboard manager contains a clipboard backup
//**********************************************************************************************************************
bool ClipboardManager::hasBackup() const
{
   return backup_.get();
}


//**********************************************************************************************************************
/// \return The plain text of the backup
//**********************************************************************************************************************
QString ClipboardManager::text() const
{
   if (!backup_)
      return QString();
   if (!backup_->text.isEmpty())
      return backup_->text;
   QString const html = backup_->html;
   if (html.isEmpty())
      return QString();
   QTextDocument doc;
   doc.setHtml(html);
   return doc.toPlainText();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
QMimeData* ClipboardManager::mimeDataFromBackup() const
{
   if (!hasBackup())
      return nullptr;
   QMimeData* mimeData = new QMimeData;
   if (!backup_->imageData.isNull())
   {
      mimeData->setImageData(backup_->imageData);
      return mimeData;
   }
   if (!backup_->colorData.isNull())
   {
      mimeData->setColorData(backup_->colorData);
      return mimeData;
   }
   if (!backup_->urls.isEmpty())
   {
      mimeData->setUrls(backup_->urls);
      return mimeData;
   }
   if (!backup_->html.isEmpty())
      mimeData->setHtml(backup_->html); // we do not return here to allow having both text and HTML in the clipboard
   if (!backup_->text.isEmpty())
      mimeData->setText(backup_->text);
   return mimeData;
}


