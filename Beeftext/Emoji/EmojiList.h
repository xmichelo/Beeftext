/// \file
/// \author 
///
/// \brief Declaration of class for emoji list.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_EMOJI_LIST_H
#define BEEFTEXT_EMOJI_LIST_H


#include "Emoji.h"


//****************************************************************************************************************************************************
/// \brief Class for emoji list.
//****************************************************************************************************************************************************
class EmojiList : public QAbstractTableModel {
public: // data type
    // ReSharper disable CppInconsistentNaming
    typedef QList<SpEmoji>::iterator iterator; ///< Type definition for iterator
    typedef QList<SpEmoji>::const_iterator const_iterator; ///< Type definition for const_iterator
    // ReSharper restore CppInconsistentNaming

public: // member functions
    explicit EmojiList(QObject *parent = nullptr); ///< Default constructor.
    EmojiList(EmojiList const &) = delete; ///< Disabled copy-constructor.
    EmojiList(EmojiList &&) = delete; ///< Disabled assignment copy-constructor.
    ~EmojiList() override = default; ///< Destructor.
    EmojiList &operator=(EmojiList const &) = delete; ///< Disabled assignment operator.
    EmojiList &operator=(EmojiList &&) = delete; ///< Disabled move assignment operator.
    SpEmoji &operator[](qint32 index); ///< Get a mutable reference to the combo at a given position in the list
    SpEmoji const &operator[](qint32 index) const; ///< Get a mutable reference to the combo at a given position in the list
    iterator begin(); ///< Returns an iterator to the beginning of the list
    const_iterator begin() const; ///< Returns a constant iterator to the beginning of the list
    iterator end(); ///< Returns an iterator to the end of the list
    const_iterator end() const; ///< Returns a constant iterator to the end of the list
    void clear(); ///< Clear the list
    bool contains(QString const &shortcode) const; ///< Check if the list contains an emoji with the given shortcode.
    SpEmoji find(QString const &shortcode) const; ///< Retrieve an emoji given its shortcode.
    void append(SpEmoji const &emoji); ///< Add an emoji at the end of the list
    qsizetype size() const; ///< Return the number of emojis in the list.
    bool isEmpty() const; ///< Check if the list is empty.

    // implementation of the Abstract table model interface
    int rowCount(const QModelIndex &parent) const override; ///< Return the row count for the model.
    int columnCount(const QModelIndex &parent) const override; ///< Return the column count for the model.
    QVariant data(const QModelIndex &index, int role) const override; ///< Return the data for the model.

private: // data members
    QList<SpEmoji> list_; ///<Type definition for a list of emojis.
};


#endif // #ifndef BEEFTEXT_EMOJI_LIST_H
