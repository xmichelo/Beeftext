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
#include "InputManager.h"
#include "Snippet/SnippetFragment.h"
#include "Snippet/TextSnippetFragment.h"
#include "Preferences/PreferencesManager.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include "XMiLib/Exception.h"
#include <utility>


using namespace xmilib;


namespace {


QString const kPropUuid = "uuid"; ///< The JSon property name for the UUID
QString const kPropName = "name"; ///< The JSON property name for the name
QString const kPropComboText = "comboText"; ///< The JSON property for the "combo text", deprecated in combo list file format v2, replaced by "keyword"
QString const kPropKeyword = "keyword"; ///< The JSON property for the for the keyword, introduced in the combo list file format v2, replacing "combo text"
QString const kPropSubstitutionText = "substitutionText"; ///< The JSON property name for the substitution text, deprecated in combo list file format v2, replaced by "snippet"
QString const kPropSnippet = "snippet"; ///< The JSON property name for the snippet, introduced in the combo list file format v2, replacing "substitution text"
QString const kPropDescription = "description"; ///< The JSON property name for the description, introduced in the combo list file format v10.
QString const kPropUseLooseMatching = "useLooseMatch"; ///< The JSON property for the 'use loose matching' option. This option is deprecated since combo list file format v9, replaced by matching mode.
QString const kPropMatchingMode = "matchingMode"; ///< The JSON property for the 'matching mode' of the combo. This option was introduced in combo list file format v9.
QString const kPropCaseSensitivity = "caseSensitivity"; ///< The JSON property for the 'case sensitivity' of the combo. This option was introduced in combo list file format v10.
QString const kPropGroup = "group"; ///< The JSON property name for the combo group 
QString const kPropCreated = "created"; ///< The JSON property name for the created date/time, deprecated in combo list file format v3, replaced by "creationDateTime"
QString const kPropCreationDateTime = "creationDateTime"; ///< The JSON property name for the created date/time, introduced in the combo list file format v3, replacing "created"
QString const kPropLastModified = "lastModified"; ///< The JSON property name for the modification date/time, deprecated in combo list file format v3, replaced by "modificationDateTime"
QString const kPropModificationDateTime = "modificationDateTime"; ///< The JSON property name for the modification date/time, introduced in the combo list file format v3, replacing "lastModified"
QString const kPropEnabled = "enabled"; ///< The JSON property name for the enabled/disabled state
QString const kCursorVariable = "#{cursor}"; ///< The cursor position variable.
qint32 const kPlaceholderMaxLength = 50; ///< The maximum length of the placeholder name.
QString const kPlaceholderElision = "..."; ///< The elision text for placeholder (used if placeholder name is too long.


} // anonymous namespace


QString const kPropUseHtml = "useHtml";


//****************************************************************************************************************************************************
/// \param[in] keyword The keyword.
/// \param[in] snippet The snippet.
/// \Return the placeholder name for a combo with the given keyword and snippet.
//****************************************************************************************************************************************************
QString Combo::placeholderName(QString const &keyword, QString const &snippet) {
    QString result = keyword.isEmpty() ? snippet : QString("[%1]: %2").arg(keyword, snippet);
    if (result.length() <= kPlaceholderMaxLength)
        return result;

    return result.left(kPlaceholderMaxLength - kPlaceholderElision.length()) + kPlaceholderElision;
}


//****************************************************************************************************************************************************
/// \param[in] name The display name of the combo
/// \param[in] keyword The keyword
/// \param[in] snippet The text that will replace the combo
/// \param[in] description The description.
/// \param[in] matchingMode The matching mode
/// \param[in] caseSensitivity The case sensitivity.
/// \param[in] enabled Is the combo enabled
//****************************************************************************************************************************************************
Combo::Combo(QString name, QString keyword, QString snippet, QString description, EMatchingMode matchingMode,
    ECaseSensitivity caseSensitivity, bool const enabled)
    : uuid_(QUuid::createUuid())
    , name_(std::move(name))
    , keyword_(std::move(keyword))
    , snippet_(std::move(snippet))
    , description_(std::move(description))
    , matchingMode_(matchingMode)
    , caseSensitivity_(caseSensitivity)
    , enabled_(enabled) {
    modificationDateTime_ = creationDateTime_ = QDateTime::currentDateTime();
}


//****************************************************************************************************************************************************
/// If the JSon object is not a valid combo, the constructed combo will be invalid
/// \param[in] object The object to read from
/// \param[in] formatVersion The version number of the combo list file format
/// \param[in] groups The list of combo groups
//****************************************************************************************************************************************************
Combo::Combo(QJsonObject const &object, qint32 formatVersion, GroupList const &groups)
    : uuid_(QUuid(object[kPropUuid].toString()))
    , name_(object[kPropName].toString())
    , keyword_(object[formatVersion >= 2 ? kPropKeyword : kPropComboText].toString())
    , snippet_(object[formatVersion >= 2 ? kPropSnippet : kPropSubstitutionText].toString())
    , description_((formatVersion < 10) ? QString() : object[kPropDescription].toString())
    , caseSensitivity_((formatVersion < 10) ? ECaseSensitivity::Default :
                       intToCaseSensitivity(object[kPropCaseSensitivity].toInt(0), ECaseSensitivity::Default))
    , creationDateTime_(QDateTime::fromString(object[formatVersion >= 3 ? kPropCreationDateTime :
                                                     kPropCreated].toString(), constants::kJsonExportDateFormat))
    , modificationDateTime_(QDateTime::fromString(object[formatVersion >= 3 ? kPropModificationDateTime :
                                                         kPropLastModified].toString(), constants::kJsonExportDateFormat))
    , enabled_(object[kPropEnabled].toBool(true)) {
    if (object.contains(kPropUseHtml) && object[kPropUseHtml].toBool(false))
        snippet_ = htmlToPlainText(snippet_);

    if (object.contains(kPropGroup)) {
        QUuid const uuid(object[kPropGroup].toString());
        GroupList::const_iterator const it = groups.findByUuid(uuid);
        if (it != groups.end())
            group_ = *it;
        else
            globals::debugLog().addWarning("While parsing combo file, a combo with an invalid group was found.");
    }

    if (formatVersion < 9) {
        if (object.contains(kPropUseLooseMatching))
            matchingMode_ = object[kPropUseLooseMatching].toBool(false) ? EMatchingMode::Loose : EMatchingMode::Default;
    } else
        matchingMode_ = static_cast<EMatchingMode>(qBound<qint32>(0, object[kPropMatchingMode].toInt(
            qint32(EMatchingMode::Strict)), static_cast<qint32>(EMatchingMode::Count) - 1));

    // because we parse an older format version, we update the modification date, as the combo manager will save
    // the file to update it to the latest format
    if (formatVersion < ComboList::fileFormatVersionNumber)
        this->touch();
}


//****************************************************************************************************************************************************
/// \return true if and only if the combo is valid
//****************************************************************************************************************************************************
bool Combo::isValid() const {
    return (!creationDateTime_.isNull()) && (!modificationDateTime_.isNull()) && (!uuid_.isNull())
           && (!snippet_.isEmpty());
}


//****************************************************************************************************************************************************
/// \return The UUID of the combo
//****************************************************************************************************************************************************
QUuid Combo::uuid() const {
    return uuid_;
}


//****************************************************************************************************************************************************
/// \return The display name of the combo
//****************************************************************************************************************************************************
QString Combo::name() const {
    return name_;
}


//****************************************************************************************************************************************************
/// \return The placeholder name.
//****************************************************************************************************************************************************
QString Combo::placeholderName() const {
    return Combo::placeholderName(keyword_, snippet_);
}


//****************************************************************************************************************************************************
/// If the name is empty,
//****************************************************************************************************************************************************
QString Combo::displayName() const {
    return name_.isEmpty() ? this->placeholderName() : name_;
}


//****************************************************************************************************************************************************
/// \param[in] name The display name of the combo
//****************************************************************************************************************************************************
void Combo::setName(QString const &name) {
    if (name_ != name) {
        name_ = name;
        this->touch();
    }
}


//****************************************************************************************************************************************************
/// \return The keyword
//****************************************************************************************************************************************************
QString Combo::keyword() const {
    return keyword_;
}


//****************************************************************************************************************************************************
/// \param[in] keyword The keyword
//****************************************************************************************************************************************************
void Combo::setKeyword(QString const &keyword) {
    if (keyword_ != keyword) {
        keyword_ = keyword;
        this->touch();
    }
}


//****************************************************************************************************************************************************
/// \return The snippet
//****************************************************************************************************************************************************
QString Combo::snippet() const {
    return snippet_;
}


//****************************************************************************************************************************************************
/// \param[in] snippet The snippet
//****************************************************************************************************************************************************
void Combo::setSnippet(QString const &snippet) {
    if (snippet_ != snippet) {
        snippet_ = snippet;
        this->touch();
    }
}


//****************************************************************************************************************************************************
/// \return The description.
//****************************************************************************************************************************************************
QString Combo::description() const {
    return description_;
}


//****************************************************************************************************************************************************
/// \param[in] description The description.
//****************************************************************************************************************************************************
void Combo::setDescription(QString const &description) {
    if (description != description_) {
        description_ = description;
        this->touch();
    }
}


//****************************************************************************************************************************************************
/// \param[in] resolveDefault If the value is default, should the function return the actual default matching mode for
/// the application.
/// \return The matching mode of the combo.
//****************************************************************************************************************************************************
EMatchingMode Combo::matchingMode(bool resolveDefault) const {
    if ((EMatchingMode::Default == matchingMode_) && resolveDefault)
        return PreferencesManager::instance().defaultMatchingMode();
    return matchingMode_;
}


//****************************************************************************************************************************************************
/// \param[in] mode The matching mode.
//****************************************************************************************************************************************************
void Combo::setMatchingMode(EMatchingMode mode) {
    if (matchingMode_ != mode) {
        matchingMode_ = mode;
        this->touch();
    }
}


//****************************************************************************************************************************************************
/// \param[in] resolveDefault If the value is default, should the function return the actual default case sensitivity
/// for the application.
/// \return The case sensitivity of the combo.
/// //****************************************************************************************************************************************************
ECaseSensitivity Combo::caseSensitivity(bool resolveDefault) const {
    if ((ECaseSensitivity::Default == caseSensitivity_) && resolveDefault)
        return PreferencesManager::instance().defaultCaseSensitivity();
    return caseSensitivity_;
}


//****************************************************************************************************************************************************
/// \param[in] caseSensitivity The case sensitivity.
//****************************************************************************************************************************************************
void Combo::setCaseSensitivity(ECaseSensitivity caseSensitivity) {
    if (caseSensitivity_ != caseSensitivity) {
        caseSensitivity_ = caseSensitivity;
        this->touch();
    }
}


//****************************************************************************************************************************************************
/// \return The last modification date of the combo
//****************************************************************************************************************************************************
QDateTime Combo::modificationDateTime() const {
    return modificationDateTime_;
}


//****************************************************************************************************************************************************
/// \return The creation date of the combo
//****************************************************************************************************************************************************
QDateTime Combo::creationDateTime() const {
    return creationDateTime_;
}


//****************************************************************************************************************************************************
/// \param[in] dateTime The date/time
//****************************************************************************************************************************************************
void Combo::setLastUseDateTime(QDateTime const &dateTime) {
    lastUseDateTime_ = dateTime;
}


//****************************************************************************************************************************************************
/// \return The last use date/time of the combo.
//****************************************************************************************************************************************************
QDateTime Combo::lastUseDateTime() const {
    return lastUseDateTime_;
}


//****************************************************************************************************************************************************
/// \return The group this combo belongs to
/// \return A null pointer if the combo does not belong to any group
//****************************************************************************************************************************************************
SpGroup Combo::group() const {
    return group_;
}


//****************************************************************************************************************************************************
/// \param[in] group The new group this combo belongs to. If null, the combo is removed from its current group
//****************************************************************************************************************************************************
void Combo::setGroup(SpGroup const &group) {
    if (group != group_) {
        group_ = group;
        this->touch();
    }
}


//****************************************************************************************************************************************************
/// \param[in] enabled The value for the enabled parameter
//****************************************************************************************************************************************************
void Combo::setEnabled(bool enabled) {
    // Note that enabling / disabling an item does not change its last modification date/time
    enabled_ = enabled;
}


//****************************************************************************************************************************************************
/// \return true if and only if the combo is enabled
//****************************************************************************************************************************************************
bool Combo::isEnabled() const {
    return enabled_;
}


//****************************************************************************************************************************************************
/// \return true if and only if the combo is usable, i.e. if it is enabled, and member of a group that is enabled (or
/// not member of a group).
//****************************************************************************************************************************************************
bool Combo::isUsable() const {
    return enabled_ && ((!group()) || group_->enabled());
}


//****************************************************************************************************************************************************
/// \param[in] input The input to check
/// \return true if and only if the input is a match for the combo
//****************************************************************************************************************************************************
bool Combo::matchesForInput(QString const &input) const {
    if (keyword_.isEmpty())
        return false;
    Qt::CaseSensitivity const cs = (this->caseSensitivity(true) == ECaseSensitivity::CaseInsensitive)
                                   ? Qt::CaseInsensitive : Qt::CaseSensitive;
    return (this->matchingMode(true) == EMatchingMode::Loose) ? input.endsWith(keyword_, cs) :
           (input.compare(keyword_, cs) == 0);
}


//****************************************************************************************************************************************************
/// \brief compute the number of time the cursor must be shifted to the left to reach the position of the cursor
/// variable.
//****************************************************************************************************************************************************
qint32 computeCursorLeftShift(QString const &str) {
    QString s = str;
    s.remove(QRegularExpression(QString(R"((%1)|(%2))").arg(constants::kDelayVariableRegExpStr, constants::kKeyVariableRegExpStr)));
    qsizetype const index = s.lastIndexOf(kCursorVariable);
    if (index < 0)
        return -1;
    return printableCharacterCount(s.right(s.length() - (index + kCursorVariable.length())));
}


//****************************************************************************************************************************************************
/// \param[in] triggeredByPicker Was the substitution triggered by the picker window
/// \return true if the substitution was actually performed (it could a been cancelled, for instance by the user
/// dismissing a variable input dialog.
//****************************************************************************************************************************************************
bool Combo::performSubstitution(bool triggeredByPicker) {
    bool cancelled = false;
    QMap<QString, QString> knownInputVariables;
    QSet<QString> const forbiddenSubcombos;
    QString newText = this->evaluatedSnippet(cancelled, forbiddenSubcombos, knownInputVariables);
    if (cancelled)
        return false;
    if (!knownInputVariables.isEmpty()) {
        // we displayed the input variable dialog at least once. Some slow/heavy application (Electron-based stuff like
        // Slack & al. for instance) will need some time to properly regain focus.
        qApp->thread()->msleep(300);
    }

    qint32 const cursorLShift = computeCursorLeftShift(newText);
    if (cursorLShift >= 0)
        newText = newText.remove(kCursorVariable, Qt::CaseInsensitive);

    InputManager &inputManager = InputManager::instance();
    PreferencesManager const &prefs = PreferencesManager::instance();
    bool const wasKeyboardHookEnabled = inputManager.setKeyboardHookEnabled(false);
    // we disable the hook to prevent endless recursive substitution

    try {
        // we erase the combo
        bool const triggersOnSpace = prefs.useAutomaticSubstitution() && prefs.comboTriggersOnSpace();

        if (!triggeredByPicker)
            eraseChars(qMax<qint32>(qint32(keyword_.size()) + (triggersOnSpace ? 1 : 0), 0));

        // we split the snippets into fragments and render them
        ListSpSnippetFragment fragments = splitStringIntoSnippetFragments(newText);
        if ((!triggeredByPicker) && (triggersOnSpace && prefs.keepFinalSpaceCharacter()))
            fragments.push_back(std::make_shared<TextSnippetFragment>(QString(" ")));
        renderSnippetFragmentList(fragments);

        // Position the cursor if needed by typing the right amount of left keystrokes.
        if (cursorLShift > 0)
            moveCursorLeft(cursorLShift);
    }
    catch (Exception const &) {
        inputManager.setKeyboardHookEnabled(wasKeyboardHookEnabled);
        throw;
    }
    inputManager.setKeyboardHookEnabled(wasKeyboardHookEnabled);

    lastUseDateTime_ = QDateTime::currentDateTime();
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] includeGroup Should the group be included in the export
/// \return A JSon object representing this Combo instance
//****************************************************************************************************************************************************
QJsonObject Combo::toJsonObject(bool includeGroup) const {
    QJsonObject result;
    result.insert(kPropUuid, uuid_.toString());
    result.insert(kPropName, name_);
    result.insert(kPropKeyword, keyword_);
    result.insert(kPropSnippet, snippet_);
    result.insert(kPropDescription, description_);
    result.insert(kPropMatchingMode, qint32(matchingMode_));
    result.insert(kPropCaseSensitivity, caseSensitivityToInt(caseSensitivity_));
    result.insert(kPropCreationDateTime, creationDateTime_.toString(constants::kJsonExportDateFormat));
    result.insert(kPropModificationDateTime, modificationDateTime_.toString(constants::kJsonExportDateFormat));
    result.insert(kPropEnabled, enabled_);
    if (includeGroup && group_)
        result.insert(kPropGroup, group_->uuid().toString());
    return result;
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void Combo::changeUuid() {
    uuid_ = QUuid::createUuid();
}


//****************************************************************************************************************************************************
/// \param[in] name The display name of the combo.
/// \param[in] keyword The keyword.
/// \param[in] snippet The text that will replace the combo.
/// \param[in] description The description.
/// \param[in] matchingMode The matching mode.
/// \param[in] caseSensitivity The case sensitivity.
/// \param[in] enabled Is the combo enabled.
/// \return A shared pointer to the created Combo.
//****************************************************************************************************************************************************
SpCombo Combo::create(QString const &name, QString const &keyword, QString const &snippet,
    QString const &description, EMatchingMode matchingMode,
    ECaseSensitivity caseSensitivity, bool enabled) {
    return std::make_shared<Combo>(name, keyword, snippet, description, matchingMode, caseSensitivity, enabled);
}


//****************************************************************************************************************************************************
/// If the JSon object is not a valid combo, the constructed combo will be invalid
///
/// \param[in] object The object to read from
/// \param[in] formatVersion The combo list file format version
/// \param[in] groups The list of combo groups
/// \return A shared pointer to the created Combo
//****************************************************************************************************************************************************
SpCombo Combo::create(QJsonObject const &object, qint32 formatVersion, GroupList const &groups) {
    return std::make_shared<Combo>(object, formatVersion, groups);
}


//****************************************************************************************************************************************************
/// Note that we make a distinction between a copy that can be generated by a copy constructor or assignment operator
/// and a duplicate: a duplicate has a different UUID and as such, even if all other field are strictly identical
/// a duplicate and its original are to be considered as different.
///
/// Also note that the keyword is not copied as two combos cannot have the same keyword.
///
/// \return a shared pointer pointing to a duplicate of the combo
//****************************************************************************************************************************************************
SpCombo Combo::duplicate(Combo const &combo) {
    // note that the duplicate is enabled even if the source is not.
    SpCombo result = std::make_shared<Combo>(combo.name(), QString(), combo.snippet(), combo.description(), combo.matchingMode(false), combo.caseSensitivity(false), combo.isEnabled());
    result->setGroup(combo.group());
    return result;
}


//****************************************************************************************************************************************************
/// This function is named after the UNIX touch command.
//****************************************************************************************************************************************************
void Combo::touch() {
    modificationDateTime_ = QDateTime::currentDateTime();
}


//****************************************************************************************************************************************************
///  This function does not process the #{cursor} variable.
///
/// \param[out] outCancelled Did the user cancel user input
/// \param[in] forbiddenSubCombos The text of the combos that are not allowed to be substituted using #{combo:}, to 
/// avoid endless recursion
/// \param[in,out] knownInputVariables The list of know input variables.
/// \return The snippet text once it has been evaluated
//****************************************************************************************************************************************************
QString Combo::evaluatedSnippet(bool &outCancelled, QSet<QString> const &forbiddenSubCombos,
    QMap<QString, QString> &knownInputVariables) const {
    outCancelled = false;
    QString remainingText = snippet_;
    QString result;

    while (true) {
        QRegularExpressionMatch match = constants::kVariableRegExp.match(remainingText);
        if (!match.hasMatch())
            return result += remainingText;

        QString variable = match.captured(1);
        qint32 const pos = qint32(match.capturedStart(0));
        if (pos > 0)
            result += remainingText.left(pos);
        remainingText = remainingText.right(remainingText.size() - pos - match.capturedLength(0));

        variable.replace("\\}", "}");
        result += evaluateVariable(variable, forbiddenSubCombos, knownInputVariables, outCancelled);

        if (outCancelled)
            return QString();
    }
}


//****************************************************************************************************************************************************
/// \param[out] outCancelled Did the user cancel user input
/// \return The snippet text once it has been evaluated
//****************************************************************************************************************************************************
QString Combo::evaluatedSnippet(bool &outCancelled) const {
    QSet<QString> const forbiddenSubCombos;
    QMap<QString, QString> knownInputVariables;
    return evaluatedSnippet(outCancelled, forbiddenSubCombos, knownInputVariables);
}
