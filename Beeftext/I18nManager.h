/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of internationalization manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


// ReSharper disable CppInconsistentNaming
#ifndef BEEFTEXT_I18N_MANAGER_H
#define BEEFTEXT_I18N_MANAGER_H
// ReSharper restore CppInconsistentNaming


#include <memory>


//****************************************************************************************************************************************************
/// \brief Internationalization (i18n) manager class
//****************************************************************************************************************************************************
// ReSharper disable once CppInconsistentNaming
class I18nManager {
public: // static functions
    static I18nManager &instance(); ///< Return the only allowed instance of the class
    void refreshSupportedLocalesList(); ///< Build the list of supported locales
    QLocale validateLocale(QLocale const &locale); ///< Validate the specified locale
    void fillLocaleCombo(QComboBox &combo); ///< Fill a combo box with the available locale
    static void selectLocaleInCombo(QLocale const &locale, QComboBox &combo); ///< Select a locale in a locale combo
    QLocale getSelectedLocaleInCombo(QComboBox const &combo); ///< Return the currently selected locale in a locale combo
    static QLocale locale(); ///< Get the current locale

public: // member functions
    I18nManager(I18nManager const &) = delete; ///< Disabled copy constructor
    I18nManager(I18nManager &&) = delete; ///< Disabled move constructor
    ~I18nManager() = default; ///< Default destructor
    I18nManager &operator=(I18nManager const &) = delete; ///< Disabled assignment operator
    I18nManager &operator=(I18nManager &&) = delete; ///< Disabled move assignment operator
    void setLocale(QLocale const &locale); ///< Set the current locale
    void loadTranslation(); ///< Load the translation
    void unloadTranslation(); ///< Unload the translation

private: // data types
    struct LocaleInfo {
        QLocale locale;
        QString folder;
    }; ///< Type definition for locale information

private: // member functions
    I18nManager(); ///< Default constructor
    static bool isValidTranslationSubfolder(LocaleInfo const &localeInfo); ///< Check whether a translation subfolder contains a valid translation
    void removeAllTranslators(); ///< Remove and free all translators used by the application
    QString translationFolderForLocale(QLocale const &locale); ///< Return the path of the translation folder for the given locale

private: // data members
    QList<LocaleInfo> supportedLocales_; ///< The list of supported locales
    std::unique_ptr<QTranslator> qtTranslator_; ///< The translator for Qt built-in contents
    std::unique_ptr<QTranslator> appTranslator_; ///< The translator for the application contents
    std::unique_ptr<QTranslator> xmilibTranslator_; ///< The translator the the XMiLib contents
};


#endif // #ifndef BEEFTEXT_I18N_MANAGER_H