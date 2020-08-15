/// \file
/// \author 
///
/// \brief Declaration of custom text edit widget
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_SNIPPET_EDIT_H
#define BEEFTEXT_SNIPPET_EDIT_H


//**********************************************************************************************************************
/// \brief A custom snippet edit that intercept and process pasted text.
//**********************************************************************************************************************
class SnippetEdit: public QTextEdit
{
   Q_OBJECT
public: // member functions
   explicit SnippetEdit(QWidget* parent = nullptr); ///< Default constructor
   SnippetEdit(SnippetEdit const&) = delete; ///< Disabled copy-constructor
   SnippetEdit(SnippetEdit&&) = delete; ///< Disabled assignment copy-constructor
   ~SnippetEdit() override = default; ///< Destructor
   SnippetEdit& operator=(SnippetEdit const&) = delete; ///< Disabled assignment operator
   SnippetEdit& operator=(SnippetEdit&&) = delete; ///< Disabled move assignment operator

private:
   void insertFromMimeData(const QMimeData* source) override; ///< Overriden version of the insert MIME format
};


#endif // #ifndef BEEFTEXT_SNIPPET_EDIT_H
