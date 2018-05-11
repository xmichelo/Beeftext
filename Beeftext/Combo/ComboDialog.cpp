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


//**********************************************************************************************************************
/// \param[in] combo The combo
/// \param[in] title The title to display in the dialog's title bar
/// \param[in] parent The parent widget of the dialog
/// \return true if and only if the user validated the dialog
//**********************************************************************************************************************
bool ComboDialog::run(SPCombo& combo, QString const& title, QWidget* parent)
{
   return QDialog::Accepted == ComboDialog(combo, title, parent).exec();
}

//**********************************************************************************************************************
/// \param[in] combo The combo
/// \param[in] title The title to display in the dialog's title bar
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ComboDialog::ComboDialog(SPCombo const& combo, QString const& title, QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , combo_(combo)
   , validator_()
   , snippetEditMenu_(nullptr)
{
   ComboManager::instance().groupListRef().ensureNotEmpty();
   if (!combo)
      throw xmilib::Exception("%1(): combo is null.");
   ui_.setupUi(this);
   this->setWindowTitle(title);
   ui_.editName->setText(combo->name());
   ui_.comboGroup->setContent(ComboManager::instance().groupListRef());
   ui_.comboGroup->setCurrentGroup(combo_->group());
   ui_.editKeyword->setText(combo->keyword());
   ui_.editKeyword->setValidator(&validator_); 
   ui_.editSnippet->setPlainText(combo->snippet());
   this->setupSnipperEditMenu();
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::setupSnipperEditMenu()
{
   connect(ui_.editSnippet, &QPlainTextEdit::customContextMenuRequested,
      this, &ComboDialog::onEditorContextMenuRequested);
   snippetEditMenu_ = ui_.editSnippet->createStandardContextMenu();

   snippetEditMenu_->addSeparator();
   snippetEditMenu_->addMenu(this->createComboVariableMenu());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
QMenu* ComboDialog::createComboVariableMenu()
{
   QMenu* menu = new QMenu(tr("&Insert Variable"), this);

   QAction *action = new QAction(tr("Clip&board Content"), this);
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
   connect(action, &QAction::triggered, [this]() {this->insertTextInSnippetEdit("#{dateTime:}", true); });
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
void ComboDialog::insertTextInSnippetEdit(QString const& text, bool move1CharLeft)
{
   QTextCursor cursor = ui_.editSnippet->textCursor();
   cursor.beginEditBlock();
   cursor.insertText(text);
   if (move1CharLeft)
   {
      cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
      cursor.endEditBlock();
      ui_.editSnippet->setTextCursor(cursor); ///< Required to the cursor position change to take effect
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

   SPGroup const group = ui_.comboGroup->currentGroup();
   if (!group)
   {
      QMessageBox::critical(this, tr("Error"), tr("The group is invalid."));
      return false;
   }

   // we check for conflicts that would make some combo 'unreachable'
   ComboList const& comboList = ComboManager::instance().comboListRef();
   for (SPCombo const& combo : comboList)
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
// 
//**********************************************************************************************************************
void ComboDialog::onActionOk()
{
   Q_ASSERT(combo_);
   if (!checkAndReportInvalidCombo())
      return;
   combo_->setName(ui_.editName->text().trimmed());
   combo_->setGroup(ui_.comboGroup->currentGroup());
   combo_->setKeyword(ui_.editKeyword->text().trimmed());
   combo_->setSnippet(ui_.editSnippet->toPlainText());
   this->accept();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::onActionNewGroup()
{
   SPGroup group = std::make_shared<Group>(QString());
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
   snippetEditMenu_->popup(ui_.editSnippet->viewport()->mapToGlobal(pos));
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


