/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of functions related to combo variables


#ifndef BEEFTEXT__COMBO__VARIABLE__H
#define BEEFTEXT__COMBO__VARIABLE__H


QString evaluateVariable(QString const& variable, QSet<QString> forbiddenSubCombos = QSet<QString>()); ///< Compute the value of a variable


#endif // #ifndef BEEFTEXT__COMBO__VARIABLE__H


