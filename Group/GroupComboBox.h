/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo box class to select a group


#ifndef BEEFTEXT__GROUP__COMBO__BOX__H
#define BEEFTEXT__GROUP__COMBO__BOX__H


#include "Group/GroupList.h"

//**********************************************************************************************************************
/// \brief A specialized QComboBox class for picking a combo group
//**********************************************************************************************************************
class GroupComboBox: public QComboBox
{
   Q_OBJECT
public: // member functions
   GroupComboBox(QWidget* parent = nullptr); ///< Default constructor
   GroupComboBox(GroupComboBox const&) = delete; ///< Disabled copy-constructor
   GroupComboBox(GroupComboBox&&) = delete; ///< Disabled assignment copy-constructor
   ~GroupComboBox() = default; ///< Destructor
   GroupComboBox& operator=(GroupComboBox const&) = delete; ///< Disabled assignment operator
   GroupComboBox& operator=(GroupComboBox&&) = delete; ///< Disabled move assignment operator
   void setContent(GroupList const& groups = GroupList()); ///< Fill the combo box with the specified group list
   void setCurrentGroup(SPGroup const& group); ///< Select a group in the combo
   SPGroup currentGroup(); ///< Get the currently selected group
};


#endif // #ifndef BEEFTEXT__GROUP__COMBO__BOX__H
