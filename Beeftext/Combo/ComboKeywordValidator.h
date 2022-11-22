/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of validator for combo keywords
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_COMBO_KEYWORD_VALIDATOR_H
#define BEEFTEXT_COMBO_KEYWORD_VALIDATOR_H


//****************************************************************************************************************************************************
/// \brief A validator for combo keyword
/// 
/// Note that this class only check that the keyword contains valid characters only, NOT the the combo keyword clashes
/// with another existing combo.
//****************************************************************************************************************************************************
class ComboKeywordValidator : public QValidator {
public: // member functions
    explicit ComboKeywordValidator(QObject *parent = nullptr); ///< Default constructor
    ComboKeywordValidator(ComboKeywordValidator const &) = delete; ///< Disabled copy constructor
    ComboKeywordValidator(ComboKeywordValidator &&) = delete; ///< Disabled move constructor
    ~ComboKeywordValidator() override = default; ///< Default destructor
    ComboKeywordValidator &operator=(ComboKeywordValidator const &) = delete; ///< Disabled assignment operator
    ComboKeywordValidator &operator=(ComboKeywordValidator &&) = delete; ///< Disabled move assignment operator
    void fixup(QString &input) const override; ///< Attempt to change the input to be valid according to the validator rules
    State validate(QString &input, int &) const override; ///< Validate the combo keyword
    State validate(QString &input) const; ///< Validate the combo keyword
};


#endif // #ifndef BEEFTEXT_COMBO_KEYWORD_VALIDATOR_H
