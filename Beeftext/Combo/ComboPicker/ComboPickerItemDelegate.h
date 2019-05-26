/// \file
/// \author 
///
/// \brief Declaration of a delegate for the combo picker items
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_COMBO_PICKER_ITEM_DELEGATE_H
#define BEEFTEXT_COMBO_PICKER_ITEM_DELEGATE_H


//**********************************************************************************************************************
/// \brief an item delegate for the combo picker.
//**********************************************************************************************************************
class ComboPickerItemDelegate: public QItemDelegate
{
   Q_OBJECT
public: // member functions
   explicit ComboPickerItemDelegate(QObject* parent = nullptr) : QItemDelegate(parent) {} ///< Default constructor
   ComboPickerItemDelegate(ComboPickerItemDelegate const&) = delete; ///< Disabled copy-constructor
   ComboPickerItemDelegate(ComboPickerItemDelegate&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboPickerItemDelegate() = default; ///< Destructor
   ComboPickerItemDelegate& operator=(ComboPickerItemDelegate const&) = delete; ///< Disabled assignment operator
   ComboPickerItemDelegate& operator=(ComboPickerItemDelegate&&) = delete; ///< Disabled move assignment operator
   void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
   QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override; 
};


#endif // #ifndef BEEFTEXT_COMBO_PICKER_ITEM_DELEGATE_H
