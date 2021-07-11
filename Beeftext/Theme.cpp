/// \file
/// \author 
///
/// \brief Implementation of theme related functions.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "Theme.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


//**********************************************************************************************************************
/// \param[in] theme The theme.
/// \return The display name of the theme.
//**********************************************************************************************************************
QString themeName(ETheme theme)
{
   switch (theme)
   {
   case ETheme::Light: return QObject::tr("light");
   case ETheme::Dark: return QObject::tr("dark");
   case ETheme::Count: default: return QObject::tr("unknown");
   }
}


//**********************************************************************************************************************
/// \param[in] combo The combo box.
//**********************************************************************************************************************
void fillThemeComboBox(QComboBox& combo)
{
   QSignalBlocker blocker(&combo);
   combo.clear();
   for (qint32 i = 0; i < static_cast<qint32>(ETheme::Count); ++i)
   {
      QString name = themeName(static_cast<ETheme>(i));
      if (name.length() > 0)
         name[0] = name[0].toUpper();
      combo.addItem(name, i);
   }
}


//**********************************************************************************************************************
/// \param[in] theme The theme.
/// \param[in] combo The combo box.
//**********************************************************************************************************************
void selectThemeInCombo(ETheme theme, QComboBox& combo)
{
   for (qint32 i = 0; i < combo.count(); ++i)
   {
      bool ok = false;
      ETheme const itemTheme = static_cast<ETheme>(combo.itemData(i).toInt(&ok));
      if (!ok)
         continue;
      if (itemTheme == theme)
         combo.setCurrentIndex(i);
   }
}


//**********************************************************************************************************************
/// \param[in] combo The combo box.
/// \return The currently selected item in a combo box.
//**********************************************************************************************************************
ETheme selectedThemeInCombo(QComboBox const& combo)
{
   qint32 index = qMax<qint32>(0, combo.currentIndex());
   if (index >= static_cast<qint32>(ETheme::Count))
      index = 0;
   return static_cast<ETheme>(index);
}


//**********************************************************************************************************************
/// \param[in] useCustomTheme Does the application use a custom theme
/// \param[in] theme The theme.
//**********************************************************************************************************************
void applyThemePreferences(bool useCustomTheme, ETheme theme)
{
   try
   {
      if (!useCustomTheme)
      {
         qApp->setStyleSheet(QString());
         return;
      }

      // Load the common part of the stylesheet.
      QString const resourcePath;
      QFile commonFile(":/MainWindow/Resources/StyleCommon.qss");
      if (!commonFile.open(QIODevice::ReadOnly | QIODevice::Text))
         throw xmilib::Exception(QString("Could not load stylesheet from resource '%1'").arg(resourcePath));
      QString stylesheet = QString::fromUtf8(commonFile.readAll()) + "\n";
      commonFile.close();

      // Add the theme-dependent part of the style sheet.
      QString const themeResourcePath = QString(":/MainWindow/Resources/Style%1.qss").arg((ETheme::Dark == theme)
         ? "Dark" : "Light");
      QFile themeFile(themeResourcePath);
      if (!themeFile.open(QIODevice::ReadOnly | QIODevice::Text))
         throw xmilib::Exception(QString("Could not load stylesheet from resource '%1'").arg(resourcePath));
      stylesheet += QString::fromUtf8(themeFile.readAll());

      qApp->setStyleSheet(stylesheet);

   }
   catch (xmilib::Exception const& e)
   {
      QString const& msg = e.qwhat();
      if (!msg.isEmpty())
         globals::debugLog().addWarning(msg);
      qApp->setStyleSheet(QString());
   }

}

