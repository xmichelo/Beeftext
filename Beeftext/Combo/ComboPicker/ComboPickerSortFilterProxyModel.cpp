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
   if (this->filterRegExp().pattern().trimmed().isEmpty())
      return true;
   for (int col = 0; col < combos.columnCount(QModelIndex()); ++col)
   {
      SpCombo const& combo = combos[sourceRow];
      QString const str = combos.data(combos.index(sourceRow, col, QModelIndex()), Qt::DisplayRole).toString();
      if (str.contains(this->filterRegExp()))
         return true;
   }
   return false;
}


//**********************************************************************************************************************
/// \param[in] sourceLeft the index in the source model of the left item in the comparison.
/// \param[in] sourceRight the index in the source model of the right item in the comparison.
/// \ return true if and only if sourceLeft is strictly inferior 
//**********************************************************************************************************************
bool ComboPickerSortFilterProxyModel::lessThan(const QModelIndex& sourceLeft, const QModelIndex& sourceRight) const
{
   ComboList& combos = ComboManager::instance().comboListRef();
   QDateTime lTime = this->sourceModel()->data(sourceLeft, ComboList::LastUsedRole).toDateTime();
   if (lTime.isNull())
      lTime = QDateTime::fromSecsSinceEpoch(0);
   QDateTime rTime = this->sourceModel()->data(sourceRight, ComboList::LastUsedRole).toDateTime();
   if (rTime.isNull())
      rTime = QDateTime::fromSecsSinceEpoch(0);
   static qint32 i = 0;
   qDebug() << QString("%1() - %2").arg(__FUNCTION__).arg(i++);
   return lTime < rTime;
}
