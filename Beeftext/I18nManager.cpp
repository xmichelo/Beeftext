/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of internationalization manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "I18nManager.h"
#include "BeeftextConstants.h"
#include "BeeftextUtils.h"
#include <XMiLib/Exception.h>
#include "BeeftextGlobals.h"


namespace {


QString kStrTranslationFileMissing = "Could not load the following Qt translation file:'%1'"; ///< A string indicating a missing translation file


}


//****************************************************************************************************************************************************
/// \brief Migrate the translation files to the new layout.
///
/// Starting with Beeftext 13, the layout of the translation files has been changed, adding the country code to
/// the language name in order to be able to support multiple variant of the same language. For example, the
/// translation file location at 'de/beeftext_de.qm' should now be 'de_DE/beeftext_de_DE.qm'
//****************************************************************************************************************************************************
void migrateTranslationFilesToNewLayout() {
    QDir const transDir(globals::userTranslationRootFolderPath());
    if (!transDir.exists())
        return;

    QList<QPair<QString, QString>> const mappings = {
        { "de",                   "de_DE" },
        { "de_DE/beeftext_de.qm", "de_DE/beeftext_de_DE.qm" },
        { "de_DE/qtbase_de.pm",   "de_DE/qtbase_de_DE.qm" },
        { "de_DE/xmilib_de.qm",   "de_DE/xmilib_de_DE.qm" },
        { "it",                   "it_IT" },
        { "it_IT/beeftext_it.qm", "it_IT/beeftext_it_IT.qm" },
        { "it_IT/qtbase_it.pm",   "it_IT/qtbase_it_IT.qm" },
        { "it_IT/xmilib_it.qm",   "it_IT/xmilib_it_IT.qm" },
        { "nl",                   "nl_NL" },
        { "nl_NL/beeftext_nl.qm", "nl_NL/beeftext_nl_NL.qm" },
        { "nl_NL/qtbase_nl.pm",   "nl_NL/qtbase_nl_NL.qm" },
        { "nl_NL/xmilib_nl.qm",   "nl_NL/xmilib_nl_NL.qm" },
        { "pl",                   "pl_PL" },
        { "pl_PL/beeftext_pl.qm", "nl_NL/beeftext_pl_PL.qm" },
        { "pl_PL/qtbase_pl.pm",   "nl_NL/qtbase_pl_PL.qm" },
        { "pl_PL/xmilib_pl.qm",   "nl_NL/xmilib_pl_PL.qm" },
        { "pt",                   "pt_PT" },
        { "pt_PT/beeftext_pt.qm", "pt_PT/beeftext_pt_PT.qm" },
        { "pt_PT/qt_pt.pm",       "pt_PT/qtbase_pt_PT.qm" },
        { "pt_PT/xmilib_pt.qm",   "pt_PT/xmilib_pt_PT.qm" },
        { "ru",                   "ru_RU" },
        { "ru_RU/beeftext_ru.qm", "ru_RU/beeftext_ru_RU.qm" },
        { "ru_RU/qtbase_ru.pm",   "ru_RU/qtbase_ru_RU.qm" },
        { "ru_RU/xmilib_ru.qm",   "ru_RU/xmilib_ru_RU.qm" },
        { "zh",                   "zh_TW" },
        { "zh_TW/beeftext_zh.qm", "zh_TW/beeftext_zh_TW.qm" },
        { "zh_TW/qtbase_zh.pm",   "zh_TW/qtbase_zh_TW.qm" },
        { "zh_TW/xmilib_zh.qm",   "zh_TW/xmilib_zh_TW.qm" },
    };

    for (auto const &[src, dst]: mappings)
        QFile::rename(transDir.absoluteFilePath(src), transDir.absoluteFilePath(dst));
}


//****************************************************************************************************************************************************
/// \brief Remove a translator, free it and set the pointer that was pointing to it to zero.
/// \param[in] translator A pointer to the translator to remove
//****************************************************************************************************************************************************
void removeTranslator(std::unique_ptr<QTranslator> &translator) {
    if (!translator)
        return;
    QCoreApplication::removeTranslator(translator.get());
    translator.reset();
}


//****************************************************************************************************************************************************
/// \return The only allowed instance of the class
//****************************************************************************************************************************************************
I18nManager &I18nManager::instance() {
    static I18nManager instance;
    return instance;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
I18nManager::I18nManager() {
    migrateTranslationFilesToNewLayout();
    this->refreshSupportedLocalesList();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void I18nManager::refreshSupportedLocalesList() {
    supportedLocales_ = {{ QLocale::English, QString() }}; ///< English is always supported, as no translation is required

    // There are two places for translation: The first contains the app provided translations
    QDir::Filters constexpr dirFilter = QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable;
    QDir const transRootDir(globals::translationRootFolderPath());
    QFileInfoList transDirs = transRootDir.entryInfoList(dirFilter, QDir::Name);

    // The second contains the user provided translations.
    // Note that in the Portable Edition, this folder is the same as the app provided ones
    QDir const userTransRootDir = globals::userTranslationRootFolderPath();
    if (userTransRootDir != transRootDir)
        transDirs += userTransRootDir.entryInfoList(dirFilter, QDir::Name);

    for (QFileInfo const &info: transDirs) {
        QLocale const locale = QLocale(info.fileName());
        if (QLocale::C == locale.language()) // the locale is unknown
            continue;

        LocaleInfo localeInfo = { locale, info.absoluteFilePath() };
        if (isValidTranslationSubfolder(localeInfo)) {
            if (supportedLocales_.end() != std::find_if(supportedLocales_.begin(), supportedLocales_.end(), [&locale](LocaleInfo const &li) -> bool { return locale == li.locale; }))
                continue; // we already have a translation for this locale

            supportedLocales_.append(localeInfo);
            globals::debugLog().addInfo(QString("A valid translation was found for %1.")
                .arg(QLocale::languageToString(localeInfo.locale.language())));
        } else
            globals::debugLog().addWarning(QString("An invalid or incomplete translation folder '%1' was detected.")
                .arg(info.fileName()));
    }
}


//****************************************************************************************************************************************************
/// \param[in] localeInfo The locale info.
/// \returns true if and only if the translation folder contains a valid translation for the language described by
//****************************************************************************************************************************************************
bool I18nManager::isValidTranslationSubfolder(LocaleInfo const &localeInfo) {
    QDir const dir(localeInfo.folder);
    if (!dir.exists())
        return false;
    QString const localName = localeInfo.locale.name();
    QStringList const files = { QString("beeftext_%1.qm").arg(localName), QString("xmilib_%1.qm").arg(localName) }; ///< qtbase_%1.ts is optional.
    for (QString const &file: files) {
        QFileInfo const fileInfo(dir.absoluteFilePath(file));
        if ((!fileInfo.exists()) || (!fileInfo.isFile()) || (!fileInfo.isReadable()))
            return false;
    }
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] locale The locale to validate
/// \return if locale is supported, it is returned, otherwise the default locale (English) is returned instead
//****************************************************************************************************************************************************
QLocale I18nManager::validateLocale(QLocale const &locale) {
    if (supportedLocales_.isEmpty())
        throw xmilib::Exception("No locale is supported.");

    // first we look for a perfect match (language AND country)
    QList<LocaleInfo>::const_iterator it = std::find_if(supportedLocales_.begin(), supportedLocales_.end(), [&locale](LocaleInfo const &l) -> bool {
        return locale.language() == l.locale.language() && locale.country() == l.locale.country();
    });
    if (it != supportedLocales_.end())
        return (*it).locale;

    // if not found we try to fall back to a locale with the same language, and if not found we default to the first
    // supported language
    it = std::find_if(supportedLocales_.begin(), supportedLocales_.end(), [&locale](LocaleInfo const &l) -> bool { return locale.language() == l.locale.language(); });
    return (it != supportedLocales_.end()) ? (*it).locale : supportedLocales_.front().locale;
}


//****************************************************************************************************************************************************
/// \param[in] combo The combo box
//****************************************************************************************************************************************************
void I18nManager::fillLocaleCombo(QComboBox &combo) {
    QSignalBlocker blocker(&combo);
    combo.clear();
    for (LocaleInfo const &localeInfo: supportedLocales_) {
        QLocale locale = localeInfo.locale;
        QString languageName = localeInfo.locale == QLocale::English ? "English" : locale.nativeLanguageName();
        if (!languageName.isEmpty())
            languageName[0] = languageName[0].toUpper();
        combo.addItem(languageName, localeInfo.locale);
    }
}


//****************************************************************************************************************************************************
/// \param[in] locale The combo
/// \param[in] combo The combo containing the locale
//****************************************************************************************************************************************************
void I18nManager::selectLocaleInCombo(QLocale const &locale, QComboBox &combo) {
    for (qint32 i = 0; i < combo.count(); ++i) {
        QVariant data = combo.itemData(i);
        if (!data.canConvert<QLocale>())
            continue;
        QLocale const itemLocale = data.toLocale();
        if (itemLocale != locale)
            continue;
        if (i == combo.currentIndex())
            return;
        combo.setCurrentIndex(i);
    }
}


//****************************************************************************************************************************************************
/// \param[in] combo The combo box
//****************************************************************************************************************************************************
QLocale I18nManager::getSelectedLocaleInCombo(QComboBox const &combo) {
    return validateLocale(combo.currentData().toLocale());
}


//****************************************************************************************************************************************************
/// \return The current locale
//****************************************************************************************************************************************************
QLocale I18nManager::locale() {
    return QLocale();
}


//****************************************************************************************************************************************************
/// \param[in] locale The new locale
//****************************************************************************************************************************************************
void I18nManager::setLocale(QLocale const &locale) {
    if (locale == QLocale())
        return; // already set
    QLocale::setDefault(locale);
    this->loadTranslation();
    QGuiApplication::setApplicationDisplayName(
        constants::kApplicationName + (isInPortableMode() ? QObject::tr(" - Portable Edition") : ""));
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void I18nManager::loadTranslation() {
    try {
        this->removeAllTranslators();

        QLocale const locale = I18nManager::locale();
        QString const localeName = locale.name();
        if (QLocale::English == locale.language())
            return; // English needs no translation

        // load and install qt translations (containing all translations for Qt internal strings)
        QCoreApplication *app = QCoreApplication::instance();
        qtTranslator_ = std::make_unique<QTranslator>(app);
        QDir const transDir = this->translationFolderForLocale(locale);
        QString qtTransFile = transDir.absoluteFilePath(QString("qtbase_%1.qm").arg(localeName));
        if (!QFileInfo(qtTransFile).exists()) // qtbase translation file is optional
        {
            qtTransFile = qtTransFile = transDir.absoluteFilePath(QString("qt_%1.qm").arg(localeName));
            if (!QFileInfo(qtTransFile).exists())
                qtTransFile = QString();
        }

        if ((!qtTransFile.isEmpty()) && (!qtTranslator_->load(qtTransFile))) {
            qtTranslator_.reset();
            throw xmilib::Exception(kStrTranslationFileMissing.arg(qtTransFile));
        }


        // load and install application translations
        appTranslator_ = std::make_unique<QTranslator>(app);
        QString const btTransFile = transDir.absoluteFilePath(QString("beeftext_%1.qm").arg(localeName));
        if (!appTranslator_->load(btTransFile)) {
            qtTranslator_.reset();
            appTranslator_.reset();
            throw xmilib::Exception(kStrTranslationFileMissing.arg(btTransFile));
        }

        // load and install xmilib translations
        xmilibTranslator_ = std::make_unique<QTranslator>(app);
        QString const xlTransFile = transDir.absoluteFilePath(QString("xmilib_%1.qm").arg(localeName));
        if (!xmilibTranslator_->load(xlTransFile)) {
            qtTranslator_.reset();
            appTranslator_.reset();
            xmilibTranslator_.reset();
            throw xmilib::Exception(kStrTranslationFileMissing.arg(xlTransFile));
        }
    }
    catch (xmilib::Exception const &e) {
        globals::debugLog().addError(e.qwhat());
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Cannot load translation:\n%1")
            .arg(e.qwhat()));
        return;
    }

    QCoreApplication::installTranslator(qtTranslator_.get());
    QCoreApplication::installTranslator(appTranslator_.get());
    QCoreApplication::installTranslator(xmilibTranslator_.get());
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void I18nManager::unloadTranslation() {
    this->removeAllTranslators();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void I18nManager::removeAllTranslators() {
    removeTranslator(qtTranslator_);
    removeTranslator(appTranslator_);
    removeTranslator(xmilibTranslator_);
}


//****************************************************************************************************************************************************
/// \return locale The locale.
//****************************************************************************************************************************************************
QString I18nManager::translationFolderForLocale(QLocale const &locale) {
    QList<LocaleInfo>::const_iterator const it = std::find_if(supportedLocales_.begin(), supportedLocales_.end(), [&locale](LocaleInfo const &localeInfo) -> bool { return localeInfo.locale == locale; });
    return supportedLocales_.end() == it ? QString() : (*it).folder;
}
