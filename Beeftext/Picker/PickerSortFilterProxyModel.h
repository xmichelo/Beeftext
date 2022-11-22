/// \file
/// \author 
///
/// \brief Declaration of a sort/filter proxy model for the picker window.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PICKER_SORT_FILTER_PROXY_MODEL_H
#define BEEFTEXT_PICKER_SORT_FILTER_PROXY_MODEL_H


//****************************************************************************************************************************************************
/// \brief A sort/filter proxy model for the combo picker window
//****************************************************************************************************************************************************
class PickerSortFilterProxyModel : public QSortFilterProxyModel {
public: // member functions
    explicit PickerSortFilterProxyModel(QObject *parent = nullptr); ///< Default constructor
    PickerSortFilterProxyModel(PickerSortFilterProxyModel const &) = delete; ///< Disabled copy-constructor
    PickerSortFilterProxyModel(PickerSortFilterProxyModel &&) = delete; ///< Disabled assignment copy-constructor
    ~PickerSortFilterProxyModel() override = default; ///< Destructor
    PickerSortFilterProxyModel &operator=(PickerSortFilterProxyModel const &) = delete; ///< Disabled assignment operator
    PickerSortFilterProxyModel &operator=(PickerSortFilterProxyModel &&) = delete; ///< Disabled move assignment operator
    bool filterAcceptsRow(int sourceRow, const QModelIndex &) const override; ///< Check if a row should be included or discarded
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override; ///< Sort function for the filter
};


#endif // #ifndef BEEFTEXT_PICKER_SORT_FILTER_PROXY_MODEL_H
