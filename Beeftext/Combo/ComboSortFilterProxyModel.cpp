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
/// \param[in] group The group.
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
   SpCombo const& combo = combos[sourceRow];
   if (group_ && (combo->group() != group_))
      return false;
   for (QString const& word : this->filterRegularExpression().pattern().split(QRegularExpression("\\s"), 
      Qt::SkipEmptyParts))
      if ((!combo->name().contains(word, Qt::CaseInsensitive)) 
         && (!combo->keyword().contains(word, Qt::CaseInsensitive))
         && (!combo->snippet().contains(word, Qt::CaseInsensitive)))
         return false;
   return true;
}


//**********************************************************************************************************************
/// \param[in] sourceLeft The index of the left part of the comparison.
/// \param[in] sourceRight The index of the right part of the comparison.
/// \return true if and only if sourceLeft is strictly inferior to sourceRight.
//**********************************************************************************************************************
bool ComboSortFilterProxyModel::lessThan(const QModelIndex& sourceLeft, const QModelIndex& sourceRight) const
{
   switch (sourceLeft.column())
   {
   case 3:
      return sourceLeft.data(ComboList::CreationDateTimeRole).toDateTime()
         < sourceRight.data(ComboList::CreationDateTimeRole).toDateTime();
   case 4:
      return sourceLeft.data(ComboList::ModificationDateTimeRole).toDateTime()
         < sourceRight.data(ComboList::ModificationDateTimeRole).toDateTime();
   case 5:
      return sourceLeft.data(ComboList::LastUseDateTimeRole).toDateTime()
         < sourceRight.data(ComboList::LastUseDateTimeRole).toDateTime();
   default:
      return QSortFilterProxyModel::lessThan(sourceLeft, sourceRight);
   }
}
