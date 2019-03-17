/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of functions related to combo variables


#ifndef BEEFTEXT_COMBO_VARIABLE_H
#define BEEFTEXT_COMBO_VARIABLE_H


QString evaluateVariable(QString const& variable, QSet<QString> forbiddenSubCombos, 
   QMap<QString, QString>& knownInputVariables, bool& outCancelled);


#endif // #ifndef BEEFTEXT_COMBO_VARIABLE_H
