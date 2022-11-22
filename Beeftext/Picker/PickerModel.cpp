/// \file
/// \author 
///
/// \brief Implementation of picker model
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PickerModel.h"
#include "Preferences/PreferencesManager.h"
#include "Emoji/EmojiManager.h"
#include "Combo/ComboManager.h"


//****************************************************************************************************************************************************
/// \param[in] parent The parent object of the model.
//****************************************************************************************************************************************************
PickerModel::PickerModel(QObject *parent)
    : QAbstractListModel(parent) {
    connect(&ComboManager::instance().comboListRef(), &ComboList::modelReset, this, &PickerModel::resetModel);
    connect(&EmojiManager::instance().emojiListRef(), &EmojiList::modelReset, this, &PickerModel::resetModel);
}


//****************************************************************************************************************************************************
/// \param[in] parent The index of the parent.
/// \return The number of row in the model.
//****************************************************************************************************************************************************
int PickerModel::rowCount(const QModelIndex &parent) const {
    PreferencesManager const &prefs = PreferencesManager::instance();
    return ComboManager::instance().comboListRef().rowCount(parent) +
           ((prefs.emojiShortcodesEnabled() && prefs.showEmojisInPickerWindow()) ?
            qint32(EmojiManager::instance().emojiListRef().size()) : 0);
}


//****************************************************************************************************************************************************
/// \param[in] index The index.
/// \param[in] role The role.
/// \return The data for the given role at the specified index.
//****************************************************************************************************************************************************
QVariant PickerModel::data(const QModelIndex &index, int role) const {
    ComboList const &comboList = ComboManager::instance().comboListRef();
    qint32 const comboListSize = comboList.size();
    qint32 const row = index.row();
    if (row < comboList.size()) {
        if (Qt::ToolTipRole == role)
            return comboList.data(index, Qt::DisplayRole);
        return comboList.data(index, role);
    }
    QModelIndex const shiftedIndex = createIndex(row - comboListSize, index.column());
    return EmojiManager::instance().emojiListRef().data(shiftedIndex, role);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PickerModel::resetModel() {
    this->beginResetModel();
    this->endResetModel();
}
