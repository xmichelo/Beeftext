/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_COMBO_GROUP_LIST_H
#define BEEFTEXT_COMBO_GROUP_LIST_H


#include "Group.h"
#include <set>


//****************************************************************************************************************************************************
/// \brief A class for combo group list
//****************************************************************************************************************************************************
class GroupList : public QAbstractListModel {

Q_OBJECT
public: // type definitions
    // ReSharper disable CppInconsistentNaming
    typedef VecSpGroup::iterator iterator; ///< Type definition for iterator
    typedef VecSpGroup::const_iterator const_iterator; ///< Type definition for const_iterator
    typedef VecSpGroup::reverse_iterator reverse_iterator; ///< Type definition for iterator
    typedef VecSpGroup::const_reverse_iterator const_reverse_iterator; ///< Type definition for const_iterator
    typedef SpGroup value_type; ///< The value-type for group
    // ReSharper restore CppInconsistentNaming

    enum EDropType {
        GroupDrop = 0, ///< Group drop mode
        ComboDrop = 1, ///< Combo drop mode
    };

public: // friends
    friend void swap(GroupList &first, GroupList &second) noexcept; ///< Swap two group lists

public: // member functions
    explicit GroupList(QObject *parent = nullptr); ///< Default constructor
    GroupList(GroupList const &ref); ///< copy-constructor
    GroupList(GroupList &&ref) noexcept; ///< assignment copy-constructor
    ~GroupList() override = default; ///< Destructor
    GroupList &operator=(GroupList const &ref); ///< Assignment operator
    GroupList &operator=(GroupList &&ref) noexcept; ///< Move assignment operator
    SpGroup &operator[](qint32 index); ///< Get a reference to the group at a given position in the list
    SpGroup const &operator[](qint32 index) const; ///< Get a constant reference to the group at a given position in the list
    qint32 size() const; ///< Return the size of the group list
    bool isEmpty() const;  ///< Test if the group list is empty
    void clear(); ///< Clear the group list
    bool contains(SpGroup const &group) const; ///< Check whether a group is already in the list, based on its UUID
    bool canGroupBeAdded(SpGroup const &group) const; ///< Check whether a group can be added
    bool append(SpGroup const &group); ///< Append a group at the end of the list
    // ReSharper disable once CppInconsistentNaming
    void push_back(SpGroup const &group); ///< Append a group at the end of the list
    void erase(qint32 index); ///< Erase a group from the list
    iterator findByUuid(QUuid const &uuid); ///< Find a group using its UUID
    const_iterator findByUuid(QUuid const &uuid) const; ///< Find a group using its UUID
    iterator begin(); ///< Returns an iterator to the beginning of the list
    const_iterator begin() const; ///< Returns a constant iterator to the beginning of the list
    iterator end(); ///< Returns an iterator to the end of the list
    const_iterator end() const; ///< Returns a constant iterator to the end of the list
    reverse_iterator rbegin(); ///< Returns a reverse iterator to the beginning of the list
    const_reverse_iterator rbegin() const; ///< Returns a constant reverse iterator to the beginning of the list
    reverse_iterator rend(); ///< Returns a reverse iterator to the end of the list
    const_reverse_iterator rend() const; ///< Returns a constant reverse iterator to the end of the list
    QJsonArray toJsonArray() const; ///< Export the group list to a JSON array
    bool readFromJsonArray(QJsonArray const &array, qint32 formatVersion, QString *outErrorMessage); ///< Read the group list from a JSON array
    bool ensureNotEmpty(); ///< make sure that the group list is not empty, creating one if necessary
    QMenu *createMenu(QString const &title, std::set<SpGroup> const &disabledGroups, QWidget *parent = nullptr) const; ///< Create a containing the list of groups
    void fillMenu(QMenu *menu, std::set<SpGroup> const &disabledGroups) const; ///< Clear the menu and fill it with the list of groups
    void setDropType(EDropType dropType); ///< Set the drop type
    bool processComboListDrop(QList<QUuid> const &uuids, qint32 index); ///< Process the dropping of a combo list
    bool processGroupDrop(qint32 groupIndex, qint32 dropIndex); ///< Process the dropping of a combo list
    /// \name List model member functions
    /// \{
    int rowCount(QModelIndex const &) const override; ///< Returns the number of rows in the model
    QVariant data(QModelIndex const &index, int role) const override; ///< Returns the model data at the given index for the given role
    Qt::DropActions supportedDropActions() const override; ///< Retrieve the supported drop actions
    Qt::ItemFlags flags(QModelIndex const &index) const override; ///< Retrieve the item flags
    QStringList mimeTypes() const override; ///< Get the MIME type that are supported for dropping
    QMimeData *mimeData(const QModelIndexList &indexes) const override; ///< Get the MIME data for a given index
    bool dropMimeData(const QMimeData *data, Qt::DropAction, int row, int, const QModelIndex &parent) override; ///< process the dropping of MIME data
    /// /}

signals:
    void groupMoved(SpGroup group, qint32 newIndex); ///< Signal for the moving of a group in the list.
    void combosChangedGroup(); ///< Signal for the changing of combo groups.

private: // data members
    VecSpGroup groups_; ///< The list of groups
    EDropType dropType_; ///< The type of data the user is about to drop
};


#endif // #ifndef BEEFTEXT_COMBO_GROUP_LIST_H
