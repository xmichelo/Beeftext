/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of Combo class that associate a combo text and its substitution text
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "Combo.h"


//**********************************************************************************************************************
/// \param[in] comboText The combo text
//**********************************************************************************************************************
Combo::Combo(QString const& comboText, QString const& substitutionText)
   : comboText_(comboText)
   , substitutionText_(substitutionText)
{

}


//**********************************************************************************************************************
/// \return The combo text
//**********************************************************************************************************************
QString Combo::comboText() const
{
   return comboText_;
}


//**********************************************************************************************************************
// \param[in] comboText The combo text
//**********************************************************************************************************************
void Combo::setComboText(QString const& comboText)
{
   comboText_ = comboText;
}


//**********************************************************************************************************************
/// \return The substitution text
//**********************************************************************************************************************
QString Combo::substitutionText() const
{
   return substitutionText_;
}


//**********************************************************************************************************************
/// \param[in] substitutionText the substitution text
//**********************************************************************************************************************
void Combo::setSubstitutionText(QString const& substitutionText)
{
   substitutionText_ = substitutionText;
}


