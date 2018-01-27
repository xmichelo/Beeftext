/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of shortcut class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__SHORTCUT__H
#define BEEFTEXT__SHORTCUT__H


//**********************************************************************************************************************
/// \brief A class for shortcuts
//**********************************************************************************************************************
class Shortcut
{
public: // member functions
	Shortcut(Qt::KeyboardModifiers const& modifiers, quint32 nativeKey, quint32 nativeScanCode); ///< Default constructor
	~Shortcut() = default; ///< Default destructor
   QString toString() const; ///< Return a string representation of the shortcut
   bool isValid() const; ///< Check whether a shortcut is valid
   Qt::KeyboardModifiers nativeModifiers() const; ///< Return the native modifiers field of the shortcut
   quint32 nativeVirtualKey() const; ///< Return the native virtual key of the shortcut
   quint32 nativeScanCode() const; ///< Return the native scan code of the shortcut

private: // member functions
	Shortcut(Shortcut const&) = delete; ///< Disabled copy constructor
	Shortcut(Shortcut&&) = delete; ///< Disabled move constructor
	Shortcut& operator=(Shortcut const&) = delete; ///< Disabled assignment operator
	Shortcut& operator=(Shortcut&&) = delete; ///< Disabled move assignment operator

private: // data members
   Qt::KeyboardModifiers modifiers_; ///< The modifiers for the shortcut
   quint32 nativeVirtualKey_; ///< The native virtual key code for the shortcut
   quint32 nativeScanCode_; ///< The native scan code for the shortcut
};


typedef std::shared_ptr<Shortcut> SPShortcut; ///< Type definition for shared pointer to Shortcut


#endif // #ifndef BEEFTEXT__SHORTCUT__H