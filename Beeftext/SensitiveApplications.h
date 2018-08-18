/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of functions related to sensitive applications
/// 
/// Sensitive applications are applications that does not work with the 'standard' Beeftext substitution mechanism
/// based on the clipboard


#ifndef BEEFTEXT_SENSITIVE_APPLICATIONS_H
#define BEEFTEXT_SENSITIVE_APPLICATIONS_H


QStringList sensitiveApplications(); ///< Retrieve the list of sensitive applications executable file names
bool isSensitiveApplication(QString const& appExeName); ///< Check wether a app exe name is the name of a sensitive application


#endif // #ifndef BEEFTEXT_SENSITIVE_APPLICATIONS_H
