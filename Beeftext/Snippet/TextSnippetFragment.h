/// \file
/// \author 
///
/// \brief Declaration of text snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_TEXT_SNIPPET_FRAGMENT_H
#define BEEFTEXT_TEXT_SNIPPET_FRAGMENT_H


#include "SnippetFragment.h"


//****************************************************************************************************************************************************
/// \brief Text snippet fragment class.
//****************************************************************************************************************************************************
class TextSnippetFragment : public SnippetFragment {
public: // member functions
    explicit TextSnippetFragment(QString const &text); ///< Default constructor.
    TextSnippetFragment(TextSnippetFragment const &) = delete; ///< Disabled copy-constructor.
    TextSnippetFragment(TextSnippetFragment &&) = delete; ///< Disabled assignment copy-constructor.
    ~TextSnippetFragment() override = default; ///< Destructor.
    TextSnippetFragment &operator=(TextSnippetFragment const &) = delete; ///< Disabled assignment operator.
    TextSnippetFragment &operator=(TextSnippetFragment &&) = delete; ///< Disabled move assignment operator.
    EType type() const override; ///< Return the type of snippet fragment.
    void render() const override; ///< Render the snippet fragment.
    QString toString() const override; ///< Return a string describing the snippet fragment.

private:
    QString text_; ///< The text.
};


#endif // #ifndef BEEFTEXT_TEXT_SNIPPET_FRAGMENT_H
