/// \file
/// \author 
///
/// \brief Implementation of shortcut snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ShortcutSnippetFragment.h"
#include "BeeftextUtils.h"


//****************************************************************************************************************************************************
/// \param[in] shortcut The shortcut.
//****************************************************************************************************************************************************
ShortcutSnippetFragment::ShortcutSnippetFragment(SpShortcut const &shortcut)
    : shortcut_(shortcut) {
}


//****************************************************************************************************************************************************
/// \return The type of snippet fragment.
//****************************************************************************************************************************************************
SnippetFragment::EType ShortcutSnippetFragment::type() const {
    return EType::Shortcut;
}


//****************************************************************************************************************************************************
/// \return A string describing the shortcut snippet fragment
//****************************************************************************************************************************************************
QString ShortcutSnippetFragment::toString() const {
    return QString("Shortcut fragment : %1").arg(shortcut_ ? shortcut_->toString() : "null");
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ShortcutSnippetFragment::render() const {
    if (shortcut_)
        renderShortcut(shortcut_);
}
