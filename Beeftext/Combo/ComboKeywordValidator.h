/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of validator for combo keywords
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__KEYWORD__VALIDATOR__H
#define BEEFTEXT__COMBO__KEYWORD__VALIDATOR__H


//**********************************************************************************************************************
/// \brief A validator for combo keyword
/// 
/// Note that this class only check that the keyword contains valid characters only, NOT the the combo keyword clashes
/// with another existing combo.
//**********************************************************************************************************************
class ComboKeywordValidator: public QValidator
{
public: // member functions
	ComboKeywordValidator(QObject* parent = nullptr); ///< Default constructor
	~ComboKeywordValidator() = default; ///< Default destructor
   void fixup(QString& input) const override; ///< Attempt to change the input to be valid according to the validator rules
   State validate(QString& input, int&) const override; ///< Validate the combo keyword
   State validate(QString& input) const; ///< Validate the combo keyword

private: // member functions
	ComboKeywordValidator(ComboKeywordValidator const&) = delete; ///< Disabled copy constructor
	ComboKeywordValidator(ComboKeywordValidator&&) = delete; ///< Disabled move constructor
	ComboKeywordValidator& operator=(ComboKeywordValidator const&) = delete; ///< Disabled assignment operator
	ComboKeywordValidator& operator=(ComboKeywordValidator&&) = delete; ///< Disabled move assignment operator
};


#endif // #ifndef BEEFTEXT__COMBO__KEYWORD__VALIDATOR__H
