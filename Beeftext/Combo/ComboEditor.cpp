/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo editor widget.
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.


#include "stdafx.h"
#include "ComboEditor.h"
#include "BeeftextConstants.h"


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the editor.
//**********************************************************************************************************************
ComboEditor::ComboEditor(QWidget* parent)
   : QWidget(parent)
{
   ui_.setupUi(this);
   connect(ui_.snippetEdit, &QPlainTextEdit::customContextMenuRequested, this, 
      &ComboEditor::onEditorContextMenuRequested);
}


//**********************************************************************************************************************
/// \return The snippet edit widget
//**********************************************************************************************************************
QPlainTextEdit* ComboEditor::plainTextEdit() const
{
   return ui_.snippetEdit;
}


//**********************************************************************************************************************
/// \return The content of the edit
//**********************************************************************************************************************
QString ComboEditor::plainText() const
{
   return ui_.snippetEdit->toPlainText();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboEditor::setPlainText(QString const& text) const
{
   ui_.snippetEdit->setPlainText(text);
}


//**********************************************************************************************************************
/// \return The menu.
//**********************************************************************************************************************
QMenu* ComboEditor::createComboVariableMenu()
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

   QMenu* comboMenu = new QMenu(tr("Co&mbo"), this);
   action = new QAction(tr("Co&mbo"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{combo:}", true); });
   comboMenu->addAction(action);
   action = new QAction(tr("Combo (&uppercase)"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{upper:}", true); });
   comboMenu->addAction(action);
   action = new QAction(tr("Combo (&lowercase)"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{lower:}", true); });
   comboMenu->addAction(action);
   menu->addMenu(comboMenu);
   action = new QAction(tr("Combo (&trimmed)"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{trim:}", true); });
   comboMenu->addAction(action);
   menu->addMenu(comboMenu);

   action = new QAction(tr("En&vironment Variable"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{envVar:}", true); });
   menu->addAction(action);
   action = new QAction(tr("User &Input"), this);
   connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{input:}", true); });
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
void ComboEditor::insertTextInSnippetEdit(QString const& text, bool move1CharLeft) const
{
   QTextCursor cursor = ui_.snippetEdit->textCursor();
   cursor.beginEditBlock();
   cursor.insertText(text);
   if (move1CharLeft)
   {
      cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
      cursor.endEditBlock();
      ui_.snippetEdit->setTextCursor(cursor); ///< Required for the cursor position change to take effect
   }
   else
      cursor.endEditBlock();
}


//**********************************************************************************************************************
/// \param[in] pos The cursos position.
//**********************************************************************************************************************
void ComboEditor::onEditorContextMenuRequested(QPoint const& pos)
{
   QScopedPointer<QMenu, QScopedPointerDeleteLater> menu(ui_.snippetEdit->createStandardContextMenu(pos));
   menu->addSeparator();
   menu->addMenu(this->createComboVariableMenu());         
   menu->exec(ui_.snippetEdit->viewport()->mapToGlobal(pos));
}


