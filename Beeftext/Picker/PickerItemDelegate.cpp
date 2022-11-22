/// \file
/// \author 
///
/// \brief Implementation of a delegate for the picker items.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PickerItemDelegate.h"
#include "Combo/Combo.h"
#include "Emoji/Emoji.h"
#include "Preferences/PreferencesManager.h"
#include "BeeftextConstants.h"


namespace {
qint32 kItemVMargin = 2; ///< The size of the vertical margin of an item.
qint32 kItemHMargin = 10; ///< The size of the horizontal margin of an item.
}


//****************************************************************************************************************************************************
/// \return The big font used in item rendering.
//****************************************************************************************************************************************************
QFont bigFont() {
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    font.setItalic(false);
    return font;
}


//****************************************************************************************************************************************************
/// \return The small font used in item rendering.
//****************************************************************************************************************************************************
QFont smallFont() {
    QFont font;
    font.setPointSize(8);
    font.setBold(true);
    font.setItalic(false);
    return font;
}


//****************************************************************************************************************************************************
/// \param[in] painter The painter.
/// \param[in] option The style options.
/// \param[in] index The index of the item to paint.
//****************************************************************************************************************************************************
void PickerItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    PreferencesManager const &prefs = PreferencesManager::instance();
    bool const light = ((!prefs.useCustomTheme()) || (ETheme::Light == prefs.theme()));
    QColor const bgColor = light ? QColor(0xff, 0xff, 0xff) : QColor(0x21, 0x21, 0x21);
    QColor const bgSelectedColor(0x50, 0x8c, 0xc8);
    QColor const bigTextColor = light ? QColor(0x4e, 0x4e, 0x4e) : QColor(0xee, 0xee, 0xee);
    QColor const bigTextSelectedColor(0xff, 0xff, 0xff);
    QColor const smallTextColor(0xc8, 0xc8, 0xc8);

    bool const isEmoji = (constants::Emoji == index.data(constants::TypeRole).value<constants::EITemType>());
    SpEmoji emoji = isEmoji ? index.data(constants::PointerRole).value<SpEmoji>() : nullptr;
    SpCombo combo = isEmoji ? nullptr : index.data(constants::PointerRole).value<SpCombo>();
    bool const ok = isEmoji ? !!emoji : !!combo;
    Q_ASSERT(ok);
    if (!ok)
        return;

    // we draw the background. Color depends on wether the item is selected or not.
    bool const selected = option.state & QStyle::State_Selected;
    painter->setBrush(selected ? bgSelectedColor : bgColor);
    painter->setPen(Qt::NoPen);
    painter->drawRect(option.rect);

    // we exclude the margin from the rendering rect.
    QRect const rect = option.rect.adjusted(kItemHMargin, kItemVMargin, -kItemHMargin, -kItemVMargin);

    // First line use big font.
    QFont const bFont = bigFont();
    QFontMetrics const bMetrics(bFont);
    // second line use small font.
    QFont const sFont = smallFont();

    QString const nameText = bMetrics.elidedText(index.data(Qt::DisplayRole).toString(), Qt::ElideRight, rect.width());
    qint32 const nameYPos = rect.top() + bMetrics.ascent();
    qint32 const nameWidth = bMetrics.boundingRect(nameText).width();
    painter->setFont(bFont);
    painter->setPen(selected ? bigTextSelectedColor : bigTextColor);
    painter->drawText(QPoint(rect.left(), nameYPos), nameText);

    painter->setPen(smallTextColor);
    painter->setFont(sFont);
    QFontMetrics const sMetrics(sFont);

    // draw keyword
    qint32 yPos = rect.bottom() - sMetrics.descent();
    painter->drawText(QPoint(rect.left(), yPos), isEmoji ? emoji->shortcode() : combo->keyword());

    // draw group name
    QString groupName = (isEmoji ? tr("Emojis") : (combo->group() ? combo->group()->name() : QString()));
    if (!groupName.trimmed().isEmpty()) {
        qint32 const groupNameMaxWidth = rect.width() - nameWidth - kItemHMargin;
        groupName = sMetrics.elidedText(groupName, Qt::ElideRight, groupNameMaxWidth);
        if (!groupName.isEmpty())
            painter->drawText(QPoint(rect.right() - sMetrics.boundingRect(groupName).width(), nameYPos), groupName);
    }
}


//****************************************************************************************************************************************************
/// \param[in] option The style options.
//****************************************************************************************************************************************************
QSize PickerItemDelegate::sizeHint(const QStyleOptionViewItem &option, QModelIndex const &) const {
    // we compute the size hint for the item. width is trivial, but height needs attention.
    // An item contains one line of text in big font, one line of text with small font, and there are margins.
    return QSize(option.rect.width(), QFontMetrics(bigFont()).height() + QFontMetrics(smallFont()).height()
                                      + 2 * kItemVMargin);
}


