/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of Combo class that associate a keyword and a snippet
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_COMBO_H
#define BEEFTEXT_COMBO_H


#include "Group/GroupList.h"
#include "MatchingMode.h"
#include "CaseSensitivity.h"
#include <memory>
#include <vector>


class Combo;


typedef std::shared_ptr<Combo> SpCombo; ///< Type definition for shared pointer to Combo
typedef std::vector<SpCombo> VecSpCombo; ///< Type definition for vector of SpCombo


//****************************************************************************************************************************************************
/// \brief The combo class that link a combo keyword and a snippet
//****************************************************************************************************************************************************
class Combo {
public: // static member functions
    static QString placeholderName(QString const &keyword, QString const &snippet);

public: // member functions
    Combo(QString name, QString keyword, QString snippet, QString description, EMatchingMode matchingMode,
        ECaseSensitivity caseSensitivity, bool enabled); ///< Default constructor
    Combo(QJsonObject const &object, qint32 formatVersion, GroupList const &groups = GroupList()); ///< Constructor from JSon object
    Combo(Combo const &) = delete; ///< Disabled copy constructor
    Combo(Combo &&) = delete; ///< Disabled move constructor
    ~Combo() = default; ///< Default destructor
    Combo &operator=(Combo const &) = delete; ///< Disabled assignment operator
    Combo &operator=(Combo &&) = delete; ///< Disabled move assignment operator
    bool isValid() const; ///< Is the combo valid
    QUuid uuid() const; ///< Get the UUID of the combo
    QString name() const; ///< Get the name of the combo
    QString placeholderName() const; ///< Return a placeholder name built using the keyword and snippet.
    QString displayName() const; ///< Get the display name of the combo. If name is empty, a placeholder name is returned
    void setName(QString const &name); ///< Set the name of the combo
    QString keyword() const; ///< retrieve the keyword
    void setKeyword(QString const &keyword); ///< Set the keyword
    QString snippet() const; ///< Retrieve the snippet
    void setSnippet(QString const &snippet); ///< Set the snippet
    QString description() const; ///< Retrieve the description of the snippet.
    void setDescription(QString const &description); ///< Set the description of the snippet.
    EMatchingMode matchingMode(bool resolveDefault) const; ///< Get the matching mode of the combo.
    void setMatchingMode(EMatchingMode mode); ///< Set the matching mode of the combo.
    ECaseSensitivity caseSensitivity(bool resolveDefault) const; ///< Get the case sensitivity of the combo.
    void setCaseSensitivity(ECaseSensitivity caseSensitivity); ///< Set the case sensitivity of the combo.
    QDateTime modificationDateTime() const; ///< Retrieve the last modification date/time of the combo
    QDateTime creationDateTime() const; ///< Retrieve the creation date/time of the combo
    void setLastUseDateTime(QDateTime const &dateTime); ///< Set the last use date time of the combo.
    QDateTime lastUseDateTime() const; ///< Retrieve the last use date/time of the combo.
    SpGroup group() const; ///< Get the combo group the combo belongs to
    void setGroup(SpGroup const &group); ///< Set the group this combo belongs to
    QString evaluatedSnippet(bool &outCancelled) const; ///< Retrieve the the snippet after having evaluated it, but leave the #{cursor} variable in place.
    QString evaluatedSnippet(bool &outCancelled, const QSet<QString> &forbiddenSubCombos,
        QMap<QString, QString> &knownInputVariables) const; ///< Retrieve the the snippet after having evaluated it, but leave the #{cursor} variable in place.
    void setEnabled(bool enabled); ///< Set the combo as enabled or not
    bool isEnabled() const; ///< Check whether the combo is enabled
    bool isUsable() const; ///< Check if the combo is usable, i.e. if it is enabled and member of a group that is enabled.
    bool matchesForInput(QString const &input) const; ///< Check if the combo is a match for the given input
    bool performSubstitution(bool triggeredByPicker); ///< Perform the combo substitution
    QJsonObject toJsonObject(bool includeGroup) const; ///< Serialize the combo in a JSon object
    void changeUuid(); ///< Get a new Uuid for the combo

public: // static functions
    static SpCombo create(QString const &name = QString(), QString const &keyword = QString(),
        QString const &snippet = QString(), QString const &description = QString(),
        EMatchingMode matchingMode = EMatchingMode::Default,
        ECaseSensitivity caseSensitivity = ECaseSensitivity::Default, bool enabled = true);
    static SpCombo create(QJsonObject const &object, qint32 formatVersion,
        GroupList const &groups = GroupList()); ///< create a Combo from a JSON object
    static SpCombo duplicate(Combo const &combo); ///< Duplicate

private: // member functions
    void touch(); ///< set the modification date/time to now

private: // data member
    QUuid uuid_; ///< The UUID of the combo
    QString name_; ///< The display name of the combo
    QString keyword_; ///< The keyword
    QString snippet_; ///< The snippet
    QString description_; ///< The description.
    EMatchingMode matchingMode_ { EMatchingMode::Default }; ///< The matching mode.
    ECaseSensitivity caseSensitivity_ { ECaseSensitivity::Default }; ///< The case sensitivity.
    SpGroup group_ { nullptr }; ///< The combo group this combo belongs to (may be null)
    QDateTime creationDateTime_; ///< The date/time of creation of the combo
    QDateTime modificationDateTime_; ///< The date/time of the last modification of the combo
    QDateTime lastUseDateTime_; ///< The last use date/time
    bool enabled_ { true }; ///< Is the combo enabled
};


extern QString const kPropUseHtml; ///< The JSON property for the "Use HTML" property, introduced in file format v7


#endif // #ifndef BEEFTEXT_COMBO_H

