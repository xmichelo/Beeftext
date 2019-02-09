/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of functions related to sensitive applications
/// 
/// Sensitive applications are applications that does not work with the 'standard' Beeftext substitution mechanism
/// based on the clipboard


#ifndef BEEFTEXT_SENSITIVE_APPLICATIONS_H
#define BEEFTEXT_SENSITIVE_APPLICATIONS_H


//**********************************************************************************************************************
/// \brief Sensitive application Manager
//**********************************************************************************************************************
class SensitiveApplicationManager
{
public: // static member functions
   static SensitiveApplicationManager& instance(); ///< Return the only allowed instance of the class

public: // member functions
   SensitiveApplicationManager(SensitiveApplicationManager const&) = delete; ///< Disabled copy-constructor
   SensitiveApplicationManager(SensitiveApplicationManager&&) = delete; ///< Disabled assignment copy-constructor
   ~SensitiveApplicationManager() = default; ///< Destructor
   SensitiveApplicationManager& operator=(SensitiveApplicationManager const&) = delete; ///< Disabled assignment operator
   SensitiveApplicationManager& operator=(SensitiveApplicationManager&&) = delete; ///< Disabled move assignment operator
   bool isSensitiveApplication(QString const& appExeName) const; ///< Check wether a app exe name is the name of a sensitive application
   bool runDialog(QWidget* parent); ///< Run the sensitive application dialog

private: // member functions
   SensitiveApplicationManager(); ///< Default constructor

private: // data members
   QStringList sensitiveApplications_; ///< The list of sensitive applications
};

#endif // #ifndef BEEFTEXT_SENSITIVE_APPLICATIONS_H
