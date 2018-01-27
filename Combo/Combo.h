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


class Combo;


typedef std::shared_ptr<Combo> SPCombo; ///< Type definition for shared pointer to Combo
typedef std::vector<SPCombo> VecSPCombo; ///< Type definition for vector of SPCombo


//**********************************************************************************************************************
/// \brief The combo class that link a combo (a.k.a. snippet, or abbreviation), to it substitution text
//**********************************************************************************************************************
class Combo
{
public: // member functions
   Combo(QString const& name, QString const& aComboText, QString const& aSubstitutionText, bool enabled = true); ///< Default constructor
   Combo(QJsonObject const& object); ///< Constructor from JSon object
   ~Combo() = default; ///< Default destructor
   bool isValid() const; ///< Is the combo valid
   QString name() const; ///< Get the name of the combo
   void setName(QString const& name); ///< Set the name of the combo
	QString comboText() const; ///< retrieve the combo text
   void setComboText(QString const& comboText); ///< Set the combo text
   QString substitutionText() const; ///< Retrieve the substitution text
   void setSubstitutionText(QString const& substitutionText); ///< Set the substitution text
   void setEnabled(bool enabled); ///< Set the combo as enabled or not
   bool isEnabled() const; ///< Check whether the combo is enabled
   void performSubstitution(); ///< Perform the combo substitution
   QJsonObject toJsonObject() const; ///< Serialize the combo in a JSon object

public: // static functions
   static SPCombo create(QString const& name = QString(), QString const& aComboText = QString(),
      QString const& aSubstitutionText = QString(), bool enabled = true);
   static SPCombo create(QJsonObject const& object); ///< create a Combo from a JSON object
   static SPCombo duplicate(Combo const& combo); ///< Duplicate

private: // member functions
   Combo(Combo const&) = delete; ///< Disabled copy constructor
	Combo(Combo&&) = delete; ///< Disabled move constructor
	Combo& operator=(Combo const&) = delete; ///< Disabled assignment operator
	Combo& operator=(Combo&&) = delete; ///< Disabled move assignment operator
   void touch(); ///< set the modification date/time to now

private: // data member
   QUuid uuid_; ///< The UUID of the combo
   QString name_; ///< The display name of the combo
   QString comboText_; ///< The combo text
   QString substitutionText_; ///< The substitution text that will replace the combo text when
   QDateTime created_; ///< The date/time of creation of the combo
   QDateTime lastModified_; ///< The date/time of the last modification of the combo
   bool enabled_; ///< Is the combo enabled
};


#endif // #ifndef BEEFTEXT__COMBO__H

