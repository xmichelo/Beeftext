/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of shortcut class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "Shortcut.h"


//****************************************************************************************************************************************************
/// \param[in] modifiers The modifiers for the shortcut.
/// \param[in] key The key.
//****************************************************************************************************************************************************
Shortcut::Shortcut(Qt::KeyboardModifiers const &modifiers, Qt::Key const &key)
    : keyCombination_(modifiers, key) {

}


//****************************************************************************************************************************************************
/// \param[in] keyCombination The key combination.
//****************************************************************************************************************************************************
Shortcut::Shortcut(QKeyCombination const &keyCombination)
    : keyCombination_(keyCombination) {
}


//****************************************************************************************************************************************************
/// \param combined An integer containing the shortcut
//****************************************************************************************************************************************************
Shortcut::Shortcut(qint32 combined) {
    keyCombination_ = QKeyCombination::fromCombined(combined);
}


//****************************************************************************************************************************************************
/// \return true if and only if the other other shortcut is identical to this
//****************************************************************************************************************************************************
bool Shortcut::operator==(Shortcut const &other) const {
    return other.keyCombination_ == this->keyCombination_;
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
bool Shortcut::operator!=(Shortcut const &other) const {
    return !(*this == other);
}


//****************************************************************************************************************************************************
/// \return A string describing the shortcut
//****************************************************************************************************************************************************
QString Shortcut::toString() const {
    return QKeySequence(keyCombination_).toString(QKeySequence::NativeText).replace("Meta", "Win", Qt::CaseSensitive);
}


//****************************************************************************************************************************************************
/// \return The key combination for the shortcut.
//****************************************************************************************************************************************************
QKeyCombination Shortcut::keyCombination() const {
    return keyCombination_;
}


//****************************************************************************************************************************************************
/// \return true if and only if the shortcut is valid
//****************************************************************************************************************************************************
bool Shortcut::isValid() const {
    Qt::KeyboardModifiers const mods = keyCombination_.keyboardModifiers();
    Qt::Key const key = keyCombination_.key();
    return (mods.testFlag(Qt::ControlModifier) || mods.testFlag(Qt::AltModifier) || mods.testFlag(Qt::MetaModifier)) &&
           (key != 0) && (key != Qt::Key_unknown);
}


//****************************************************************************************************************************************************
/// \return the modifiers field for the shortcut
//****************************************************************************************************************************************************
Qt::KeyboardModifiers Shortcut::keyboardModifiers() const {
    return keyCombination_.keyboardModifiers();
}


//****************************************************************************************************************************************************
/// \return the main key for the shortcut
//****************************************************************************************************************************************************
Qt::Key Shortcut::key() const {
    return keyCombination_.key();
}


//****************************************************************************************************************************************************
/// \return The shortcut packed into a single integer
//****************************************************************************************************************************************************
qint32 Shortcut::toCombined() const {
    return keyCombination_.toCombined();
}


/// \param[in] modifiers The modifiers.
/// \param[in] key The key
/// \return a shortcut corresponding to the modifiers and key.
/// \return A null pointer if the shortcut allocation failed.
//****************************************************************************************************************************************************
SpShortcut Shortcut::fromModifiersAndKey(Qt::KeyboardModifiers modifiers, Qt::Key key) {
    return fromKeyCombination(QKeyCombination(modifiers, key));
}


//****************************************************************************************************************************************************
/// \param[in] str The string containing the shortcut.
/// \return A shortcut corresponding to the string.
/// \return A null pointer if the string is not a valid shortcut.
//****************************************************************************************************************************************************
SpShortcut Shortcut::fromString(QString const &str) {
    QKeySequence const seq = QKeySequence::fromString(str, QKeySequence::NativeText);
    if (seq.count() < 1)
        return nullptr;
    QKeyCombination const key(seq[0]);
    return std::make_shared<Shortcut>(key.keyboardModifiers(), key.key());
}


//****************************************************************************************************************************************************
/// \param[in] keyCombination The key combination.
/// \return The shortcut representing the keyCombination.
/// \return A null pointer if an error occured
//****************************************************************************************************************************************************
SpShortcut Shortcut::fromKeyCombination(QKeyCombination const &keyCombination) {
    return std::make_shared<Shortcut>(keyCombination);
}


//****************************************************************************************************************************************************
/// \param[in] combined A integer containing the packed key combination.
/// \return The shortcut represented in the combined integer.
/// \return A null pointer if an error occured
//****************************************************************************************************************************************************
SpShortcut Shortcut::fromCombined(qint32 combined) {
    return std::make_shared<Shortcut>(combined);
}
