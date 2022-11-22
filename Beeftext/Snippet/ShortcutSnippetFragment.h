/// \file
/// \author 
///
/// \brief Declaration of shortcut snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_SHORTCUT_SNIPPET_FRAGMENT_H
#define BEEFTEXT_SHORTCUT_SNIPPET_FRAGMENT_H


#include "SnippetFragment.h"
#include "Shortcut.h"


//****************************************************************************************************************************************************
/// \brief Shortcut snippet fragment class
//****************************************************************************************************************************************************
class ShortcutSnippetFragment : public SnippetFragment {
public: // member functions
    explicit ShortcutSnippetFragment(SpShortcut const &shortcut); ///< Default constructor.
    ShortcutSnippetFragment(ShortcutSnippetFragment const &) = delete; ///< Disabled copy-constructor.
    ShortcutSnippetFragment(ShortcutSnippetFragment &&) = delete; ///< Disabled assignment copy-constructor.
    ~ShortcutSnippetFragment() override = default; ///< Destructor.
    ShortcutSnippetFragment &operator=(ShortcutSnippetFragment const &) = delete; ///< Disabled assignment operator.
    ShortcutSnippetFragment &operator=(ShortcutSnippetFragment &&) = delete; ///< Disabled move assignment operator.
    EType type() const override; ///< The type of fragment.
    QString toString() const override; ///< Return a string describing the snippet fragment.
    void render() const override; ///< Render the snippet fragment.

private: // data members
    SpShortcut shortcut_; ///< The shortcut
};


#endif // #ifndef BEEFTEXT_SHORTCUT_SNIPPET_FRAGMENT_H
