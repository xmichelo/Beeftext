/// \file
/// \author 
///
/// \brief Implementation of string list editor model
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "StringListEditorModel.h"


//**********************************************************************************************************************
/// \param[in] parent The parent object of the model
//**********************************************************************************************************************
StringListEditorModel::StringListEditorModel(QObject* parent)
   : QStringListModel(parent)
{
}


//**********************************************************************************************************************
/// \param[in] stringList The string list
/// \param[in] parent The parent object of the model
//**********************************************************************************************************************
StringListEditorModel::StringListEditorModel(QStringList const& stringList, QObject* parent)
   : QStringListModel(stringList, parent)
{
}


//**********************************************************************************************************************
/// \param[in] index The index of the item
/// \return The flags for the item
//**********************************************************************************************************************
Qt::ItemFlags StringListEditorModel::flags(const QModelIndex& index) const
{
   // we only allow drop between items, not on item, i.e. when the index is invalid
   Qt::ItemFlags const flags = QStringListModel::flags(index); 
   return index.isValid() ? flags & ~Qt::ItemIsDropEnabled : flags;
}
