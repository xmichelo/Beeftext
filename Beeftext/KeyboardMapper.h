/// \file
/// \author 
///
/// \brief Declaration of keyboard related functions
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_KEYBOARD_H
#define BEEFTEXT_KEYBOARD_H


//****************************************************************************************************************************************************
/// \brief Class for mapping Windows keyboard input to Qt keys
//****************************************************************************************************************************************************
class KeyboardMapper {
public: // member functions
    static KeyboardMapper &instance(); ///< Return the only allowed instance of the class
    KeyboardMapper(KeyboardMapper const &) = delete; ///< Disabled copy-constructor.
    KeyboardMapper(KeyboardMapper &&) = delete; ///< Disabled assignment copy-constructor.
    ~KeyboardMapper() = default; ///< Destructor.
    KeyboardMapper &operator=(KeyboardMapper const &) = delete; ///< Disabled assignment operator.
    KeyboardMapper &operator=(KeyboardMapper &&) = delete; ///< Disabled move assignment operator.
    Qt::Key virtualKeyCodeToQtKey(quint32 vkCode); ///< Return the Qt key associated with a virtual key code.
    quint32 qtKeyToVirtualKeyCode(Qt::Key const &key); ///< Return the virtual key code for a Qt::Key.
    static Qt::KeyboardModifiers nativeModifiersToQtModifiers(quint32 modifiers); ///< Return the Qt modifier bitfield corresponding to a native motifier bitfield.
    static quint32 qtModifiersToNativeModifiers(Qt::KeyboardModifiers const &modifiers); ///< Return the native modifier bitfield corresponding to a Qt modifier bitfield.

private: // member functions
    KeyboardMapper(); ///< Default constructor.
    void computeMappings(); ///< Compute the mappings.

private: // data members
    QList<Qt::Key> qtKeysForVirtualKeys_; ///< An array containing the list of QtKey for each of the 256 virtual key codes
    QHash<Qt::Key, quint32> virtualKeysForQtKeys_; ///< A map that bind a QtKey to an virtual key code.
};


#endif // #ifndef BEEFTEXT_KEYBOARD_H
