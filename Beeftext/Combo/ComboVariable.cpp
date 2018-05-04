/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of functions related to combo variables


#include "stdafx.h"
#include "ComboVariable.h"
#include "ComboManager.h"


QString resolveEscapingInVariableParameter(QString paramStr); ///< Resolve the escaped characters ( \\} and \\\\ in a variable parameter


//**********************************************************************************************************************
/// \param[in] paramStr The variable parameter
/// \return The parameter where the escaped characters have been resolved
//**********************************************************************************************************************
QString resolveEscapingInVariableParameter(QString paramStr)
{
   paramStr.replace(R"(\\)", R"(\)");
   paramStr.replace(R"(\})", R"(})");
   return paramStr;
}


//**********************************************************************************************************************
/// \param[in] variable The variable, without the enclosing #{}
/// \param[in] forbiddenSubCombos The text of the combos that are not allowed to be substituted using #{combo:}, to 
/// avoid endless recursion
/// \return The result of evaluating the variable
//**********************************************************************************************************************
QString evaluateVariable(QString const& variable, QSet<QString> forbiddenSubCombos)
{
   QString const fallbackResult = QString("#{%1}").arg(variable);
   QLocale const systemLocale = QLocale::system();
   if (variable == "clipboard")
   {
      QClipboard const* clipboard = qApp->clipboard();
      return clipboard ? clipboard->text() : QString();
   }

   if (variable == "date")
      return systemLocale.toString(QDate::currentDate());

   if (variable == "time")
      return systemLocale.toString(QTime::currentTime());

   if (variable == "dateTime")
      return systemLocale.toString(QDateTime::currentDateTime());

   QString const kCustomDateTimeVariable = "dateTime:";
   if (variable.startsWith(kCustomDateTimeVariable))
   {
      QString const formatString = resolveEscapingInVariableParameter(variable.right(variable.size() 
         - kCustomDateTimeVariable.size()));
      return formatString.isEmpty() ? QString() : systemLocale.toString(QDateTime::currentDateTime(), formatString);
   }

   QString const kComboVariable = "combo:";
   if (variable.startsWith(kComboVariable))
   {
      QString const comboName = resolveEscapingInVariableParameter(variable.right(variable.size() - 
         kComboVariable.size()));
      if (forbiddenSubCombos.contains(comboName))
         return fallbackResult;
      ComboList const& combos = ComboManager::instance().comboListRef();
      ComboList::const_iterator const it = std::find_if(combos.begin(), combos.end(), 
         [&comboName](SPCombo const& combo) -> bool { return combo->keyword() == comboName; });
      
      return combos.end() == it ? fallbackResult 
          : (*it)->evaluatedSnippet(nullptr, forbiddenSubCombos << comboName);// forbiddenSubcombos is intended at avoiding endless recursion
   }

   return fallbackResult ; // we could not recognize the variable, so we put it back in the result
}

