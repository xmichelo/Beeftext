/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__MANAGER
#define BEEFTEXT__COMBO__MANAGER


#include "Combo.h"


//**********************************************************************************************************************
/// \brief A class managing the Combo and triggering the substitution when necessary
//**********************************************************************************************************************
class ComboManager: public QObject
{
   Q_OBJECT
public: // static member functions
   static ComboManager& instance(); ///< Returns a reference to the only allowed instance of the class

public: // member functions
	~ComboManager() = default; ///< Default destructor

private: // member functions
   ComboManager(); ///< Default constructor
   ComboManager(ComboManager const&) = delete; ///< Disabled copy constructor
	ComboManager(ComboManager const&&) = delete; ///< Disabled move constructor
	ComboManager& operator=(ComboManager const&) = delete; ///< Disabled assignment operator
	ComboManager& operator=(ComboManager const&&) = delete; ///< Disabled move assignment operator

private: // slots
   void onComboBreakerTyped(); ///< Slot for the "Combo Breaker Typed" signal
   void onCharacterTyped(QChar c); ///< Slot for the "Character Typed" signal
   void onBackspaceTyped(); ///< Slot for the 'Backspace typed" signal

private: // data member
   QString currentText_; ///< The current string
   VecSPCombo comboList_; ///< The list of combos
};


#endif // #ifndef BEEFTEXT__COMBO__MANAGER