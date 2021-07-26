/// \file
/// \author 
///
/// \brief Implementation of the appearance preference pane
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PrefsPaneAppearance.h"
#include "I18nManager.h"
#include "BeeftextGlobals.h"


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the frame.
//**********************************************************************************************************************
PrefsPaneAppearance::PrefsPaneAppearance(QWidget* parent)
   : PrefsPane(parent)
   , prefs_(PreferencesManager::instance())
{
   ui_.setupUi(this);
   I18nManager::instance().fillLocaleCombo(*ui_.comboLocale);
   fillThemeComboBox(*ui_.comboTheme);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneAppearance::load() const
{
   QSignalBlocker blocker = QSignalBlocker(ui_.comboLocale);
   this->onRefreshLanguageList();
   blocker = QSignalBlocker(ui_.checkUseCustomTheme);
   ui_.checkUseCustomTheme->setChecked(prefs_.useCustomTheme());
   blocker = QSignalBlocker(ui_.comboTheme);
   selectThemeInCombo(prefs_.theme(), *ui_.comboTheme);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneAppearance::onRefreshLanguageList() const
{
   I18nManager& i18NManager = I18nManager::instance();
   QLocale const currentLocale = I18nManager::locale();
   i18NManager.refreshSupportedLocalesList();
   i18NManager.fillLocaleCombo(*ui_.comboLocale);
   QLocale const validatedLocale = i18NManager.validateLocale(currentLocale);
   I18nManager::selectLocaleInCombo(validatedLocale, *ui_.comboLocale);
   i18NManager.setLocale(validatedLocale);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneAppearance::onComboLanguageValueChanged(int) const
{
   prefs_.setLocale(I18nManager::instance().getSelectedLocaleInCombo(*ui_.comboLocale));
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneAppearance::onOpenTranslationFolder()
{
   QString const path = globals::userTranslationRootFolderPath();
   if (QDir(path).exists())
      QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


//**********************************************************************************************************************
/// \param[in] checked Is the radio button checked?
//**********************************************************************************************************************
void PrefsPaneAppearance::onCheckUseCustomTheme(bool checked) const
{
   prefs_.setUseCustomTheme(checked);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneAppearance::onComboThemeValueChanged(int) const
{
   prefs_.setTheme(selectedThemeInCombo(*ui_.comboTheme));
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneAppearance::updateGui() const
{
   ui_.comboTheme->setEnabled(prefs_.useCustomTheme());
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void PrefsPaneAppearance::changeEvent(QEvent* event)
{
   if (QEvent::LanguageChange == event->type())
   {
      ui_.retranslateUi(this);
      QSignalBlocker blocker(ui_.comboTheme);
      fillThemeComboBox(*ui_.comboTheme);
      selectThemeInCombo(prefs_.theme(), *ui_.comboTheme);
   }
   PrefsPane::changeEvent(event);
}
