/// \file
/// \author 
///
/// \brief Implementation of a sort/filter proxy model for the combo picker window
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ComboPickerSortFilterProxyModel.h"
#include "../ComboManager.h"


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the instance.
//*********************************************************************************************************************
ComboPickerSortFilterProxyModel::ComboPickerSortFilterProxyModel(QObject* parent)
   : QSortFilterProxyModel(parent)
{
   this->setSortCaseSensitivity(Qt::CaseInsensitive);
   this->setFilterCaseSensitivity(Qt::CaseInsensitive);
}


//**********************************************************************************************************************
/// \param[in] sourceRow The row index in the source model
//**********************************************************************************************************************
bool ComboPickerSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex&) const
{
   ComboList& combos = ComboManager::instance().comboListRef();
   //if (this->filterRegExp().pattern().trimmed().isEmpty()) /* a empty search string will return an empty result list */
   //   return true;
   for (int col = 0; col < combos.columnCount(QModelIndex()); ++col)
   {
      SpCombo const& combo = combos[sourceRow];
      QString const str = combos.data(combos.index(sourceRow, col, QModelIndex()), Qt::DisplayRole).toString();
      if (str.contains(this->filterRegExp()))
         return true;
   }
   return false;
}
