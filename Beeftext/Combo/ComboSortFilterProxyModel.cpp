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
#include "BeeftextConstants.h"


//****************************************************************************************************************************************************
/// \param[in] parent The parent object of the proxy model
//****************************************************************************************************************************************************
ComboSortFilterProxyModel::ComboSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {
    this->setSortCaseSensitivity(Qt::CaseInsensitive);
    this->setFilterCaseSensitivity(Qt::CaseInsensitive);
}


//****************************************************************************************************************************************************
/// \param[in] group The group.
//****************************************************************************************************************************************************
void ComboSortFilterProxyModel::setGroup(SpGroup const &group) {
    group_ = group;
    this->invalidateFilter();
}


//****************************************************************************************************************************************************
/// \param[in] sourceRow The row index in the source model
//****************************************************************************************************************************************************
bool ComboSortFilterProxyModel::filterAcceptsRow(int sourceRow, QModelIndex const &) const {
    ComboList &combos = ComboManager::instance().comboListRef();
    SpCombo const &combo = combos[sourceRow];
    if (group_ && (combo->group() != group_))
        return false;
    for (QString const &word: this->filterRegularExpression().pattern().replace("\\ ", " ")
        .split(QRegularExpression("\\s"), Qt::SkipEmptyParts)) {
        QRegularExpression const rx(word, QRegularExpression::CaseInsensitiveOption);
        if ((!combo->name().contains(rx)) && (!combo->keyword().contains(rx)) && (!combo->snippet().contains(rx))
            && (!combo->description().contains(rx)))
            return false;
    }
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] sourceLeft The index of the left part of the comparison.
/// \param[in] sourceRight The index of the right part of the comparison.
/// \return true if and only if sourceLeft is strictly inferior to sourceRight.
//****************************************************************************************************************************************************
bool ComboSortFilterProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const {
    bool const leftIsCombo = (constants::Combo == sourceLeft.data(constants::TypeRole).value<constants::EITemType>());
    bool const rightIsCombo = (constants::Combo == sourceRight.data(constants::TypeRole).value<constants::EITemType>());
    SpCombo const lCombo(sourceLeft.data(constants::PointerRole).value<SpCombo>());
    SpCombo const rCombo(sourceRight.data(constants::PointerRole).value<SpCombo>());
    bool const ok = (leftIsCombo && rightIsCombo && lCombo && rCombo);
    Q_ASSERT(ok);
    if (!ok)
        return false;
    switch (sourceLeft.column()) {
    case 3:
        return lCombo->creationDateTime() < rCombo->creationDateTime();
    case 4:
        return lCombo->modificationDateTime() < rCombo->modificationDateTime();
    case 5:
        return lCombo->lastUseDateTime() < rCombo->lastUseDateTime();
    default:
        return QSortFilterProxyModel::lessThan(sourceLeft, sourceRight);
    }
}
