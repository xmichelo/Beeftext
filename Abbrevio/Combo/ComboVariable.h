/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of functions related to combo variables


#ifndef ABBREVIO_COMBO_VARIABLE_H
#define ABBREVIO_COMBO_VARIABLE_H


QString evaluateVariable(QString const &variable, QSet<QString> const &forbiddenSubCombos,
    QMap<QString, QString> &knownInputVariables, bool &outCancelled); ///< Compute the value of a variable.


#endif // #ifndef ABBREVIO_COMBO_VARIABLE_H
