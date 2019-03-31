/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of Combo class that associate a keyword and a snippet
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "Combo.h"
#include "ComboVariable.h"
#include "ComboManager.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <utility>


using namespace xmilib;


namespace {
QString const kPropUuid = "uuid"; ///< The JSon property name for the UUID
QString const kPropName = "name"; ///< The JSON property name for the name
QString const kPropComboText = "comboText";
///< The JSON property for the "combo text", deprecated in combo list file format v2, replaced by "keyword"
QString const kPropKeyword = "keyword";
///< The JSON property for the for the keyword, introduced in the combo list file format v2, replacing "combo text"
QString const kPropSubstitutionText = "substitutionText";
///< The JSON property name for the substitution text, deprecated in combo list file format v2, replaced by "snippet"
QString const kPropSnippet = "snippet";
///< The JSON property name for the snippet, introduced in the combo list file format v2, replacing "substitution text"
QString const kPropUseLooseMatching = "useLooseMatch"; ///< The JSON property for the 'use loose matching' option
QString const kPropGroup = "group"; ///< The JSON property name for the combo group 
QString const kPropCreated = "created";
///< The JSON property name for the created date/time, deprecated in combo list file format v3, replaced by "creationDateTime"
QString const kPropCreationDateTime = "creationDateTime";
///< The JSON property name for the created date/time, introduced in the combo list file format v3, replacing "created"
QString const kPropLastModified = "lastModified";
///< The JSON property name for the modification date/time, deprecated in combo list file format v3, replaced by "modificationDateTime"
QString const kPropModificationDateTime = "modificationDateTime";
///< The JSON property name for the modification date/time, introduced in the combo list file format v3, replacing "lastModified"
QString const kPropEnabled = "enabled"; ///< The JSON property name for the enabled/disabled state


} // anonymous namespace


//**********************************************************************************************************************
/// \param[in] name The display name of the combo
/// \param[in] keyword The keyword
/// \param[in] snippet The text that will replace the combo
/// \param[in] useLooseMatching Should the combo use loose matching
/// \param[in] enabled Is the combo enabled
//**********************************************************************************************************************
Combo::Combo(QString name, QString keyword, QString snippet, bool const useLooseMatching, bool const enabled)
   : uuid_(QUuid::createUuid()),
     name_(std::move(name)),
     keyword_(std::move(keyword)),
     snippet_(std::move(snippet)),
     useLooseMatching_(useLooseMatching),
     group_(nullptr),
     enabled_(enabled)
{
   modificationDateTime_ = creationDateTime_ = QDateTime::currentDateTime();
}


//**********************************************************************************************************************
/// If the JSon object is not a valid combo, the constructed combo will be invalid
/// \param[in] object The object to read from
/// \param[in] formatVersion The version number of the combo list file format
/// \param[in] groups The list of combo groups
//**********************************************************************************************************************
Combo::Combo(QJsonObject const& object, qint32 formatVersion, GroupList const& groups)
   : uuid_(QUuid(object[kPropUuid].toString())),
     name_(object[kPropName].toString()),
     keyword_(object[formatVersion >= 2 ? kPropKeyword : kPropComboText].toString()),
     snippet_(object[formatVersion >= 2 ? kPropSnippet : kPropSubstitutionText].toString()),
     useLooseMatching_(object[kPropUseLooseMatching].toBool(false)),
     group_(nullptr),
     creationDateTime_(QDateTime::fromString(object[formatVersion >= 3 ? kPropCreationDateTime :
        kPropCreated].toString(), constants::kJsonExportDateFormat)),
     modificationDateTime_(QDateTime::fromString(object[formatVersion >= 3 ? kPropModificationDateTime :
        kPropLastModified].toString(), constants::kJsonExportDateFormat)),
     enabled_(object[kPropEnabled].toBool(true))
{
   if (object.contains(kPropGroup))
   {
      QUuid const uuid(object[kPropGroup].toString());
      GroupList::const_iterator const it = groups.findByUuid(uuid);
      if (it != groups.end())
         group_ = *it;
      else
         globals::debugLog().addWarning("While parsing combo file, a combo with an invalid group was found.");
   }

   // because we parse a older format version, we update the modification date, as the combo manager will save 
   // the file to update it to the latest format
   if (formatVersion < ComboList::fileFormatVersionNumber)
      this->touch();
}


//**********************************************************************************************************************
/// \return true if and only if the combo is valid
//**********************************************************************************************************************
bool Combo::isValid() const
{
   return (!creationDateTime_.isNull()) && (!modificationDateTime_.isNull()) && (!uuid_.isNull())
      && (!keyword_.isEmpty()) && (!snippet_.isEmpty());
}


//**********************************************************************************************************************
/// \return The UUID of the combo
//**********************************************************************************************************************
QUuid Combo::uuid() const
{
   return uuid_;
}


//**********************************************************************************************************************
/// \return The display name of the combo
//**********************************************************************************************************************
QString Combo::name() const
{
   return name_;
}


//**********************************************************************************************************************
/// \param[in] name The display name of the combo
//**********************************************************************************************************************
void Combo::setName(QString const& name)
{
   if (name_ != name)
   {
      name_ = name;
      this->touch();
   }
}


//**********************************************************************************************************************
/// \return The keyword
//**********************************************************************************************************************
QString Combo::keyword() const
{
   return keyword_;
}


//**********************************************************************************************************************
/// \param[in] keyword The keyword
//**********************************************************************************************************************
void Combo::setKeyword(QString const& keyword)
{
   if (keyword_ != keyword)
   {
      keyword_ = keyword;
      this->touch();
   }
}


//**********************************************************************************************************************
/// \return The snippet
//**********************************************************************************************************************
QString Combo::snippet() const
{
   return snippet_;
}


//**********************************************************************************************************************
/// \param[in] snippet The snippet
//**********************************************************************************************************************
void Combo::setSnippet(QString const& snippet)
{
   if (snippet_ != snippet)
   {
      snippet_ = snippet;
      this->touch();
   }
}


//**********************************************************************************************************************
/// \return true if and only if the combo uses loose matching
//**********************************************************************************************************************
bool Combo::useLooseMatching() const
{
   return useLooseMatching_;
}


//**********************************************************************************************************************
/// \param[in] useLooseMatching Should the combo use loose matching
//**********************************************************************************************************************
void Combo::setUseLooseMatching(bool useLooseMatching)
{
   if (useLooseMatching_ != useLooseMatching)
   {
      useLooseMatching_ = useLooseMatching;
      this->touch();
   }
}


//**********************************************************************************************************************
/// \return The last modification date of the combo
//**********************************************************************************************************************
QDateTime Combo::modificationDateTime() const
{
   return modificationDateTime_;
}


//**********************************************************************************************************************
/// \return The creation date of the combo
//**********************************************************************************************************************
QDateTime Combo::creationDateTime() const
{
   return creationDateTime_;
}


//**********************************************************************************************************************
/// \return The group this combo belongs to
/// \return A null pointer if the combo does not belong to any group
//**********************************************************************************************************************
SpGroup Combo::group() const
{
   return group_;
}


//**********************************************************************************************************************
/// \param[in] group The new group this combo belongs to. If null, the combo is removed from its current group
//**********************************************************************************************************************
void Combo::setGroup(SpGroup const& group)
{
   if (group != group_)
   {
      group_ = group;
      this->touch();
   }
}


//**********************************************************************************************************************
/// \param[in] enabled The value for the enabled parameter
//**********************************************************************************************************************
void Combo::setEnabled(bool enabled)
{
   // Note that enabling / disabling an item does not change its last modification date/time
   enabled_ = enabled;
}


//**********************************************************************************************************************
/// \return true if and only if the combo is enabled
//**********************************************************************************************************************
bool Combo::isEnabled() const
{
   return enabled_;
}


//**********************************************************************************************************************
/// \param[in] input The input to check
/// \return true if and only if the input is a match for the combo
//**********************************************************************************************************************
bool Combo::matchesForInput(QString const& input) const
{
   return useLooseMatching_ ? input.endsWith(keyword_) : (input == keyword_);
}


//**********************************************************************************************************************
/// \return true if the substitution was actually performed (it could a been cancelled, for instance by the user
/// dismissing a variable input dialog.
//*********************************************************************************************************************
bool Combo::performSubstitution() const
{
   qint32 cursorLeftShift = -1;
   bool cancelled = false;
   QMap<QString, QString> knownInputVariables;
   QString const& newText = this->evaluatedSnippet(cancelled, QSet<QString>(), knownInputVariables, &cursorLeftShift);
   if (!cancelled)
      performTextSubstitution(keyword_.size(), newText, cursorLeftShift);
   return !cancelled;
}


//**********************************************************************************************************************
/// \param[in] includeGroup Should the group be included in the export
/// \return A JSon object representing this Combo instance
//**********************************************************************************************************************
QJsonObject Combo::toJsonObject(bool includeGroup) const
{
   QJsonObject result;
   result.insert(kPropUuid, uuid_.toString());
   result.insert(kPropName, name_);
   result.insert(kPropKeyword, keyword_);
   result.insert(kPropSnippet, snippet_);
   result.insert(kPropUseLooseMatching, useLooseMatching_);
   result.insert(kPropCreationDateTime, creationDateTime_.toString(constants::kJsonExportDateFormat));
   result.insert(kPropModificationDateTime, modificationDateTime_.toString(constants::kJsonExportDateFormat));
   result.insert(kPropEnabled, enabled_);
   if (includeGroup && group_)
      result.insert(kPropGroup, group_->uuid().toString());
   return result;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void Combo::changeUuid()
{
   uuid_ = QUuid::createUuid();
}


//**********************************************************************************************************************
/// \param[in] name The display name of the combo
/// \param[in] keyword The keyword
/// \param[in] snippet The text that will replace the combo
/// \param[in] useLooseMatching Does the combo use loose matching
/// \param[in] enabled Is the combo enabled
/// \return A shared pointer to the created Combo
//**********************************************************************************************************************
SpCombo Combo::create(QString const& name, QString const& keyword, QString const& snippet, bool useLooseMatching,
   bool enabled)
{
   return std::make_shared<Combo>(name, keyword, snippet, useLooseMatching, enabled);
}


//**********************************************************************************************************************
/// If the JSon object is not a valid combo, the constructed combo will be invalid
///
/// \param[in] object The object to read from
/// \param[in] formatVersion The combo list file format version
/// \param[in] groups The list of combo groups
/// \return A shared pointer to the created Combo
//**********************************************************************************************************************
SpCombo Combo::create(QJsonObject const& object, qint32 formatVersion, GroupList const& groups)
{
   return std::make_shared<Combo>(object, formatVersion, groups);
}


//**********************************************************************************************************************
/// Note that we make a distinction between a copy that can be generated by a copy constructor or assignment operator
/// and a duplicate: a duplicate has a different UUID and as such, even if all other field are strictly identical
/// a duplicate and its original are to be considered as different.
///
/// Also note that the keyword is not copied as two combos cannot have the same keyword.
///
/// \return a shared pointer pointing to a duplicate of the combo
//**********************************************************************************************************************
SpCombo Combo::duplicate(Combo const& combo)
{
   // note that the duplicate is enabled even if the source is not.
   SpCombo result = std::make_shared<Combo>(combo.name(), QString(), combo.snippet(), combo.useLooseMatching(),
      combo.isEnabled());
   result->setGroup(combo.group());
   return result;
}


//**********************************************************************************************************************
/// This function is named after the UNIX touch command.
//**********************************************************************************************************************
void Combo::touch()
{
   modificationDateTime_ = QDateTime::currentDateTime();
}


//**********************************************************************************************************************
/// \param[out] outCancelled Did the user cancel user input
/// \param[in] outCursorPos The final position of the cursor, relative to the beginning of the snippet
/// \param[in] forbiddenSubCombos The text of the combos that are not allowed to be substituted using #{combo:}, to 
/// avoid endless recursion
/// \param[in,out] knownInputVariables The list of know input variables.
/// \return The snippet text once it has been evaluated
//**********************************************************************************************************************
QString Combo::evaluatedSnippet(bool& outCancelled, QSet<QString> forbiddenSubCombos, 
   QMap<QString, QString>& knownInputVariables, qint32* outCursorPos) const
{
   QString remainingText = snippet_;
   QString result;

   // The following regular expression detects the first variable #{}, ensuring the closing } is not preceded by a \.
   // Lazy (a.k.a. non-greedy) operators are used to match the first variable with the smallest possible contents
   // inside the #{}.
   QRegularExpression const regexp(R"((#\{(.*?)(?<!\\)\}))");

   while (true)
   {
      QRegularExpressionMatch match = regexp.match(remainingText);
      if (!match.hasMatch())
         return result + remainingText;

      // we add the text before the variable
      result += remainingText.left(match.capturedStart(1));

      QString const variable = match.captured(2);
      if ("cursor" == variable)
      {
         if (outCursorPos)
            *outCursorPos = result.size();
      }
      else
      {
         // we add the text before the variable and the evaluated variable contents to the result
         result += evaluateVariable(match.captured(2), forbiddenSubCombos, knownInputVariables, outCancelled);
         if (outCancelled)
            return QString();
      }

      // we still need to evaluate the text that was at the right of the variable
      remainingText = remainingText.right(remainingText.size() - match.capturedEnd(1));
   }
}
