/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of sort and filter proxy model for combos
///
/// Sort and filter proxy models are Qt classes allowing custom filtering and sorting of item in table and list views
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMOBO__SORT__FILTER__PROXY__MODEL__H
#define BEEFTEXT__COMOBO__SORT__FILTER__PROXY__MODEL__H


#include "Group/Group.h"


//**********************************************************************************************************************
/// \brief A sort and filter proxy model class for combos
//**********************************************************************************************************************
class ComboSortFilterProxyModel: public QSortFilterProxyModel
{
   Q_OBJECT
public: // member functions
   ComboSortFilterProxyModel(QObject* parent = nullptr); ///< Default constructor
   ~ComboSortFilterProxyModel() = default; ///< Default destructor
   void setGroup(SPGroup const& group); ///< Set the group to display
    
protected: // member functions
   bool filterAcceptsRow(int sourceRow, QModelIndex const& index) const override; ///< Check if a row should be sorted

private: // member functions
   ComboSortFilterProxyModel(ComboSortFilterProxyModel const&) = delete; ///< Disabled copy constructor
   ComboSortFilterProxyModel(ComboSortFilterProxyModel&&) = delete; ///< Disabled move constructor
   ComboSortFilterProxyModel& operator=(ComboSortFilterProxyModel const&) = delete; ///< Disabled assignment operator
   ComboSortFilterProxyModel& operator=(ComboSortFilterProxyModel&&) = delete; ///< Disabled move assignment operator

private: // data members
   SPGroup group_; ///< The group to display
};


#endif // #ifndef BEEFTEXT__COMOBO__SORT__FILTER__PROXY__MODEL__H