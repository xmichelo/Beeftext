/// \file
/// \author 
///
/// \brief Implementation of theme related functions.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "Theme.h"


//**********************************************************************************************************************
/// \param[in] theme The theme.
/// \return The display name of the theme.
//**********************************************************************************************************************
QString themeName(ETheme theme)
{
   switch (theme)
   {
   case ETheme::Light: return "light";
   case ETheme::Dark: return "dark";
   case ETheme::Count: 
   default: return "unknown";
   }
}


//**********************************************************************************************************************
/// \param[in] combo The combo box.
//**********************************************************************************************************************
void fillThemeComboBox(QComboBox& combo)
{
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
