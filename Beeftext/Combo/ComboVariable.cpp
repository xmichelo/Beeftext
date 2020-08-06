/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of functions related to combo variables


#include "stdafx.h"
#include "ComboVariable.h"
#include "ComboManager.h"
#include "VariableInputDialog.h"
#include "BeeftextGlobals.h"
#include "Clipboard/ClipboardManagerDefault.h"


namespace {


enum class ECaseChange
{
   NoChange, ///< Do not change case
   ToUpper, ///< Convert to upper case
   ToLower ///< Conver to lower case
}; ///< Enumeration for case change


QString const kCustomDateTimeVariable = "dateTime:"; ///< The dateTime variable.
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
   QString str = globals::clipboardManager().text();
   QString result;
   for (QChar const& c : str)
      result += qcharToDiscordEmoji(c);
   return result;
}


//**********************************************************************************************************************
/// \brief Split a timeshift string (e.g. +1d-4w+11h), into individual shifts (e.g. { +1d, -4w, +11h)
///
/// \param[in] shiftStr The timeshift string
/// \return a string list containing the individual shifts
//**********************************************************************************************************************
QStringList splitTimeShiftString(QString const& shiftStr)
{
   QStringList result;
   QString acc;
   for (QChar const c: shiftStr)
   {
      if (('+' == c) || ('-' == c))
      {
         if (!acc.isEmpty())
         {
            result.append(acc);
            acc = QString();
         }
      }
      acc += c;
   }
   if (!acc.isEmpty())
      result.append(acc);
   return result;
}


//**********************************************************************************************************************
/// \brief Returns the current date shifted according to the instructions in the shift string.
/// \param[in] shiftStr The string describing the timeshift (as defined in the dateTime: variable documentation.
/// \return The current date shifted according to the instructions in the shift string.
//**********************************************************************************************************************
QDateTime shiftedDateTime(QString const& shiftStr)
{
   QDateTime result = QDateTime::currentDateTime();

   QStringList const shifts = splitTimeShiftString(shiftStr);
   for (QString const& shift: shifts)
   {
      QRegularExpressionMatch const match = QRegularExpression(R"(([+-])(\d+)([yMwdhmsz]))").match(shift);
      if (!match.hasMatch())
         continue;
      bool ok = false;
      qint64 value = match.captured(2).toLongLong(&ok);
      if (!ok)
         continue;
      if (match.captured(1) == "-")
         value = -value;
      char const type = match.captured(3)[0].toLatin1();
      switch (type)
      {
      case 'y': result = result.addYears(value); break;
      case 'M': result = result.addMonths(value); break;
      case 'w': result = result.addDays(value * 7); break;
      case 'd': result = result.addDays(value); break;
      case 'h': result = result.addSecs(3600 * value); break;
      case 'm': result = result.addSecs(60 * value); break;
      case 's': result = result.addSecs(value); break;
      case 'z': result = result.addMSecs(value); break;
      default: break;
      }
   }
   return result;
}


//**********************************************************************************************************************
/// \brief Evaluate a #[dateTime:} variable
/// \param[in] variable The variable.
/// \return the result of the evaluation.
//**********************************************************************************************************************
QString evaluateDateTimeVariable(QString const& variable)
{
   QString const formatString = resolveEscapingInVariableParameter(variable.right(variable.size()
      - kCustomDateTimeVariable.size()));

   QRegularExpression const regExp(R"(^dateTime(:(([+-]\d+[yMwdhmsz])+))?:(.*)$)");
   QRegularExpressionMatch const match = regExp.match(variable);
   if (!match.hasMatch())
      return QString();
   QDateTime const dateTime = match.captured(1).isEmpty() ? QDateTime::currentDateTime() :
      shiftedDateTime(match.captured(2));
   QString const formatStr = match.captured(4);
   return formatStr.isEmpty() ? QLocale::system().toString(dateTime) : QLocale::system().toString(dateTime, formatStr);
}


//**********************************************************************************************************************
/// \brief Evaluate a #{combo:} variable.
///
/// \param[in] variable The variable, without the enclosing #{}.
/// \param[in] caseChange The change of case (uppercase, lowercase) to apply to the evaluated variable.
/// \param[in] forbiddenSubCombos The text of the combos that are not allowed to be substituted using #{combo:}, to 
/// avoid endless recursion.
/// \param[in,out] knownInputVariables The list of know input variables.
/// \param[out] outCancelled Was the input variable cancelled by the user.
/// \return The result of evaluating the variable.
//**********************************************************************************************************************
QString evaluateComboVariable(QString const& variable, ECaseChange caseChange, QSet<QString> forbiddenSubCombos, 
   QMap<QString, QString>& knownInputVariables, bool& outIsHtml, bool& outCancelled)
{
   outIsHtml = false;
   QString fallbackResult = QString("#{%1}").arg(variable);
   qint32 const varNameLength = variable.indexOf(':');
   if (varNameLength < 0)
      return QString();
   QString const comboName = resolveEscapingInVariableParameter(variable.right(variable.size() - varNameLength - 1));
   if (forbiddenSubCombos.contains(comboName))
      return fallbackResult;
   ComboList const& combos = ComboManager::instance().comboListRef();
   ComboList::const_iterator const it = std::find_if(combos.begin(), combos.end(),
      [&comboName](SpCombo const& combo) -> bool { return combo->keyword() == comboName; });

   if (combos.end() == it)
      return fallbackResult;
   QString str = (*it)->evaluatedSnippet(outCancelled, forbiddenSubCombos << comboName, knownInputVariables, 
      nullptr); // forbiddenSubcombos is intended at avoiding endless recursion
   outIsHtml = (*it)->useHtml();
   switch (caseChange)
   {
   case ECaseChange::ToUpper: return str.toUpper();
   case ECaseChange::ToLower: return str.toLower();
   case ECaseChange::NoChange: default: return str;
   }
}


//**********************************************************************************************************************
/// \brief Evaluate an #{input:} variable.
///
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
/// \brief Evaluate an #{envvar:} variable.
///
/// \param[in] variable The variable, without the enclosing #{}.
/// \return The result of evaluating the variable.
//**********************************************************************************************************************
QString evaluateEnvVarVariable(QString const& variable)
{
   return QProcessEnvironment::systemEnvironment().value(variable.right(variable.size() -
      kEnvVarVariable.size()));
}


//**********************************************************************************************************************
//  \brief Trim the specified plain or rich text, i.e. erase all non printable characters at the beginning and end
//  of the text.
///
/// \param[in] text The text
/// \param[in] isHtml Is the text in HTML format?
/// \return The trimmed text
//**********************************************************************************************************************
QString trimText(QString const& text, bool isHtml)
{
   if (!isHtml)
      return text.trimmed();

   QTextDocument doc;
   doc.setHtml(text);
   QTextCursor cursor(&doc);

   // trim the beginning
   cursor.movePosition(QTextCursor::Start);
   while ((!cursor.atEnd()) && cursor.selectedText().trimmed().isEmpty())
   {
      cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
   }
   if (!cursor.selectedText().isEmpty())
      cursor.deleteChar();

   // trim the end
   cursor.movePosition(QTextCursor::End);
   while ((!cursor.atStart()) && cursor.selectedText().trimmed().isEmpty())
   {
      cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
   }
   if (!cursor.selectedText().isEmpty())
      cursor.deleteChar();

   return doc.toHtml();
}


} // anonymous namespace



//**********************************************************************************************************************
/// \param[in] variable The variable, without the enclosing #{}.
/// \param[in] forbiddenSubCombos The text of the combos that are not allowed to be substituted using #{combo:}, to 
/// avoid endless recursion.
/// \param[in,out] knownInputVariables The list of know input variables.
/// \param[out] outIsHtml Is the evaluated variable in HTML format?
/// \param[out] outCancelled Was the input variable cancelled by the user.
/// \return The result of evaluating the variable.
//**********************************************************************************************************************
QString evaluateVariable(QString const& variable, QSet<QString> const& forbiddenSubCombos, 
   QMap<QString, QString>& knownInputVariables, bool& outIsHtml, bool& outCancelled)
{
   outIsHtml = false;
   outCancelled = false;
   QLocale const systemLocale = QLocale::system();
   if (variable == "clipboard")
   {
      ClipboardManager& clipboardManager = globals::clipboardManager();
      QString html = clipboardManager.html();
      if (html.isEmpty())
         return clipboardManager.text();
      outIsHtml = true;
      return html;
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

   if (variable.startsWith("combo:"))
      return evaluateComboVariable(variable, ECaseChange::NoChange, forbiddenSubCombos, knownInputVariables, 
         outIsHtml, outCancelled);

   if (variable.startsWith("upper:"))
      return evaluateComboVariable(variable, ECaseChange::ToUpper, forbiddenSubCombos, knownInputVariables, 
         outIsHtml, outCancelled);

   if (variable.startsWith("lower:"))
      return evaluateComboVariable(variable, ECaseChange::ToLower, forbiddenSubCombos, knownInputVariables, 
         outIsHtml, outCancelled);

   if (variable.startsWith("trim:"))
   {
      QString const var = evaluateComboVariable(variable, ECaseChange::NoChange, forbiddenSubCombos, 
         knownInputVariables, outIsHtml, outCancelled);
      return trimText(var, outIsHtml);
   }

   if (variable.startsWith(kInputVariable))
      return evaluateInputVariable(variable, knownInputVariables, outCancelled);

   if (variable.startsWith(kEnvVarVariable))
      return evaluateEnvVarVariable(variable);

   return QString("#{%1}").arg(variable); // we could not recognize the variable, so we put it back in the result
}
