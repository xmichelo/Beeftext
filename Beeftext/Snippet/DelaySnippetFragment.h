/// \file
/// \author 
///
/// \brief Declaration of delay snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_DELAY_SNIPPET_FRAGMENT_H
#define BEEFTEXT_DELAY_SNIPPET_FRAGMENT_H


#include "SnippetFragment.h"


//****************************************************************************************************************************************************
/// \brief Delay snippet fragment class.
//****************************************************************************************************************************************************
class DelaySnippetFragment : public SnippetFragment {
public: // member functions
    explicit DelaySnippetFragment(qint32 delayMs); ///< Default constructor.
    DelaySnippetFragment(DelaySnippetFragment const &) = delete; ///< Disabled copy-constructor.
    DelaySnippetFragment(DelaySnippetFragment &&) = delete; ///< Disabled assignment copy-constructor.
    ~DelaySnippetFragment() override = default; ///< Destructor.
    DelaySnippetFragment &operator=(DelaySnippetFragment const &) = delete; ///< Disabled assignment operator.
    DelaySnippetFragment &operator=(DelaySnippetFragment &&) = delete; ///< Disabled move assignment operator.
    EType type() const override; ///< Return the type of snippet fragment.
    void render() const override; ///< Render the snippet fragment.
    QString toString() const override; ///< Return a string describing the snippet fragment.

private: // data members
    qint32 delayMs_; ///< The delay in milliseconds.
};


#endif // #ifndef BEEFTEXT_DELAY_SNIPPET_FRAGMENT_H
