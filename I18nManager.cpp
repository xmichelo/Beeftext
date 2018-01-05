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


namespace {
   QList<QLocale> const kSupportedLocales = { QLocale::English }; ///< The list of locales supported by the application
}


void removeTranslator(std::unique_ptr<QTranslator>& translator); ///< Remove a translator, free it and set the pointer that was pointing to it to zero


//**********************************************************************************************************************
/// \param[in] translator A pointer to the translator to remove
//**********************************************************************************************************************
void removeTranslator(std::unique_ptr<QTranslator>& translator)
{
   if (!translator)
      return;
   qApp->removeTranslator(translator.get());
   translator.reset();
}


//**********************************************************************************************************************
/// \return The only allowed instance of the class
//**********************************************************************************************************************
I18nManager& I18nManager::instance()
{
   static I18nManager instance;
   return instance;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void I18nManager::loadTranslation()
{
   this->removeAllTranslators();

   QLocale const locale(this->locale());
   QString const langStr(locale.name().left(2));
   if (QLocale::English == locale.language())
      return; // English needs no translation

   // load and install qt translations (containing all translations for Qt internal strings)
   qtTranslator_ = std::make_unique<QTranslator>(qApp);
   QString qtTransFile = QString(":/translations/qtbase_%1.qm").arg(langStr);
   if (!qtTranslator_->load(qtTransFile))
   {
      qtTranslator_.reset();
      QMessageBox::critical(nullptr, "Translation File Missing", "Could not find Qt translation file.");
      return;
   }

   // load and install application translations
   appTranslator_ = std::make_unique<QTranslator>(qApp);
   QString fileName = QString(":/translations/beeftext_%2.qm").arg(langStr);
   if (!appTranslator_->load(fileName))
   {
      appTranslator_.reset();
      QMessageBox::critical(nullptr, "Translation File Missing", "Could not find application translation file.");
      return;
   }

   // load and install xmilib translations
   xmilibTranslator_ = std::make_unique<QTranslator>(qApp);
   if (!xmilibTranslator_->load(QString(":/translations/xmilib_%1.qm").arg(langStr)))
   {
      xmilibTranslator_.reset();
      QMessageBox::critical(nullptr, "Translation File Missing", "Could not find kitrisLib translation file.");
      return;
   }

   qApp->installTranslator(qtTranslator_.get());
   qApp->installTranslator(appTranslator_.get());
   qApp->installTranslator(xmilibTranslator_.get());
}


//**********************************************************************************************************************
/// \return The current locale
//**********************************************************************************************************************
QLocale I18nManager::locale() const
{
   return QLocale();
}


//**********************************************************************************************************************
/// \param[in] locale The new locale
//**********************************************************************************************************************
void I18nManager::setLocale(QLocale const& locale)
{
   if (locale == QLocale())
      return; // already set
   QLocale::setDefault(locale);
   this->loadTranslation();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void I18nManager::removeAllTranslators()
{
   removeTranslator(qtTranslator_);
   removeTranslator(appTranslator_);
   removeTranslator(xmilibTranslator_);
}


