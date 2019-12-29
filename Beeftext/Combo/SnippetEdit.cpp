/// \file
/// \author 
///
/// \brief Implementation of custom text edit widget
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "SnippetEdit.h"
#include <QtCore/qlogging.h>


//**********************************************************************************************************************
/// \brief Remve the images from the text of a document.
///
/// \param[in] html The HTML document
/// \return The text contained in the document, with the images removed.
//**********************************************************************************************************************
QString removeImagesFromHtml(QString const& html)
{
   QTextDocument doc;
   doc.setHtml(html);
   QTextBlock block = doc.begin();
   QVector<qint32> positions;
   while (block.isValid())
   {
      for (QTextBlock::iterator it = block.begin(); it != block.end(); ++it)
      {
         QTextFragment const fragment = it.fragment();
         if (!fragment.isValid())
            continue;
         QTextCharFormat const format = fragment.charFormat();
         if (!format.isImageFormat())
            continue;
         QTextImageFormat const imageFormat = format.toImageFormat();
         if (!format.isValid())
            continue;
         positions.push_back(fragment.position());
      }
      block = block.next();
   }

   QTextCursor cursor(&doc);
   for (QVector<qint32>::reverse_iterator it = positions.rbegin(); it != positions.rend(); ++it)
   {
      cursor.setPosition(*it);
      cursor.deleteChar();
   }
   return doc.toHtml();
}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the edit.
//**********************************************************************************************************************
SnippetEdit::SnippetEdit(QWidget* parent)
   : QTextEdit(parent)
{
}


//**********************************************************************************************************************
/// \param[in] source The MIME data being pasted
//**********************************************************************************************************************
void SnippetEdit::insertFromMimeData(const QMimeData* source)
{
   if (!source)
      return;

   if (!this->acceptRichText())
   {
      if (source->hasText())
         this->insertPlainText(source->text());
      else
         if (source->hasHtml())
            this->insertHtml(source->html());
      return;
   }

   if (source->hasHtml())
      this->insertHtml(removeImagesFromHtml(source->html()));
   else 
      QTextEdit::insertFromMimeData(source);
}
