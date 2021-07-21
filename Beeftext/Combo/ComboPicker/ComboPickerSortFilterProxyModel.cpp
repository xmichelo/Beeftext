/// \file
/// \author 
///
/// \brief Implementation of a sort/filter proxy model for the combo picker window
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ComboPickerSortFilterProxyModel.h"
#include "BeeftextConstants.h"


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
   if (this->filterRegularExpression().pattern().trimmed().isEmpty())
      return false;

   QAbstractItemModel const* model = this->sourceModel();
   if ((!model) || (!model->columnCount(QModelIndex())) || 
      (!model->data(model->index(sourceRow, 0, QModelIndex()), constants::EnabledRole).toBool()))
      return false;
   QModelIndex const index = model->index(sourceRow, 0, QModelIndex());
   QString const comboName = model->data(index, Qt::DisplayRole).toString();
   QString const keyword = model->data(index, constants::KeywordRole).toString();
   QString const snippet = model->data(index, constants::SnippetRole).toString();
   QString const groupName = model->data(index, constants::GroupNameRole).toString();
   for (QString const& word: this->filterRegularExpression().pattern().replace("\\ ", " ")
      .split(QRegularExpression("\\s"), Qt::SkipEmptyParts))
   {
      QRegularExpression const rx(word, QRegularExpression::CaseInsensitiveOption);
      if ((!comboName.contains(rx)) && (!keyword.contains(rx))
         && (!snippet.contains(rx)) && (!groupName.contains(rx)))
         return false;
   }
   return true;
}


//**********************************************************************************************************************
/// \param[in] sourceLeft the index in the source model of the left item in the comparison.
/// \param[in] sourceRight the index in the source model of the right item in the comparison.
/// \ return true if and only if sourceLeft is strictly inferior 
//**********************************************************************************************************************
bool ComboPickerSortFilterProxyModel::lessThan(const QModelIndex& sourceLeft, const QModelIndex& sourceRight) const
{
   QDateTime lTime = this->sourceModel()->data(sourceLeft, constants::LastUseDateTimeRole).toDateTime();
   if (lTime.isNull())
      lTime = QDateTime::fromSecsSinceEpoch(0);
   QDateTime rTime = this->sourceModel()->data(sourceRight, constants::LastUseDateTimeRole).toDateTime();
   if (rTime.isNull())
      rTime = QDateTime::fromSecsSinceEpoch(0);
   return lTime < rTime;
}
