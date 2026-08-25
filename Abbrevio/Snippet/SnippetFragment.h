/// \file
/// \author 
///
/// \brief Declaration of snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFEXT_SNIPPET_FRAGMENT_H
#define BEEFEXT_SNIPPET_FRAGMENT_H


//****************************************************************************************************************************************************
/// \brief Snippet fragment class.
//****************************************************************************************************************************************************
class SnippetFragment {
public: // data types
    enum class EType {
        Text = 0, ///< Text fragment.
        Delay = 1, ///< Delay fragment.
        Key = 2, ///< Key fragment.
        Shortcut = 3, ///< Shortcut fragment
        Count = 4, ///< The number of fragment types
    }; ///< Enumeration for the type of fragment

public: // member functions
    SnippetFragment() = default; ///< Default constructor.
    SnippetFragment(SnippetFragment const &) = delete; ///< Disabled copy-constructor.
    SnippetFragment(SnippetFragment &&) = delete; ///< Disabled assignment copy-constructor.
    virtual ~SnippetFragment() = default; ///< Destructor.
    SnippetFragment &operator=(SnippetFragment const &) = delete; ///< Disabled assignment operator.
    SnippetFragment &operator=(SnippetFragment &&) = delete; ///< Disabled move assignment operator.
    virtual EType type() const = 0; ///< The type of fragment.
    virtual QString toString() const = 0; ///< Return a string describing the snippet fragment.
    virtual void render() const = 0; ///< Render the snippet fragment.
};


typedef std::shared_ptr<SnippetFragment> SpSnippetFragment; ///< Type definition for shared pointer to snippet fragment.
typedef QList<SpSnippetFragment> ListSpSnippetFragment; ///< Type definition for vector of SpSnippetFragment.


ListSpSnippetFragment splitStringIntoSnippetFragments(QString const &str); ///< Split a string snippet fragments.
void renderSnippetFragmentList(ListSpSnippetFragment const &fragments); ///< Render a list of snippet fragments.


#endif // #ifndef BEEFEXT_SNIPPET_FRAGMENT_H
