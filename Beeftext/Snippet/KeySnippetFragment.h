/// \file
/// \author 
///
/// \brief Declaration of key snippet fragment.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_KEY_SNIPPET_FRAGMENT_H
#define BEEFTEXT_KEY_SNIPPET_FRAGMENT_H


#include "SnippetFragment.h"


//****************************************************************************************************************************************************
/// \brief Key snippet fragment class
//****************************************************************************************************************************************************
class KeySnippetFragment : public SnippetFragment {
public: // member functions
    KeySnippetFragment(QString const &key, qint32 repeatCount); ///< Default constructor.
    KeySnippetFragment(KeySnippetFragment const &) = delete; ///< Disabled copy-constructor.
    KeySnippetFragment(KeySnippetFragment &&) = delete; ///< Disabled assignment copy-constructor.
    ~KeySnippetFragment() override = default; ///< Destructor.
    KeySnippetFragment &operator=(KeySnippetFragment const &) = delete; ///< Disabled assignment operator.
    KeySnippetFragment &operator=(KeySnippetFragment &&) = delete; ///< Disabled move assignment operator.
    EType type() const override; ///< The type of fragment.
    QString toString() const override; ///< Return a string describing the snippet fragment.
    void render() const override; ///< Render the snippet fragment.

private: // data members
    quint16 key_ { 0 }; ///< The key.
    qint32 repeatCount_ { 1 }; ///< The repeat count for the key.
};


#endif // #ifndef BEEFTEXT_KEY_SNIPPET_FRAGMENT_H
