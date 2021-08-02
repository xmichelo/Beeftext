/// \file
/// \author 
///
/// \brief Declaration of snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFEXT_SNIPPET_FRAGMENT_H
#define BEEFEXT_SNIPPET_FRAGMENT_H


#include "BeeftextUtils.h"


//**********************************************************************************************************************
/// \brief Snippet fragment class.
//**********************************************************************************************************************
class SnippetFragment
{
public: // data types
   enum class EType
   {
      Text = 0, ///< Text fragment.
      Delay  = 1, ///< Delay fragment.
      Count  = 2, ///< The number of fragment types
   }; ///< Enumeration for the type of fragment

public: // member functions
   SnippetFragment() {}; ///< Default constructor.
   SnippetFragment(SnippetFragment const&) = delete; ///< Disabled copy-constructor.
   SnippetFragment(SnippetFragment&&) = delete; ///< Disabled assignment copy-constructor.
   virtual ~SnippetFragment() = default; ///< Destructor.
   SnippetFragment& operator=(SnippetFragment const&) = delete; ///< Disabled assignment operator.
   SnippetFragment& operator=(SnippetFragment&&) = delete; ///< Disabled move assignment operator.
   virtual EType type() const = 00; ///< The type of fragment.
   virtual QString toString() const = 00; ///< Return a string describing the snippet fragment.
};


typedef std::shared_ptr<SnippetFragment> SpSnippetFragment; ///< Type definition for shared pointer to snippet fragment.
typedef QList<SpSnippetFragment> ListSpSnippetFragment; ///< Type definition for vector of SpSnippetFragment.


ListSpSnippetFragment splitStringIntoSnippetFragments(QString const& str); ///< Split a string snippet fragments


#endif // #ifndef BEEFEXT_SNIPPET_FRAGMENT_H
