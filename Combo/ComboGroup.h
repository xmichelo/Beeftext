/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__GROUP__H
#define BEEFTEXT__COMBO__GROUP__H


#include <memory>


//**********************************************************************************************************************
/// \brief A class for combo groups
//**********************************************************************************************************************
class ComboGroup
{
public: // member functions
	ComboGroup(QString const& name = QString(), QString const& description = QString()); ///< Default constructor
	~ComboGroup() = default; ///< Default destructor
   bool isValid() const; ///< Check if a combo group is valid
	QString name() const; ///< Get the name of the group
   void setName(QString const& name); ///< Set the name of the group
   QString description() const; ///< Get the description of the group
   void setDescription(QString const& description); ///< Set the description of the group

private: // member functions
	ComboGroup(ComboGroup const&) = delete; ///< Disabled copy constructor
	ComboGroup(ComboGroup&&) = delete; ///< Disabled move constructor
	ComboGroup& operator=(ComboGroup const&) = delete; ///< Disabled assignment operator
	ComboGroup& operator=(ComboGroup&&) = delete; ///< Disabled move assignment operator

private: // data members
   QString name_; ///< The name of the group
   QString description_; ///< The description of the group
};


typedef std::shared_ptr<ComboGroup> SPComboGroup; ///< Type definition for shared pointer to combo group

#endif // #ifndef BEEFTEXT__COMBO__GROUP__H
