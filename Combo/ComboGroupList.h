/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__GROUP__LIST__H
#define BEEFTEXT__COMBO__GROUP__LIST__H


#include "ComboGroup.h"


//**********************************************************************************************************************
/// \brief A class for combo group list
//**********************************************************************************************************************
class ComboGroupList: public QAbstractTableModel
{
   Q_OBJECT
public: // type definitions
   typedef VecSPComboGroup::iterator iterator; ///< Type definition for iterator
   typedef VecSPComboGroup::const_iterator const_iterator; ///< Type definition for const_iterator
   typedef VecSPComboGroup::reverse_iterator reverse_iterator; ///< Type definition for iterator
   typedef VecSPComboGroup::const_reverse_iterator const_reverse_iterator; ///< Type definition for const_iterator
   typedef SPComboGroup value_type; ///< The value-type for group

public: // member functions
   ComboGroupList(QObject* parent = nullptr); ///< Default constructor
   ComboGroupList(ComboGroupList const&) = delete; ///< Disabled copy-constructor
   ComboGroupList(ComboGroupList&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboGroupList() = default; ///< Destructor
   ComboGroupList& operator=(ComboGroupList const&) = delete; ///< Disabled assignment operator
   ComboGroupList& operator=(ComboGroupList&&) = delete; ///< Disabled move assignment operator
   qint32 size() const; ///< Return the size of the combo list
   bool isEmpty() const;  ///< Test if the combo list is empty
   void clear(); ///< Clear the combo list
   bool contains(SPComboGroup const& group) const; ///< Check whether a group is already in the list, based on its UUID
   bool canGroupBeAdded(SPComboGroup const& group) const; ///< Check whether a group can be added
   bool append(SPComboGroup const& group); ///< Append a combo at the end of the list
   void push_back(SPComboGroup const& group); ///< Append a combo at the end of the list
   void erase(qint32 index); ///< Erase a combo from the list
   iterator findByUuid(QUuid const& uuid); ///< Find a combo using its UUID
   const_iterator findByUuid(QUuid const& uuid) const; ///< Find a combo using its UUID
   iterator begin(); ///< Returns an iterator to the beginning of the list
   const_iterator begin() const; ///< Returns a constant iterator to the beginning of the list
   iterator end(); ///< Returns an iterator to the end of the list
   const_iterator end() const; ///< Returns a constant iterator to the end of the list
   reverse_iterator rbegin(); ///< Returns a reverse iterator to the beginning of the list
   const_reverse_iterator rbegin() const; ///< Returns a constant reverse iterator to the beginning of the list
   reverse_iterator rend(); ///< Returns a reverse iterator to the end of the list
   const_reverse_iterator rend() const; ///< Returns a constant reverse iterator to the end of the list
   QJsonArray toJsonArray() const; ///< Export the combo list to a JSON array
   bool readFromJsonArray(QJsonArray const& array, qint32 formatVersion, QString* outErrorMessage); ///< Read the combo list from a JSON array

private: // data members
   VecSPComboGroup groups_; ///< The list of combo groups
};


#endif // #ifndef BEEFTEXT__COMBO__GROUP__LIST__H