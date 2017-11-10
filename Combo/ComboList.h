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


//**********************************************************************************************************************
/// \brief A class for combo lists
//**********************************************************************************************************************
class ComboList: public QAbstractTableModel
{
   Q_OBJECT

   typedef VecSPCombo::iterator iterator; ///< Type definition for iterator
   typedef VecSPCombo::const_iterator const_iterator; ///< Type definition for const_iterator
   typedef VecSPCombo::reverse_iterator reverse_iterator; ///< Type definition for iterator
   typedef VecSPCombo::const_reverse_iterator const_reverse_iterator; ///< Type definition for const_iterator

public: // member functions
	ComboList(QObject* parent = nullptr); ///< Default constructor
	~ComboList() = default; ///< Default destructor
	qint32 size() const; ///< Return the size of the combo list
   bool isEmpty() const;  ///< Test if the combo list is empty
   void clear(); ///< Clear the combo list
   void append(SPCombo const& combo); ///< Append a combo at the end of the list
   void erase(qint32 index); ///< Erase a combo from the list
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
   QJsonDocument toJsonDocument() const; ///< Export the Combo list to a JSon document
   bool readFromJsonDocument(QJsonDocument const& doc, QString* outErrorMsg = nullptr); ///< Read a combo list from a JSON document
   void markComboAsEdited(qint32 index); ///< Mark a combo as edited
   /// \name Table model member functions
   ///\{
   int rowCount(QModelIndex const& = QModelIndex()) const override; ///< Retrieve the number of row in the table model
   int columnCount(QModelIndex const & = QModelIndex()) const override; ///< Retrieve the number of row in the table model
   QVariant data(QModelIndex const& index = QModelIndex(), int role = Qt::DisplayRole) const override; ///< Retrieve the data from the table model
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override; ///< Retrieve header data from the table model
   ///\}

private: // member functions
	ComboList(ComboList const&) = delete; ///< Disabled copy constructor
	ComboList(ComboList&&) = delete; ///< Disabled move constructor
	ComboList& operator=(ComboList const&) = delete; ///< Disabled assignment operator
	ComboList& operator=(ComboList&&) = delete; ///< Disabled move assignment operator

private: // data members
   VecSPCombo combos_; ///< The list of combos
};


#endif // #ifndef BEETTEXT__COMBO__LIST__H