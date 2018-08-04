/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of dialog for creating/editing a combo
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "../Group/GroupDialog.h"
#include "ComboDialog.h"
#include "ComboManager.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>
#include "PreferencesManager.h"


namespace {


//**********************************************************************************************************************
/// \param[in] keyword The keyword.
/// \param[in] parent The parent widget of the dialog.
/// \return true if and only if the user decided to proceed with the short keyword.
//**********************************************************************************************************************
bool showShortKeywordConfirmationDialog(QString const& keyword, QWidget* parent = nullptr)
{
   QMessageBox msgBox(parent);
   msgBox.setText(QObject::tr("The keyword is very short. Are you sure you want to use the keyword '%1'?")
      .arg(keyword));
   msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   msgBox.setDefaultButton(QMessageBox::No);
   msgBox.setEscapeButton(QMessageBox::No);
   QCheckBox* check = new QCheckBox(QObject::tr("Do not show this warning again."), &msgBox);
   msgBox.setCheckBox(check);
   qint32 const button = msgBox.exec();
   PreferencesManager::instance().setWarnAboutShortComboKeywords(!check->isChecked());
   return (QMessageBox::Yes == button);
}


}


//**********************************************************************************************************************
/// \param[in] combo The combo
/// \param[in] title The title to display in the dialog's title bar
/// \param[in] parent The parent widget of the dialog
/// \return true if and only if the user validated the dialog
//**********************************************************************************************************************
bool ComboDialog::run(SpCombo& combo, QString const& title, QWidget* parent)
{
   return Accepted == ComboDialog(combo, title, parent).exec();
}


//**********************************************************************************************************************
/// \param[in] combo The combo
/// \param[in] title The title to display in the dialog's title bar
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ComboDialog::ComboDialog(SpCombo const& combo, QString const& title, QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , combo_(combo)
{
   ComboManager::instance().groupListRef().ensureNotEmpty();
   if (!combo)
      throw xmilib::Exception("%1(): combo is null.");
   ui_.setupUi(this);
   this->setWindowTitle(title);
   ui_.editName->setText(combo->name());
   ui_.comboGroup->setContent(ComboManager::instance().groupListRef());
   ui_.comboGroup->setCurrentGroup(combo_->group());
   this->setMatchingComboValue(combo->useLooseMatching());
   ui_.editKeyword->setText(combo->keyword());
   ui_.editKeyword->setValidator(&validator_);
   ui_.editSnippet->setPlainText(combo->snippet());
   connect(ui_.editSnippet, &QPlainTextEdit::customContextMenuRequested, this, 
      &ComboDialog::onEditorContextMenuRequested);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
QMenu* ComboDialog::createComboVariableMenu()
{
   QMenu* menu = new QMenu(tr("&Insert Variable"), this);

   QAction* action = new QAction(tr("Clip&board Content"), this);
   connect(action, &QAction::triggered, [&]() { this->insertTextInSnippetEdit("#{clipboard}", false); });
   menu->addAction(action);

   QMenu* dtMenu = new QMenu(tr("&Date/Time"));
   action = new QAction(tr("D&ate"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{date}", false); });
   dtMenu->addAction(action);
   action = new QAction(tr("&Time"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{time}", false); });
   dtMenu->addAction(action);
   action = new QAction(tr("Dat&e && Time"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{dateTime}", false); });
   dtMenu->addAction(action);
   action = new QAction(tr("&Custom Date && Time"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{dateTime:}", true); });
   dtMenu->addAction(action);
   menu->addMenu(dtMenu);

   action = new QAction(tr("C&ursor Position"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{cursor}", false); });
   menu->addAction(action);
   action = new QAction(tr("Co&mbo"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{combo:}", true); });
   menu->addAction(action);
   menu->addSeparator();
   action = new QAction(tr("&About Variables"), this);
   connect(action, &QAction::triggered, []() { QDesktopServices::openUrl(constants::kBeeftextWikiVariablesUrl); });
   menu->addAction(action);
   return menu;
}


//**********************************************************************************************************************
/// \param[in] text The text to insert
/// \param[in] move1CharLeft Should the cursor be moved by one character to the left after insertion
//**********************************************************************************************************************
void ComboDialog::insertTextInSnippetEdit(QString const& text, bool move1CharLeft) const
{
   QTextCursor cursor = ui_.editSnippet->textCursor();
   cursor.beginEditBlock();
   cursor.insertText(text);
   if (move1CharLeft)
   {
      cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
      cursor.endEditBlock();
      ui_.editSnippet->setTextCursor(cursor); ///< Required for the cursor position change to take effect
   }
   else
      cursor.endEditBlock();
}


//**********************************************************************************************************************
/// \return true if and only if the combo is valid
//**********************************************************************************************************************
bool ComboDialog::checkAndReportInvalidCombo()
{
   if (ui_.editSnippet->toPlainText().isEmpty())
   {
      QMessageBox::critical(this, tr("Error"), tr("The snippet text is empty."));
      return false;
   }
   QString text = ui_.editKeyword->text();
   if (QValidator::Acceptable != validator_.validate(text))
   {
      QMessageBox::critical(this, tr("Error"), tr("The keyword is invalid."));
      return false;
   }

   SpGroup const group = ui_.comboGroup->currentGroup();
   if (!group)
   {
      QMessageBox::critical(this, tr("Error"), tr("The group is invalid."));
      return false;
   }

   // we check for conflicts that would make some combo 'unreachable'
   ComboList const& comboList = ComboManager::instance().comboListRef();
   for (SpCombo const& combo: comboList)
   {
      QString const newCombo = ui_.editKeyword->text();
      QString const existing = combo->keyword();
      if ((combo != combo_) && (existing.startsWith(newCombo) || newCombo.startsWith(existing)))
      {
         QMessageBox::critical(this, tr("Error"), tr("An existing combo, triggered by '%1', is creating a conflict "
            "with this combo.").arg(combo->keyword()));
         return false;
      }
   }
   return true;
}


//**********************************************************************************************************************
/// \param[in] useLooseMatching true if the combo uses loose matching
//**********************************************************************************************************************
void ComboDialog::setMatchingComboValue(bool const useLooseMatching) const
{
   ui_.comboMatching->setCurrentIndex(useLooseMatching ? 1 : 0);
}


//**********************************************************************************************************************
/// \return true if the 'Loose' item is selected
//**********************************************************************************************************************
bool ComboDialog::getMatchingComboValue() const
{
   return 1 == ui_.comboMatching->currentIndex();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::onActionOk()
{
   Q_ASSERT(combo_);
   if (!checkAndReportInvalidCombo())
      return;
   QString const keyword = ui_.editKeyword->text().trimmed();
   if (PreferencesManager::instance().warnAboutShortComboKeywords() && (keyword.size() < 3) 
      && (!showShortKeywordConfirmationDialog(keyword, this)))
      return;
   combo_->setName(ui_.editName->text().trimmed());
   combo_->setGroup(ui_.comboGroup->currentGroup());
   combo_->setUseLooseMatching(this->getMatchingComboValue());
   combo_->setKeyword(keyword);
   combo_->setSnippet(ui_.editSnippet->toPlainText());
   this->accept();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::onActionNewGroup()
{
   SpGroup group = std::make_shared<Group>(QString());
   if ((!GroupDialog::run(group, tr("New Group"), this)) || !group)
      return;
   ComboManager::instance().comboListRef().groupListRef().append(group);
   ui_.comboGroup->setContent(ComboManager::instance().groupListRef());
   ui_.comboGroup->setCurrentGroup(group);
}


//**********************************************************************************************************************
/// \param[in] pos The position of the cursor
//**********************************************************************************************************************
void ComboDialog::onEditorContextMenuRequested(QPoint const& pos)
{
   QScopedPointer<QMenu, QScopedPointerDeleteLater> menu(ui_.editSnippet->createStandardContextMenu(pos));
   menu->addSeparator();
   menu->addMenu(this->createComboVariableMenu());         
   menu->exec(ui_.editSnippet->viewport()->mapToGlobal(pos));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::updateGui() const
{
   QString keyword = ui_.editKeyword->text();
   bool const canAccept = (QValidator::Acceptable == validator_.validate(keyword)) &&
      (!ui_.editSnippet->toPlainText().isEmpty()) && ui_.comboGroup->currentGroup();
   ui_.buttonOk->setEnabled(canAccept);
}


