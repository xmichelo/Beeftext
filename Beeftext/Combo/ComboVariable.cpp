/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of functions related to combo variables


#include "stdafx.h"
#include "ComboVariable.h"
#include "ComboManager.h"
#include "VariableInputDialog.h"


namespace {


QString const kCustomDateTimeVariable = "dateTime:"; ///< The dateTime variable.
QString const kComboVariable = "combo:"; ///< The combo variable.
QString const kInputVariable = "input:"; ///< The input variable.
QString const kEnvVarVariable = "envVar:"; ///< The envVar variable.


//**********************************************************************************************************************
/// \brief Resolve the escaped characters ( \\} and \\\\ in a variable parameter.
/// \param[in] paramStr The variable parameter.
/// \return The parameter where the escaped characters have been resolved.
//**********************************************************************************************************************
QString resolveEscapingInVariableParameter(QString paramStr)
{
   paramStr.replace(R"(\\)", R"(\)");
   paramStr.replace(R"(\})", R"(})");
   return paramStr;
}


//**********************************************************************************************************************
/// \brief Converts a character to a Discord emoji. 
///
/// \note 'a space is added a the end of the returned emoji because Discord sometimes concatenates emoji. For instance
/// to letter emoji corresponding to a country code are concatenated into a flag emoji
///
/// \param[in] c The character
/// \return A string containing the emoji corresponding to the character
/// \return A string containing a series of whitespaces if no matching emoji exists
//**********************************************************************************************************************
QString qcharToDiscordEmoji(QChar const& c)
{
   QChar const l = c.toLower();
   if ((l >= 'a') && (l <= 'z'))
      return QString(":regional_indicator_%1: ").arg(l);
   QMap<QChar, QString> const substs = { { '0', ":zero: " }, { '1', ":one: " }, { '2', ":two: " }, { '3', ":three: " },
   { '4', ":four: " }, { '5', ":five: " }, { '6', ":six: " }, { '7', ":seven: "}, { '8', ":eight: "}, 
   { '9', ":nine: " }, { '!', ":exclamation: "}, { '?', ":question: " } };
   return substs.value(c, "        ");
}


//**********************************************************************************************************************
/// \brief Create a Discord emoji representation of the content of the clipboard
///
/// \return A string containing the sequence of Discord emojis
//**********************************************************************************************************************
QString discordEmojisFromClipboard()
{
   QClipboard const* clipboard = QGuiApplication::clipboard();
   if (!clipboard)
      return QString();
   QString result;
   for (QChar const& c : clipboard->text())
      result += qcharToDiscordEmoji(c);
   return result;
}


//**********************************************************************************************************************
/// \param[in] variable The variable.
/// \return the result of the evaluation.
//**********************************************************************************************************************
QString evaluateDateTimeVariable(QString const& variable)
{
   QString const formatString = resolveEscapingInVariableParameter(variable.right(variable.size()
      - kCustomDateTimeVariable.size()));
   return formatString.isEmpty() ? QString() : QLocale::system().toString(QDateTime::currentDateTime(), formatString);
}


//**********************************************************************************************************************
/// \param[in] variable The variable, without the enclosing #{}.
/// \param[in] forbiddenSubCombos The text of the combos that are not allowed to be substituted using #{combo:}, to 
/// avoid endless recursion.
/// \param[in,out] knownInputVariables The list of know input variables.
/// \param[out] outCancelled Was the input variable cancelled by the user.
/// \return The result of evaluating the variable.
//**********************************************************************************************************************
QString evaluateComboVariable(QString const& variable, QSet<QString> forbiddenSubCombos, 
   QMap<QString, QString>& knownInputVariables, bool& outCancelled)
{
   QString const fallbackResult = QString("#{%1}").arg(variable);
   QString const comboName = resolveEscapingInVariableParameter(variable.right(variable.size() -
      kComboVariable.size()));
   if (forbiddenSubCombos.contains(comboName))
      return fallbackResult;
   ComboList const& combos = ComboManager::instance().comboListRef();
   ComboList::const_iterator const it = std::find_if(combos.begin(), combos.end(),
      [&comboName](SpCombo const& combo) -> bool { return combo->keyword() == comboName; });

   return combos.end() == it ? fallbackResult : (*it)->evaluatedSnippet(outCancelled, 
      forbiddenSubCombos << comboName, knownInputVariables, nullptr);
   // forbiddenSubcombos is intended at avoiding endless recursion
}


//**********************************************************************************************************************
/// \param[in] variable The variable, without the enclosing #{}.
/// \param[in,out] knownInputVariables The list of know input variables.
/// \param[out] outCancelled Was the input variable cancelled by the user.
/// \return The result of evaluating the variable.
//**********************************************************************************************************************
QString evaluateInputVariable(QString const& variable, QMap<QString, QString>& knownInputVariables, bool& outCancelled)
{
   // check if we already add the user input for the given description
   QString const description = variable.right(variable.size() - kInputVariable.size());
   if (knownInputVariables.contains(description))
      return knownInputVariables[description];

   QString result;
   if (!VariableInputDialog::run(resolveEscapingInVariableParameter(description), result))
   {
      outCancelled = true;
      return QString();
   }
   knownInputVariables.insert(description, result); // add the new input to the list of known ones
   return result;

}


//**********************************************************************************************************************
/// \param[in] variable The variable, without the enclosing #{}.
/// \return The result of evaluating the variable.
//**********************************************************************************************************************
QString evaluateEnvVarVariable(QString const& variable)
{
   return QProcessEnvironment::systemEnvironment().value(variable.right(variable.size() -
      kEnvVarVariable.size()));
}


} // anonymous namespace


//**********************************************************************************************************************
/// \param[in] variable The variable, without the enclosing #{}.
/// \param[in] forbiddenSubCombos The text of the combos that are not allowed to be substituted using #{combo:}, to 
/// avoid endless recursion.
/// \param[in,out] knownInputVariables The list of know input variables.
/// \param[out] outCancelled Was the input variable cancelled by the user.
/// \return The result of evaluating the variable.
//**********************************************************************************************************************
QString evaluateVariable(QString const& variable, QSet<QString> forbiddenSubCombos, 
   QMap<QString, QString>& knownInputVariables, bool& outCancelled)
{
   outCancelled = false;
   QLocale const systemLocale = QLocale::system();
   if (variable == "clipboard")
   {
      QClipboard const* clipboard = QGuiApplication::clipboard();
      return clipboard ? clipboard->text() : QString();
   }
   
   if (variable == "discordemoji") //secret variable that create text in Discord emoji from the clipboard text
      return discordEmojisFromClipboard();

   if (variable == "date")
      return systemLocale.toString(QDate::currentDate());

   if (variable == "time")
      return systemLocale.toString(QTime::currentTime());

   if (variable == "dateTime")
      return systemLocale.toString(QDateTime::currentDateTime());

   if (variable.startsWith(kCustomDateTimeVariable))
      return evaluateDateTimeVariable(variable);

   if (variable.startsWith(kComboVariable))
      return evaluateComboVariable(variable, forbiddenSubCombos, knownInputVariables, outCancelled);

   if (variable.startsWith(kInputVariable))
      return evaluateInputVariable(variable, knownInputVariables, outCancelled);

   if (variable.startsWith(kEnvVarVariable))
      return evaluateEnvVarVariable(variable);

   return QString("#{%1}").arg(variable); // we could not recognize the variable, so we put it back in the result
}
