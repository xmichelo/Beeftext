/// \file
/// \author 
///
/// \brief Implementation of a sort/filter proxy model for the picker window
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PickerSortFilterProxyModel.h"
#include "Emoji/Emoji.h"
#include "Combo/Combo.h"
#include "BeeftextConstants.h"


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the instance.
//*********************************************************************************************************************
PickerSortFilterProxyModel::PickerSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {
    this->setSortCaseSensitivity(Qt::CaseInsensitive);
    this->setFilterCaseSensitivity(Qt::CaseInsensitive);
}


//****************************************************************************************************************************************************
/// \param[in] sourceRow The row index in the source model
//****************************************************************************************************************************************************
bool PickerSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &) const {
    QString const pattern = this->filterRegularExpression().pattern().trimmed();
    QAbstractItemModel const *model = this->sourceModel();
    if ((!model) || (!model->columnCount(QModelIndex())))
        return false;
    QModelIndex const index = model->index(sourceRow, 0, QModelIndex());
    bool const isEmoji = (index.data(constants::TypeRole).value<constants::EITemType>() == constants::Emoji);
    if (isEmoji) {
        if (pattern.size() < 3)
            return false;
    } else if (pattern.isEmpty())
        return true;

    SpEmoji const emoji(isEmoji ? index.data(constants::PointerRole).value<SpEmoji>() : nullptr);
    SpCombo const combo(isEmoji ? nullptr : index.data(constants::PointerRole).value<SpCombo>());
    bool const ok = isEmoji ? !!emoji.get() : !!combo.get();
    Q_ASSERT(ok);
    if ((!ok) || ((!isEmoji) && (!combo->isUsable())))
        return false;

    QString const comboName = model->data(index, Qt::DisplayRole).toString();
    QString const keyword = isEmoji ? emoji->shortcode() : combo->keyword();
    QString const snippet = isEmoji ? emoji->value() : combo->snippet();
    QString const description = isEmoji ? QString() : combo->description();
    SpGroup const group = isEmoji ? nullptr : combo->group();
    for (QString const &word: this->filterRegularExpression().pattern().replace("\\ ", " ")
        .split(QRegularExpression("\\s"), Qt::SkipEmptyParts)) {
        QRegularExpression const rx(word, QRegularExpression::CaseInsensitiveOption);
        if ((!comboName.contains(rx)) && (!keyword.contains(rx)) && (!snippet.contains(rx))
            && (!(group && group->name().contains(rx))) && (!description.contains(rx)))
            return false;
    }
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] sourceLeft the index in the source model of the left item in the comparison.
/// \param[in] sourceRight the index in the source model of the right item in the comparison.
/// \ return true if and only if sourceLeft is strictly inferior 
//****************************************************************************************************************************************************
bool PickerSortFilterProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const {
    bool const lIsCombo = (sourceLeft.data(constants::TypeRole).value<constants::EITemType>() == constants::Combo);
    bool const rIsCombo = (sourceRight.data(constants::TypeRole).value<constants::EITemType>() == constants::Combo);
    SpCombo const lCombo = lIsCombo ? sourceLeft.data(constants::PointerRole).value<SpCombo>() : nullptr;
    SpCombo const rCombo = rIsCombo ? sourceRight.data(constants::PointerRole).value<SpCombo>() : nullptr;
    SpEmoji const lEmoji = !lIsCombo ? sourceLeft.data(constants::PointerRole).value<SpEmoji>() : nullptr;
    SpEmoji const rEmoji = !rIsCombo ? sourceRight.data(constants::PointerRole).value<SpEmoji>() : nullptr;
    QDateTime const originTime = QDateTime::fromSecsSinceEpoch(0);
    QDateTime const lTime = lIsCombo ? (lCombo ? lCombo->lastUseDateTime() : originTime) :
                            (lEmoji ? lEmoji->lastUseDateTime() : originTime);
    QDateTime const rTime = rIsCombo ? (rCombo ? rCombo->lastUseDateTime() : originTime) :
                            (rEmoji ? rEmoji->lastUseDateTime() : originTime);
    return lTime < rTime;
}
