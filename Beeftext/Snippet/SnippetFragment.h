/// \file
/// \author 
///
/// \brief Declaration of snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFEXT_SNIPPET_FRAGMENT_H
#define BEEFEXT_SNIPPET_FRAGMENT_H


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
      Count  = 3, ///< The number of fragment types
   }; ///< Enumeration for the type of fragment
public: // member functions
   SnippetFragment() = default; ///< Default constructor.
   SnippetFragment(SnippetFragment const&) = delete; ///< Disabled copy-constructor.
   SnippetFragment(SnippetFragment&&) = delete; ///< Disabled assignment copy-constructor.
   virtual ~SnippetFragment() = default; ///< Destructor.
   SnippetFragment& operator=(SnippetFragment const&) = delete; ///< Disabled assignment operator.
   SnippetFragment& operator=(SnippetFragment&&) = delete; ///< Disabled move assignment operator.
   virtual EType type() const = 00; ///< The type of fragment.
   virtual void render() const = 00; ///< Render the snippet fragment
};


#endif // #ifndef BEEFEXT_SNIPPET_FRAGMENT_H
