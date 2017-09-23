/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of Combo class that associate a combo text and its substitution text
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__H
#define BEEFTEXT__COMBO__H


#include <memory>
#include <vector>


//**********************************************************************************************************************
/// \brief The combo class that link a combo (a.k.a. snippet, or abbreviation), to it substitution text
//**********************************************************************************************************************
class Combo
{
public: // member functions
	Combo(QString const& aComboText = QString(), QString const& aSubstitutionText = QString()); ///< Default constructor
	~Combo() = default; ///< Default destructor
	QString comboText() const; ///< retrieve the combo text
   void setComboText(QString const& comboText); ///< Set the combo text
   QString substitutionText() const; ///< Retrieve the substitution text
   void setSubstitutionText(QString const& substitutionText); ///< Set the substitution text

private: // member functions
	Combo(Combo const&) = delete; ///< Disabled copy constructor
	Combo(Combo const&&) = delete; ///< Disabled move constructor
	Combo& operator=(Combo const&) = delete; ///< Disabled assignment operator
	Combo& operator=(Combo const&&) = delete; ///< Disabled move assignment operator

public: // data member
   QString comboText_; ///< The combo text
   QString substitutionText_; ///< The substitution text that will replace the combo text when
};


typedef std::shared_ptr<Combo> SPCombo; ///< Type definition for shared pointer to Combo
typedef std::vector<SPCombo> VecSPCombo; ///< Type definition for vector of SPCombo


#endif // #ifndef BEEFTEXT__COMBO__H

