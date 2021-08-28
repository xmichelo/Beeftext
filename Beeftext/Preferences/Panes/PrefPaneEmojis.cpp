/// \file
/// \author 
///
/// \brief Implementation of preference pane for emojis
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PrefPaneEmojis.h"
#include "Emoji/EmojiManager.h"


//**********************************************************************************************************************
/// \param[in] parent the parent widget
//**********************************************************************************************************************
PrefPaneEmojis::PrefPaneEmojis(QWidget* parent)
   : PrefPane(parent)
   , prefs_(PreferencesManager::instance())
{
   ui_.setupUi(this);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefPaneEmojis::load() const
{
   QSignalBlocker blocker(ui_.checkEnableEmoji);
   ui_.checkEnableEmoji->setChecked(prefs_.emojiShortcodesEnabled());
   blocker = QSignalBlocker(ui_.editEmojiLeftDelimiter);
   ui_.editEmojiLeftDelimiter->setText(prefs_.emojiLeftDelimiter());
   blocker = QSignalBlocker(ui_.editEmojiRightDelimiter);
   ui_.editEmojiRightDelimiter->setText(prefs_.emojiRightDelimiter());
   blocker = QSignalBlocker(ui_.checkShowEmojiInPickerWindow);
   ui_.checkShowEmojiInPickerWindow->setChecked(prefs_.showEmojisInPickerWindow());
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefPaneEmojis::updateGui() const
{
   QWidgetList widgets = { ui_.buttonEmojiExcludedApps, ui_.labelEmojiLeftDelimiter, ui_.editEmojiLeftDelimiter,
   ui_.labelEmojiRightDelimiter, ui_.editEmojiRightDelimiter, ui_.checkShowEmojiInPickerWindow };
   for (QWidget* const widget : widgets)
      widget->setEnabled(ui_.checkEnableEmoji->isChecked());
}


//**********************************************************************************************************************
/// \param[in] checked The state of the check box.
//**********************************************************************************************************************
void PrefPaneEmojis::onCheckEnableEmojiShortcodes(bool checked) const
{
   prefs_.setEmojiShortcodeEnabled(checked);
   EmojiManager& emojis = EmojiManager::instance();
   if (checked)
      emojis.loadEmojis();
   else
      emojis.unloadEmojis();
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefPaneEmojis::onEditEmojiExcludedApplications()
{
   EmojiManager::instance().runDialog(this);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefPaneEmojis::onEmojiLeftDelimiterChanged(QString const&) const
{
   prefs_.setEmojiLeftDelimiter(ui_.editEmojiLeftDelimiter->text());
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefPaneEmojis::onEmojiRightDelimiterChanged(QString const&) const
{
   prefs_.setEmojiRightDelimiter(ui_.editEmojiRightDelimiter->text());
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked.
//**********************************************************************************************************************
void PrefPaneEmojis::onCheckShowEmojisInPickerWindow(bool checked) const
{
   prefs_.setShowEmojisInPickerWindow(checked);
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void PrefPaneEmojis::changeEvent(QEvent* event)
{
   if (QEvent::LanguageChange == event->type())
      ui_.retranslateUi(this);
   PrefPane::changeEvent(event);
}
