/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of sort and filter proxy model for combos
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboSortFilterProxyModel.h"
#include "ComboList.h"
#include "ComboManager.h"


//**********************************************************************************************************************
/// \param[in] parent The parent object of the proxy model
//**********************************************************************************************************************
ComboSortFilterProxyModel::ComboSortFilterProxyModel(QObject* parent)
   : QSortFilterProxyModel(parent)
{
   this->setSortCaseSensitivity(Qt::CaseInsensitive);
   this->setFilterCaseSensitivity(Qt::CaseInsensitive);
}


//**********************************************************************************************************************
/// \param[in] group
//**********************************************************************************************************************
void ComboSortFilterProxyModel::setGroup(SpGroup const& group)
{
   group_ = group;
   this->invalidateFilter();
}


//**********************************************************************************************************************
/// \param[in] sourceRow The row index in the source model
//**********************************************************************************************************************
bool ComboSortFilterProxyModel::filterAcceptsRow(int sourceRow, QModelIndex const&) const
{
   ComboList& combos = ComboManager::instance().comboListRef();

   for (int col = 0; col < combos.columnCount(QModelIndex()); ++col)
   {
      SpCombo const& combo = combos[sourceRow];
      if (group_ && (combo->group() != group_))
         return false;
      QString const str = combos.data(combos.index(sourceRow, col, QModelIndex()), Qt::DisplayRole).toString();
      if (str.contains(this->filterRegExp()))
         return true;
   }
   return false;
}


//**********************************************************************************************************************
/// \param[in] sourceLeft The index of the left part of the comparison.
/// \param[in] sourceRight The index of the right part of the comparison.
/// \return true if and only if sourceLeft is strictly inferior to sourceRight.
//**********************************************************************************************************************
bool ComboSortFilterProxyModel::lessThan(const QModelIndex& sourceLeft, const QModelIndex& sourceRight) const
{
   qint32 const rowLeft = sourceLeft.row();
   qint32 const rowRight = sourceRight.row();
   ComboList const& combos = ComboManager::instance().comboListRef();
   switch (sourceLeft.column())
   {
   case 3:
      return combos[rowLeft]->creationDateTime() < combos[rowRight]->creationDateTime();
   case 4:
      return combos[rowLeft]->modificationDateTime() <  combos[rowRight]->modificationDateTime();
   case 5:
      return combos[rowLeft]->lastUsedDateTime() < combos[rowRight]->lastUsedDateTime();
   default:
      return QSortFilterProxyModel::lessThan(sourceLeft, sourceRight);
   }
}

