/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of shortcut class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_SHORTCUT_H
#define BEEFTEXT_SHORTCUT_H


class Shortcut;


typedef std::shared_ptr<Shortcut> SpShortcut; ///< Type definition for shared pointer to Shortcut


//****************************************************************************************************************************************************
/// \brief A class for shortcuts
//****************************************************************************************************************************************************
class Shortcut {
public: // member functions
    Shortcut(Qt::KeyboardModifiers const &modifiers, Qt::Key const &key); ///< Default constructor
    explicit Shortcut(QKeyCombination const &keyCombination); ///< Create a shortcut from a key combination
    explicit Shortcut(qint32 combined); ///< create a shortcut from a integer containing packed information about the shortcut.
    Shortcut(Shortcut const &) = delete; ///< Disabled copy constructor
    Shortcut(Shortcut &&) = delete; ///< Disabled move constructor
    ~Shortcut() = default; ///< Default destructor
    Shortcut &operator=(Shortcut const &) = delete; ///< Disabled assignment operator
    Shortcut &operator=(Shortcut &&) = delete; ///< Disabled move assignment operator
    bool operator==(Shortcut const &other) const; ///< Overloaded equality check operator
    bool operator!=(Shortcut const &other) const; ///< Overloaded equality check operator
    QString toString() const; ///< Return a string representation of the shortcut
    QKeyCombination keyCombination() const; ///< Return the key combination for the shortcut.
    bool isValid() const; ///< Check whether a shortcut is valid
    Qt::KeyboardModifiers keyboardModifiers() const; ///< Return the native modifiers field of the shortcut
    Qt::Key key() const; ///< Return the key
    qint32 toCombined() const; ///< Return the key combination packed in a single integer.

public: // static member functions
    static SpShortcut fromModifiersAndKey(Qt::KeyboardModifiers modifiers, Qt::Key key); ///< Create a SpShortcut from a string (e.g. 'Ctrl+Shift+F2').
    static SpShortcut fromString(QString const &str); ///< Create a SpShortcut from a string (e.g. 'Ctrl+Shift+F2').
    static SpShortcut fromKeyCombination(QKeyCombination const &keyCombination); ///< Create a SpShortcut from a key combination.
    static SpShortcut fromCombined(qint32 combined); ///< Create a SpShortcut from a packed integer.

private: // data members
    QKeyCombination keyCombination_; ///< The key combination for the shortcut
};


#endif // #ifndef BEEFTEXT_SHORTCUT_H
