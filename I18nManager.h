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
public: // static functions
   static I18nManager& instance(); ///< Return the only allowed instance of the class
   static QLocale validateLocale(QLocale const& locale); ///< Validate the specified locale
   static void fillLocaleCombo(QComboBox& combo); ///< Fill a combo box with the available locale
   static void selectLocaleInCombo(QLocale const& locale, QComboBox& combo); ///< Select a locale in a locale combo
   static QLocale getSelectedLocaleInCombo(QComboBox const& combo); ///< Return the currently selected locale in a locale combo

public: // member functions
   I18nManager(I18nManager const&) = delete; ///< Disabled copy constructor
   I18nManager(I18nManager&&) = delete; ///< Disabled move constructor
	~I18nManager() = default; ///< Default destructor
   I18nManager& operator=(I18nManager const&) = delete; ///< Disabled assignment operator
   I18nManager& operator=(I18nManager&&) = delete; ///< Disabled move assignment operator
   static QLocale locale(); ///< Get the current locale
   void setLocale(QLocale const& locale); ///< Set the current locale
   void loadTranslation(); ///< Load the translation
   void unloadTranslation(); ///< Unload the translation

private: // member functions
   I18nManager() = default; ///< Default constructor
   void removeAllTranslators(); ///< Remove and free all translators used by the application

private: // data members
   std::unique_ptr<QTranslator> qtTranslator_; ///< The translator for Qt built-in contents
   std::unique_ptr<QTranslator> appTranslator_; ///< The translator for the application contents
   std::unique_ptr<QTranslator> xmilibTranslator_; ///< The translator the the XMiLib contents
};



#endif // #ifndef BEEFTEXT__I18N__MANAGER__H