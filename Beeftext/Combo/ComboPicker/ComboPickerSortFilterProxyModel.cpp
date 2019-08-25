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
   if (this->filterRegExp().pattern().trimmed().isEmpty())
      return true;

   QAbstractItemModel const* model = this->sourceModel();
   if ((!model) || (!model->data(model->index(sourceRow, 0, QModelIndex()), ComboList::EnabledRole).toBool()))
      return false;

   for (int col = 0; col < model->columnCount(QModelIndex()); ++col)
   {
      QString const str = model->data(model->index(sourceRow, col, QModelIndex()), Qt::DisplayRole).toString();
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
   QDateTime lTime = this->sourceModel()->data(sourceLeft, ComboList::LastUseDateTimeRole).toDateTime();
   if (lTime.isNull())
      lTime = QDateTime::fromSecsSinceEpoch(0);
   QDateTime rTime = this->sourceModel()->data(sourceRight, ComboList::LastUseDateTimeRole).toDateTime();
   if (rTime.isNull())
      rTime = QDateTime::fromSecsSinceEpoch(0);
   return lTime < rTime;
}
