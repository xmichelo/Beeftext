/// \file
/// \author 
///
/// \brief Declaration of string list editor model
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_STRING_LIST_EDITOR_MODEL_H
#define BEEFTEXT_STRING_LIST_EDITOR_MODEL_H


//**********************************************************************************************************************
/// \brief 
//**********************************************************************************************************************
class StringListEditorModel: public QStringListModel
{
public: // member functions
   explicit StringListEditorModel(QObject* parent = nullptr); ///< Default constructor
   StringListEditorModel(StringListEditorModel const&) = delete; ///< Disabled copy-constructor
   StringListEditorModel(StringListEditorModel&&) = delete; ///< Disabled assignment copy-constructor
   ~StringListEditorModel() = default; ///< Destructor
   StringListEditorModel& operator=(StringListEditorModel const&) = delete; ///< Disabled assignment operator
   StringListEditorModel& operator=(StringListEditorModel&&) = delete; ///< Disabled move assignment operator
   Qt::ItemFlags flags(const QModelIndex& index) const override; ///< Return the flags for the item at the selected index
};


#endif // #ifndef BEEFTEXT_STRING_LIST_EDITOR_MODEL_H
