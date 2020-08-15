/// \file
/// \author 
///
/// \brief Declaration of a sort/filter proxy model for the combo picker window
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_COMBO_PICKER_SORT_FILTER_PROXY_MODEL_H
#define BEEFTEXT_COMBO_PICKER_SORT_FILTER_PROXY_MODEL_H


//**********************************************************************************************************************
/// \brief A sort/filter proxy model for the combo picker window
//**********************************************************************************************************************
class ComboPickerSortFilterProxyModel: public QSortFilterProxyModel
{
public: // member functions
   explicit ComboPickerSortFilterProxyModel(QObject* parent = nullptr); ///< Default constructor
   ComboPickerSortFilterProxyModel(ComboPickerSortFilterProxyModel const&) = delete; ///< Disabled copy-constructor
   ComboPickerSortFilterProxyModel(ComboPickerSortFilterProxyModel&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboPickerSortFilterProxyModel() override = default; ///< Destructor
   ComboPickerSortFilterProxyModel& operator=(ComboPickerSortFilterProxyModel const&) = delete; ///< Disabled assignment operator
   ComboPickerSortFilterProxyModel& operator=(ComboPickerSortFilterProxyModel&&) = delete; ///< Disabled move assignment operator
   bool filterAcceptsRow(int sourceRow, const QModelIndex&) const override; ///< Check if a row should be included or discarded
   bool lessThan(const QModelIndex& sourceLeft, const QModelIndex& sourceRight) const override; ///< Sort function for the filter
};


#endif // #ifndef BEEFTEXT_COMBO_PICKER_SORT_FILTER_PROXY_MODEL_H
