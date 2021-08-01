/// \file
/// \author 
///
/// \brief Implementation of snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "SnippetFragment.h"
#include "TextSnippetFragment.h"
#include "DelaySnippetFragment.h"


namespace {
   QString const kStrDelayRegEx(R"(#{delay:(\d+)})");
}

//**********************************************************************************************************************
/// \param[in] str The string to split.
/// \return the list of fragments
//**********************************************************************************************************************
ListSpSnippetFragment splitStringIntoSnippetFragments(QString const& str)
{
   ListSpSnippetFragment result;
   QString s(str);
   QRegularExpression const rx(QString(R"(^(.*)%1(.*)$)").arg(kStrDelayRegEx), 
      QRegularExpression::InvertedGreedinessOption);
   QRegularExpressionMatch match;
   while ((match = rx.match(s)).hasMatch())
   {
      QString const before = match.captured(1);
      if (!before.isEmpty())
         result.append(std::make_shared<TextSnippetFragment>(before));
      bool ok = false;
      qint32 const delay = match.captured(2).toInt(&ok);
      if (ok && (delay > 0))
         result.append(std::make_shared<DelaySnippetFragment>(delay));

      s = match.captured(3);
   }
   if (!s.isEmpty())
      result.append(std::make_shared<TextSnippetFragment>(s));
   return result;
}
