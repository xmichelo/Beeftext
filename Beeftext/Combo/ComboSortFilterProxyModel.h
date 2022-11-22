/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of sort and filter proxy model for combos
///
/// Sort and filter proxy models are Qt classes allowing custom filtering and sorting of item in table and list views
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_COMOBO_SORT_FILTER_PROXY_MODEL_H
#define BEEFTEXT_COMOBO_SORT_FILTER_PROXY_MODEL_H


#include "Group/Group.h"


//****************************************************************************************************************************************************
/// \brief A sort and filter proxy model class for combos
//****************************************************************************************************************************************************
class ComboSortFilterProxyModel : public QSortFilterProxyModel {
Q_OBJECT
public: // member functions
    explicit ComboSortFilterProxyModel(QObject *parent = nullptr); ///< Default constructor
    ComboSortFilterProxyModel(ComboSortFilterProxyModel const &) = delete; ///< Disabled copy constructor
    ComboSortFilterProxyModel(ComboSortFilterProxyModel &&) = delete; ///< Disabled move constructor
    ~ComboSortFilterProxyModel() override = default; ///< Default destructor
    ComboSortFilterProxyModel &operator=(ComboSortFilterProxyModel const &) = delete; ///< Disabled assignment operator
    ComboSortFilterProxyModel &operator=(ComboSortFilterProxyModel &&) = delete; ///< Disabled move assignment operator
    void setGroup(SpGroup const &group); ///< Set the group to display

protected: // member functions
    bool filterAcceptsRow(int sourceRow, QModelIndex const &) const override; ///< Check if a row should be included or discarded
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override; ///< Return true if and only if sourceLeft is inferior to sourceRight
private: // data members
    SpGroup group_; ///< The group to display
};


#endif // #ifndef BEEFTEXT_COMOBO_SORT_FILTER_PROXY_MODEL_H
