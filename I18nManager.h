/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of internationalization manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__I18N__MANAGER__H
#define BEEFTEXT__I18N__MANAGER__H


#include <memory>


//**********************************************************************************************************************
/// \brief Internationalization (i18n) manager class
//**********************************************************************************************************************
class I18nManager
{
public: // member functions
   static I18nManager& instance(); ///< Return the only allowed instance of the class
	~I18nManager() = default; ///< Default destructor
   QLocale locale() const; ///< Get the current locale
   void setLocale(QLocale const& locale); ///< Set the current locale
   void loadTranslation(); ///< Load the translation

private: // member functions
   I18nManager() = default; ///< Default constructor
   I18nManager(I18nManager const&) = delete; ///< Disabled copy constructor
	I18nManager(I18nManager&&) = delete; ///< Disabled move constructor
	I18nManager& operator=(I18nManager const&) = delete; ///< Disabled assignment operator
	I18nManager& operator=(I18nManager&&) = delete; ///< Disabled move assignment operator
   void removeAllTranslators(); ///< Remove and free all translators used by the application

private: // data members
   std::unique_ptr<QTranslator> qtTranslator_; ///< The translator for Qt built-in contents
   std::unique_ptr<QTranslator> appTranslator_; ///< The translator for the application contents
   std::unique_ptr<QTranslator> xmilibTranslator_; ///< The translator the the XMiLib contents
};



#endif // #ifndef BEEFTEXT__I18N__MANAGER__H