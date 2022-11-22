/// \file
/// \author 
///
/// \brief Implementation of text snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "TextSnippetFragment.h"
#include "BeeftextUtils.h"


//****************************************************************************************************************************************************
/// \param[in] text The text.
//****************************************************************************************************************************************************
TextSnippetFragment::TextSnippetFragment(QString const &text)
    : SnippetFragment()
    , text_(text) {
}


//****************************************************************************************************************************************************
/// \return the type of the snippet fragment.
//****************************************************************************************************************************************************
SnippetFragment::EType TextSnippetFragment::type() const {
    return EType::Text;
}


//****************************************************************************************************************************************************
/// \return a string describing the snippet fragment.
//****************************************************************************************************************************************************
QString TextSnippetFragment::toString() const {
    return "Text fragment: " + text_;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void TextSnippetFragment::render() const {
    insertText(text_);
}
