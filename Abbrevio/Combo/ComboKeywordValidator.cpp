/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of validator for combo keyword
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboKeywordValidator.h"


bool isCharacterAcceptable(QChar const &c); ///< Is the char acceptable in a combo keyword




//****************************************************************************************************************************************************
/// \param[in] c The character
/// \return true if and only if c is acceptable in a combo string
//****************************************************************************************************************************************************
bool isCharacterAcceptable(QChar const &c) {
    // we accept all printable characters except white spaces
    return (!c.isSpace()) && (c.isPrint());
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent object of the validator
//****************************************************************************************************************************************************
ComboKeywordValidator::ComboKeywordValidator(QObject *parent)
    : QValidator(parent) {

}


//****************************************************************************************************************************************************
/// \param[in,out] input The input to fix
//****************************************************************************************************************************************************
void ComboKeywordValidator::fixup(QString &input) const {
    QString result;
    for (QChar const c: input)
        if (isCharacterAcceptable(c))
            result.append(c);
    input.swap(result);
}


//****************************************************************************************************************************************************
/// \param[in] input The input text to validate
/// \return The validation state of the text
//****************************************************************************************************************************************************
QValidator::State ComboKeywordValidator::validate(QString &input, int &) const {
    for (QChar const &c: input)
        if (!isCharacterAcceptable(c))
            return Invalid;
    return Acceptable;
}


//****************************************************************************************************************************************************
/// This convenience function is the same as its variant with pos
///
/// \param[in] input The input text to validate
/// \return The validation state of the text
//****************************************************************************************************************************************************
QValidator::State ComboKeywordValidator::validate(QString &input) const {
    int unused = 0;
    return validate(input, unused);
}


