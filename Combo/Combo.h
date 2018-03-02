/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of Combo class that associate a keyword and a snippet
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
/// \brief The combo class that link a combo keyword and a snippet
//**********************************************************************************************************************
class Combo
{
public: // member functions
   Combo(QString const& name, QString const& aKeyword, QString const& aSnippet, bool enabled = true); ///< Default constructor
   Combo(QJsonObject const& object, qint32 formatVersion); ///< Constructor from JSon object
   ~Combo() = default; ///< Default destructor
   bool isValid() const; ///< Is the combo valid
   QUuid uuid() const; ///< Get the UUID of the combo
   QString name() const; ///< Get the name of the combo
   void setName(QString const& name); ///< Set the name of the combo
	QString keyword() const; ///< retrieve the keyword
   void setKeyword(QString const& keyword); ///< Set the keyword
   QString snippet() const; ///< Retrieve the snippet
   QDateTime modificationDateTime() const; ///< Retrieve the last modification date/time of the combo
   QDateTime creationDateTime() const; ///< Retrieve the creation date/time of the combo

   QString evaluatedSnippet(qint32* outCursorPos = nullptr, 
      QSet<QString> forbiddenSubcombos = QSet<QString>()) const; ///< Retrieve the the snippet after having evaluated it
   void setSnippet(QString const& snippet); ///< Set the snippet
   void setEnabled(bool enabled); ///< Set the combo as enabled or not
   bool isEnabled() const; ///< Check whether the combo is enabled
   void performSubstitution(); ///< Perform the combo substitution
   QJsonObject toJsonObject() const; ///< Serialize the combo in a JSon object
   void changeUuid(); ///< Get a new Uuid for the combo

public: // static functions
   static SPCombo create(QString const& name = QString(), QString const& aKeyword = QString(),
      QString const& aSnippet = QString(), bool enabled = true);
   static SPCombo create(QJsonObject const& object, qint32 formatVersion); ///< create a Combo from a JSON object
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
   QString keyword_; ///< The keyword
   QString snippet_; ///< The snippet
   QDateTime creationDateTime_; ///< The date/time of creation of the combo
   QDateTime modificationDateTime_; ///< The date/time of the last modification of the combo
   bool enabled_; ///< Is the combo enabled
};


#endif // #ifndef BEEFTEXT__COMBO__H

