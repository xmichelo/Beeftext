/// \file
/// \author 
///
/// \brief Declaration of theme related functions.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_THEME_H
#define BEEFTEXT_THEME_H


//****************************************************************************************************************************************************
/// \brief An enumeration for the theme.
//****************************************************************************************************************************************************
enum class ETheme {
    Light = 0, ///< Light theme.
    Dark = 1, ///< Dark theme.
    Count = 2 /// The number of themes.
};


QString themeName(ETheme theme); ///< Get the display name of a theme.
void fillThemeComboBox(QComboBox &combo); ///< Fill a combo box with the list of themes.
void selectThemeInCombo(ETheme theme, QComboBox &combo); ///< Select a theme in a theme combo box.
ETheme selectedThemeInCombo(QComboBox const &combo); ///< Get the currently selected theme in a combo box.
void applyThemePreferences(bool useCustomTheme, ETheme theme); ///< Apply the theme preferences.


#endif // #ifndef BEEFTEXT_THEME_H
