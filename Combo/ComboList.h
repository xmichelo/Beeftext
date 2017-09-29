/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of Combo list class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEETTEXT__COMBO__LIST__H
#define BEETTEXT__COMBO__LIST__H


//**********************************************************************************************************************
/// \brief A class for combo lists
//**********************************************************************************************************************
class ComboList
{
public: // member functions
	ComboList(); ///< Default constructor
	~ComboList() = default; ///< Default destructor
	
private: // member functions
	ComboList(ComboList const&) = delete; ///< Disabled copy constructor
	ComboList(ComboList&&) = delete; ///< Disabled move constructor
	ComboList& operator=(ComboList const&) = delete; ///< Disabled assignment operator
	ComboList& operator=(ComboList&&) = delete; ///< Disabled move assignment operator
};


#endif // #ifndef BEETTEXT__COMBO__LIST__H