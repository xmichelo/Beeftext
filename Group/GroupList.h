/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__GROUP__LIST__H
#define BEEFTEXT__COMBO__GROUP__LIST__H


#include "Group.h"


//**********************************************************************************************************************
/// \brief A class for combo group list
//**********************************************************************************************************************
class GroupList: public QAbstractListModel
{
   Q_OBJECT
public: // type definitions
   typedef VecSPGroup::iterator iterator; ///< Type definition for iterator
   typedef VecSPGroup::const_iterator const_iterator; ///< Type definition for const_iterator
   typedef VecSPGroup::reverse_iterator reverse_iterator; ///< Type definition for iterator
   typedef VecSPGroup::const_reverse_iterator const_reverse_iterator; ///< Type definition for const_iterator
   typedef SPGroup value_type; ///< The value-type for group

public: // friends
   friend void swap(GroupList& first, GroupList& second); ///< Swap two group lists

public: // member functions
   GroupList(QObject* parent = nullptr); ///< Default constructor
   GroupList(GroupList const& ref); ///< copy-constructor
   GroupList(GroupList&& ref); ///< assignment copy-constructor
   ~GroupList() = default; ///< Destructor
   GroupList& operator=(GroupList const& ref); ///< Assignment operator
   GroupList& operator=(GroupList&& ref); ///< Move assignment operator
   SPGroup& operator[](qint32 index); ///< Get a reference to the group at a given position in the list
   SPGroup const& operator[](qint32 index) const; ///< Get a constant reference to the group at a given position in the list
   qint32 size() const; ///< Return the size of the group list
   bool isEmpty() const;  ///< Test if the group list is empty
   void clear(); ///< Clear the group list
   bool contains(SPGroup const& group) const; ///< Check whether a group is already in the list, based on its UUID
   bool canGroupBeAdded(SPGroup const& group) const; ///< Check whether a group can be added
   bool append(SPGroup const& group); ///< Append a group at the end of the list
   void push_back(SPGroup const& group); ///< Append a group at the end of the list
   void erase(qint32 index); ///< Erase a group from the list
   iterator findByUuid(QUuid const& uuid); ///< Find a group using its UUID
   const_iterator findByUuid(QUuid const& uuid) const; ///< Find a group using its UUID
   iterator begin(); ///< Returns an iterator to the beginning of the list
   const_iterator begin() const; ///< Returns a constant iterator to the beginning of the list
   iterator end(); ///< Returns an iterator to the end of the list
   const_iterator end() const; ///< Returns a constant iterator to the end of the list
   reverse_iterator rbegin(); ///< Returns a reverse iterator to the beginning of the list
   const_reverse_iterator rbegin() const; ///< Returns a constant reverse iterator to the beginning of the list
   reverse_iterator rend(); ///< Returns a reverse iterator to the end of the list
   const_reverse_iterator rend() const; ///< Returns a constant reverse iterator to the end of the list
   QJsonArray toJsonArray() const; ///< Export the group list to a JSON array
   bool readFromJsonArray(QJsonArray const& array, qint32 formatVersion, QString* outErrorMessage); ///< Read the group list from a JSON array
   bool ensureNotEmpty(); ///< make sure that the group list is not empty, creating one if necessary

   /// \name List model member functions
   /// \{
   int rowCount(QModelIndex const& parent = QModelIndex()) const override; ///< Returns the number of rows in the model
   QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override; ///< Returns the model data at the given index for the given role
   Qt::DropActions supportedDropActions() const override; ///< Retrieve the supported drop actions
   Qt::ItemFlags flags(QModelIndex const& index) const override; ///< Retrieve the item flags
   QStringList mimeTypes() const; ///< Get the MIME type that are supported for dropping
   QMimeData* mimeData(const QModelIndexList &indexes) const override; ///< Get the MIME data for a given index
   bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent); ///< process the dropping of MIME data
   /// /}

signals:
   void groupMoved(SPGroup group, qint32 newIndex); ///< Signal for the moving of a group in the list.

private: // data members
   VecSPGroup groups_; ///< The list of groups
};


#endif // #ifndef BEEFTEXT__COMBO__GROUP__LIST__H