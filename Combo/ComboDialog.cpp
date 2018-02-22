/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of dialog for creating/editing a combo
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
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
bool ComboDialog::run(SPCombo const& combo, QString const& title, QWidget* parent)
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
   , substitutionEditMenu_(nullptr)
{
   if (!combo)
      throw xmilib::Exception("%1(): combo is null.");
   ui_.setupUi(this);
   this->setWindowTitle(title);
   ui_.editName->setText(combo->name());
   ui_.editKeyword->setText(combo->keyword());
   ui_.editKeyword->setValidator(&validator_); 
   ui_.editSubstitution->setPlainText(combo->substitutionText());
   this->setupSubstitutionEditMenu();
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::setupSubstitutionEditMenu()
{
   connect(ui_.editSubstitution, &QPlainTextEdit::customContextMenuRequested,
      this, &ComboDialog::onEditorContextMenuRequested);
   substitutionEditMenu_ = ui_.editSubstitution->createStandardContextMenu();

   substitutionEditMenu_->addSeparator();
   substitutionEditMenu_->addMenu(this->createComboVariableMenu());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
QMenu* ComboDialog::createComboVariableMenu()
{
   QMenu* menu = new QMenu(tr("&Insert Variable"), this);

   QAction *action = new QAction(tr("Clip&board Content"), this);
   connect(action, &QAction::triggered, [&]() { this->insertTextInSubstitutionEdit("#{clipboard}", false); });
   menu->addAction(action);

   QMenu* dtMenu = new QMenu(tr("&Date/Time"));
   action = new QAction(tr("D&ate"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSubstitutionEdit("#{date}", false); });
   dtMenu->addAction(action);
   action = new QAction(tr("&Time"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSubstitutionEdit("#{time}", false); });
   dtMenu->addAction(action);
   action = new QAction(tr("Dat&e && Time"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSubstitutionEdit("#{dateTime}", false); });
   dtMenu->addAction(action);
   action = new QAction(tr("&Custom Date && Time"), this);
   connect(action, &QAction::triggered, [this]() {this->insertTextInSubstitutionEdit("#{dateTime:}", true); });
   dtMenu->addAction(action);
   menu->addMenu(dtMenu);

   action = new QAction(tr("C&ursor Position"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSubstitutionEdit("#{cursor}", false); });
   menu->addAction(action);
   action = new QAction(tr("Co&mbo"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSubstitutionEdit("#{combo:}", true); });
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
void ComboDialog::insertTextInSubstitutionEdit(QString const& text, bool move1CharLeft)
{
   QTextCursor cursor = ui_.editSubstitution->textCursor();
   cursor.beginEditBlock();
   cursor.insertText(text);
   if (move1CharLeft)
   {
      cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
      cursor.endEditBlock();
      ui_.editSubstitution->setTextCursor(cursor); ///< Required to the cursor position change to take effect
   }
   else
      cursor.endEditBlock();
}


//**********************************************************************************************************************
/// \return The combo
//**********************************************************************************************************************
SPCombo ComboDialog::combo() const
{
   return combo_;
}


//**********************************************************************************************************************
/// \return true if and only if the combo is valid
//**********************************************************************************************************************
bool ComboDialog::checkAndReportInvalidCombo()
{
   if (ui_.editSubstitution->toPlainText().isEmpty())
   {
      QMessageBox::critical(this, tr("Error"), tr("The substitution text is empty."));
      return false;
   }
   QString text = ui_.editKeyword->text();
   if (QValidator::Acceptable != validator_.validate(text))
   {
      QMessageBox::critical(this, tr("Error"), tr("The keyword is invalid."));
      return false;
   }

   // we check for conflicts that would make some combo 'unreachable'
   ComboList const& comboList = ComboManager::instance().getComboListRef();
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
   combo_->setKeyword(ui_.editKeyword->text().trimmed());
   combo_->setSubstitutionText(ui_.editSubstitution->toPlainText());
   this->accept();
}


//**********************************************************************************************************************
/// \param[in] pos The position of the cursor
//**********************************************************************************************************************
void ComboDialog::onEditorContextMenuRequested(QPoint const& pos)
{
   substitutionEditMenu_->popup(ui_.editSubstitution->viewport()->mapToGlobal(pos));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::updateGui() const
{
   QString keyword = ui_.editKeyword->text();
   bool const canAccept = (QValidator::Acceptable == validator_.validate(keyword)) &&
      (!ui_.editSubstitution->toPlainText().isEmpty());
   ui_.actionOk->setEnabled(canAccept);
   ui_.buttonOk->setEnabled(canAccept);
}


