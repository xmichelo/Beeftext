/// \file
/// \author 
///
/// \brief Declaration of a delegate for the picker items.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PICKER_ITEM_DELEGATE_H
#define BEEFTEXT_PICKER_ITEM_DELEGATE_H


//****************************************************************************************************************************************************
/// \brief an item delegate for the picker.
//****************************************************************************************************************************************************
class PickerItemDelegate : public QItemDelegate {
Q_OBJECT
public: // member functions
    explicit PickerItemDelegate(QObject *parent = nullptr)
        : QItemDelegate(parent) {} ///< Default constructor
    PickerItemDelegate(PickerItemDelegate const &) = delete; ///< Disabled copy-constructor
    PickerItemDelegate(PickerItemDelegate &&) = delete; ///< Disabled assignment copy-constructor
    ~PickerItemDelegate() override = default; ///< Destructor
    PickerItemDelegate &operator=(PickerItemDelegate const &) = delete; ///< Disabled assignment operator
    PickerItemDelegate &operator=(PickerItemDelegate &&) = delete; ///< Disabled move assignment operator
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, QModelIndex const &) const override;
};


#endif // #ifndef BEEFTEXT_PICKER_ITEM_DELEGATE_H
