/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of Combo list class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEETTEXT__COMBO__LIST__H
#define BEETTEXT__COMBO__LIST__H


#include "Combo.h"
#include "Group/GroupList.h"


//**********************************************************************************************************************
/// \brief A class for combo lists
//**********************************************************************************************************************
class ComboList: public QAbstractTableModel
{
   Q_OBJECT
public: // type definitions
   typedef VecSPCombo::iterator iterator; ///< Type definition for iterator
   typedef VecSPCombo::const_iterator const_iterator; ///< Type definition for const_iterator
   typedef VecSPCombo::reverse_iterator reverse_iterator; ///< Type definition for iterator
   typedef VecSPCombo::const_reverse_iterator const_reverse_iterator; ///< Type definition for const_iterator
   typedef SPCombo value_type;

public: // static data members
   static QString const defaultFileName; ///< The default name for combo list files

public: // friends
   friend void swap(ComboList& first, ComboList& second); ///< Swap two combo lists

public: // member functions
	ComboList(QObject* parent = nullptr); ///< Default constructor
   ComboList(ComboList const& ref); ///< Copy constructor
   ComboList(ComboList&& ref); ///< Move constructor
   ~ComboList() = default; ///< Default destructor
   ComboList& operator=(ComboList const& ref); ///< Assignment operator
   ComboList& operator=(ComboList&& ref); ///< Move assignment operator
   GroupList& groupListRef(); ///< Return a mutable reference to the group list
   GroupList const& groupListRef() const; ///< Return a constant reference to the group list
   qint32 size() const; ///< Return the size of the combo list
   bool isEmpty() const;  ///< Test if the combo list is empty
   void clear(); ///< Clear the combo list
   bool contains(SPCombo const& combo) const; ///< Check whether a combo is already in the list, based on its UUID
   bool isKeywordUsed(QString const& keyword) const; ///< Check whether a keyword is already used in the list
   bool canComboBeAdded(SPCombo const& combo) const; ///< Check whether a combo can be added
   bool append(SPCombo const& combo); ///< Append a combo at the end of the list
   void push_back(SPCombo const& combo); ///< Append a combo at the end of the list
   void erase(qint32 index); ///< Erase a combo from the list
   void eraseCombosOfGroup(SPGroup const& group); ///< Erase all the combos of a given group
   ComboList::const_iterator findByKeyword(QString const& keyword) const; ///< Find a combo by its keyword
   ComboList::iterator findByKeyword(QString const& keyword); ///< Find a combo by its keyword
   ComboList::const_iterator findByUuid(QUuid const& uuid) const; ///< Find a combo by its UUID
   ComboList::iterator findByUuid(QUuid const& uuid); ///< Find a combo by its UUID
   SPCombo& operator[](qint32 index); ///< Get a mutable reference to the combo at a given position in the list
   SPCombo const& operator[](qint32 index) const; ///< Get a mutable reference to the combo at a given position in the list
   iterator begin(); ///< Returns an iterator to the beginning of the list
   const_iterator begin() const; ///< Returns a constant iterator to the beginning of the list
   iterator end(); ///< Returns an iterator to the end of the list
   const_iterator end() const; ///< Returns a constant iterator to the end of the list
   reverse_iterator rbegin(); ///< Returns a reverse iterator to the beginning of the list
   const_reverse_iterator rbegin() const; ///< Returns a constant reverse iterator to the beginning of the list
   reverse_iterator rend(); ///< Returns a reverse iterator to the end of the list
   const_reverse_iterator rend() const; ///< Returns a constant reverse iterator to the end of the list
   QJsonDocument toJsonDocument(bool includeGroups) const; ///< Export the Combo list to a JSon document
   bool readFromJsonDocument(QJsonDocument const& doc, bool* outInOlderFileFormat = nullptr, 
      QString* outErrorMsg = nullptr); ///< Read a combo list from a JSON document
   bool save(QString const& path, bool saveGroups, QString* outErrorMessage = nullptr) const; ///< Save a combo list to a JSON file
   bool load(QString const& path, bool* outInOlderFileFormat = nullptr, QString* outErrorMessage = nullptr); /// Load a combo list from a JSON file
   void markComboAsEdited(qint32 index); ///< Mark a combo as edited
   void ensureCorrectGrouping(bool *outWasInvalid = false); ///< make sure every combo is affected to a group (and that there is at least one group

   /// \name Table model member functions
   ///\{
   int rowCount(QModelIndex const& = QModelIndex()) const override; ///< Retrieve the number of row in the table model
   int columnCount(QModelIndex const & = QModelIndex()) const override; ///< Retrieve the number of row in the table model
   QVariant data(QModelIndex const& index = QModelIndex(), int role = Qt::DisplayRole) const override; ///< Retrieve the data from the table model
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override; ///< Retrieve header data from the table model
   Qt::DropActions supportedDropActions() const override; ///< Retrieve the supported drop actions
   Qt::ItemFlags flags(QModelIndex const& index) const override; ///< Retrieve the item flags
   QStringList mimeTypes() const; ///< Get the MIME type that are supported for dropping
   QMimeData* mimeData(const QModelIndexList &indexes) const override; ///< Get the MIME data for some indexes
   //bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent); ///< process the dropping of MIME data
                                                                                                                    ///\}

private: // data members
   VecSPCombo combos_; ///< The list of combos
   GroupList groups_; ///< The list of groups
};


#endif // #ifndef BEETTEXT__COMBO__LIST__H