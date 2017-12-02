/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of validator for combo text
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__TEXT__VALIDATOR__H
#define BEEFTEXT__COMBO__TEXT__VALIDATOR__H


//**********************************************************************************************************************
/// \brief A validator for combo text
/// 
/// Note that this class only check that the combo contains valid characters only, NOT the the combo text clashes
/// with another existing combo.
//**********************************************************************************************************************
class ComboTextValidator: public QValidator
{
public: // member functions
	ComboTextValidator(QObject* parent = nullptr); ///< Default constructor
	~ComboTextValidator() = default; ///< Default destructor
   void fixup(QString& input) const override; ///< Attempt to change the input to be valid according to the validator rules
   State validate(QString& input, int&) const override; ///< Validate the combo text
   State validate(QString& input) const; ///< Validate the combo text

private: // member functions
	ComboTextValidator(ComboTextValidator const&) = delete; ///< Disabled copy constructor
	ComboTextValidator(ComboTextValidator&&) = delete; ///< Disabled move constructor
	ComboTextValidator& operator=(ComboTextValidator const&) = delete; ///< Disabled assignment operator
	ComboTextValidator& operator=(ComboTextValidator&&) = delete; ///< Disabled move assignment operator
};


#endif // #ifndef BEEFTEXT__COMBO__TEXT__VALIDATOR__H