/// \file
/// \author 
///
/// \brief Implementation of combo picker model
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ComboPickerModel.h"
#include "../ComboManager.h"


//**********************************************************************************************************************
/// \param[in] parent The parent object of the model.
//**********************************************************************************************************************
ComboPickerModel::ComboPickerModel(QObject* parent)
   : QAbstractListModel(parent)
{
   ComboList& comboList = ComboManager::instance().comboListRef();
   connect(&comboList, &ComboList::modelReset, this, &ComboPickerModel::resetModel);
}


//**********************************************************************************************************************
/// \param[in] parent The index of the parent.
/// \return The number of row in the model.
//**********************************************************************************************************************
int ComboPickerModel::rowCount(const QModelIndex& parent) const
{
   return ComboManager::instance().comboListRef().rowCount(parent);
}


//**********************************************************************************************************************
/// \param[in] index The index.
/// \param[in] role The role.
/// \return The data for the given role at the specified index.
//**********************************************************************************************************************
QVariant ComboPickerModel::data(const QModelIndex& index, int role) const
{
   ComboList const& comboList = ComboManager::instance().comboListRef();
   if (Qt::ToolTipRole == role)
      return comboList.data(index, ComboList::SnippetRole);
   return comboList.data(index, role);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboPickerModel::resetModel()
{
   this->beginResetModel();
   this->endResetModel();
}
