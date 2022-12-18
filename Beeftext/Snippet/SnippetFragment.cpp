/// \file
/// \author 
///
/// \brief Implementation of snippet fragment class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "SnippetFragment.h"
#include "TextSnippetFragment.h"
#include "ShortcutSnippetFragment.h"
#include "DelaySnippetFragment.h"
#include "KeySnippetFragment.h"
#include "BeeftextConstants.h"


namespace {


qint32 constexpr kDelayBetweenFragmentsMs = 100; ///< THe delay between fragments in milliseconds


}


//****************************************************************************************************************************************************
/// \brief split a string into snippet fragment at the #{key:} variable.
/// \param[in] str The string.
/// \return the text split into fragments
//****************************************************************************************************************************************************
ListSpSnippetFragment splitForKeyVariable(QString const &str) {
    ListSpSnippetFragment result;
    QString s(str);
    QRegularExpression const rx(QString(R"((.*)%1(.*))").arg(constants::kKeyVariableRegExpStr), QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match;
    while ((match = rx.match(s)).hasMatch()) {
        QString const after = match.captured(4);
        if (!after.isEmpty())
            result.prepend(std::make_shared<TextSnippetFragment>(after));
        bool ok = true;
        QString const repeatStr = match.captured(3);
        qint32 repeatCount = 1;
        if (!repeatStr.isEmpty())
            repeatCount = repeatStr.toInt(&ok);
        if (ok)
            result.prepend(std::make_shared<KeySnippetFragment>(match.captured(2), repeatCount));
        s = match.captured(1);
    }
    if (!s.isEmpty())
        result.prepend(std::make_shared<TextSnippetFragment>(s));
    return result;
}


//****************************************************************************************************************************************************
/// \brief split a string into snippet fragment at the #{shortcut:} variable.
/// \param[in] str The string.
/// \return the text split into fragments
//****************************************************************************************************************************************************
ListSpSnippetFragment splitForShortcutVariable(QString const &str) {
    ListSpSnippetFragment result;
    QString s(str);
    QRegularExpression const rx(QString(R"((.*)%1(.*))").arg(constants::kShortcutVariableRegExpStr), QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match;
    while ((match = rx.match(s)).hasMatch()) {
        QString const after = match.captured(3);
        if (!after.isEmpty())
            result = splitForKeyVariable(after) + result;

        SpShortcut const shortcut = Shortcut::fromString(match.captured(2));
        if (shortcut)
            result.prepend(std::make_shared<ShortcutSnippetFragment>(shortcut));
        s = match.captured(1);
    }
    if (!s.isEmpty())
        result = splitForKeyVariable(s) + result;
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] str The string to split.
/// \return the list of fragments
//****************************************************************************************************************************************************
ListSpSnippetFragment splitStringIntoSnippetFragments(QString const &str) {
    ListSpSnippetFragment result;
    QString s(str);
    QRegularExpression const rx(QString(R"((.*)%1(.*))").arg(constants::kDelayVariableRegExpStr), QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match;
    while ((match = rx.match(s)).hasMatch()) {
        QString const after = match.captured(3);
        if (!after.isEmpty())
            result = splitForShortcutVariable(after) + result;
        bool ok = false;
        qint32 const delay = match.captured(2).toInt(&ok);
        if (ok && (delay > 0))
            result.prepend(std::make_shared<DelaySnippetFragment>(delay));
        s = match.captured(1);
    }
    if (!s.isEmpty())
        result = splitForShortcutVariable(s) + result;
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] fragments The list of snippet fragments.
///
/// \note this function does not disable the keyboard hook before operating.
//****************************************************************************************************************************************************
void renderSnippetFragmentList(ListSpSnippetFragment const &fragments) {
    qsizetype const count = fragments.size();
    for (qsizetype i = 0; i < fragments.size(); ++i) {
        SpSnippetFragment const &fragment = fragments[i];
        if (!fragment)
            continue;
        fragment->render();
        if (i != count - 1)
            QThread::msleep(kDelayBetweenFragmentsMs);
    }
}


