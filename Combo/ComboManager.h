/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__MANAGER
#define BEEFTEXT__COMBO__MANAGER


#include "ComboList.h"
#include <memory>


//**********************************************************************************************************************
/// \brief A class managing the Combo and triggering the substitution when necessary
//**********************************************************************************************************************
class ComboManager: public QObject
{
   Q_OBJECT
public: // static member functions
   static ComboManager& instance(); ///< Returns a reference to the only allowed instance of the class

public: // member functions
	~ComboManager(); ///< Default destructor
   ComboList& getComboListRef(); ///< Return a mutable reference to the combo list
   ComboList const& getComboListRef() const; ///< Return a constant reference to the combo list
   bool loadComboListFromFile(QString* outErrorMsg = nullptr); ///< Load the combo list from the default file
   bool saveComboListToFile(QString* outErrorMsg = nullptr); /// Save the combo list to the default location

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
   ComboList comboList_; ///< The list of combos
   std::unique_ptr<QSound> sound_; ///< The sound to play when a combo is executed
};


#endif // #ifndef BEEFTEXT__COMBO__MANAGER