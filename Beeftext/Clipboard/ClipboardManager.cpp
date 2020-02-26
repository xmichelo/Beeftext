/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of clipboard manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ClipboardManager.h"
#include <XMiLib/Scoped/ScopedClipboardAccess.h>
#include <XMiLib/Scoped/ScopedGlobalMemoryLock.h>
#include <XMiLib/Exception.h>


using namespace xmilib;


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

   EmptyClipboard();
   ScopedClipboardAccess const sca(nullptr);
   if (!sca.isOpen())
      return;

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
