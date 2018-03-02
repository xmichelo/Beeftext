/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__GROUP__CLASS__H
#define BEEFTEXT__COMBO__GROUP__CLASS__H


class ComboGroup;


typedef std::shared_ptr<ComboGroup> SPComboGroup; ///< Type definition for shared pointer to SPComboGroup
typedef std::vector<ComboGroup> VecSPComboGroup; ///< Type definition for vector of SPComboGroup


//**********************************************************************************************************************
/// \brief Combo group class
//**********************************************************************************************************************
class ComboGroup
{
public: // member functions
   ComboGroup(QString const& name, QString const& description = QString()); ///< Default constructor
   ComboGroup(QJsonObject const& object, qint32 formatVersion); ///< Constructor from JSON object
   ComboGroup(ComboGroup const&) = delete; ///< Disabled copy-constructor
   ComboGroup(ComboGroup&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboGroup() = default; ///< Destructor
   ComboGroup& operator=(ComboGroup const&) = delete; ///< Disabled assignment operator
   ComboGroup& operator=(ComboGroup&&) = delete; ///< Disabled move assignment operator
   bool isValid() const; ///< Check if the group is valid
   QUuid uuid() const; ///< Get the UUID of the group
   QString name() const; ///< Get the name of the group
   void setName(QString const& name); ///< Set the name of the group
   QString description() const; ///< Get the description of the group
   void setDescription(QString const& description); ///< Set the description of the groups
   QJsonObject toJsonObject() const; ///< Serialize the combo in a JSon object

public: // static functions
   static SPComboGroup create(QString const& name, QString const& description = QString()); ///< Create a SPCombo
   static SPComboGroup create(QJsonObject const& object, qint32 formatVersion); ///< Create a SCombo from a JSON object

private: // member functions
   void touch(); ///< Set the modification date/time to the current date/time

private: // data members
   QUuid uuid_; ///< The UUID for the group
   QString name_; ///< The name of the group
   QString description_; ///< The description of the group
   QDateTime creationDateTime_; ///< The creation date/time of the group
   QDateTime modificationDateTime_; ///< The last modification date/time of the group
};


#endif // #ifndef BEEFTEXT__COMBO__GROUP__CLASS__H