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
/// \parma[in] parent The parent widget of the editor.
//**********************************************************************************************************************
ComboEditor::ComboEditor(QWidget* parent)
   : QWidget(parent)
{
   ui_.setupUi(this);
   ui_.snippetEdit->setAcceptRichText(false);
   connect(ui_.snippetEdit, &QPlainTextEdit::customContextMenuRequested, this, 
      &ComboEditor::onEditorContextMenuRequested);
   ui_.snippetEdit->textCursor().movePosition(QTextCursor::End); // required to ensure the font is detected correctly
   this->fillFontSizeCombo();
}


//**********************************************************************************************************************
/// \return The snippet edit widget
//**********************************************************************************************************************
SnippetEdit* ComboEditor::snippetEdit() const
{
   return ui_.snippetEdit;
}


//**********************************************************************************************************************
/// \param[in] richTextMode Is the control is rich text mode. 
//**********************************************************************************************************************
void ComboEditor::setRichTextMode(bool richTextMode) const
{
   ui_.frameRichTextControls->setVisible(richTextMode);
   if (!richTextMode)
   {
      QString const plainText = QTextDocumentFragment::fromHtml(ui_.snippetEdit->toHtml()).toPlainText();
      ui_.snippetEdit->setAcceptRichText(false);
      ui_.snippetEdit->setPlainText(plainText);
      return;
   }
   ui_.snippetEdit->setAcceptRichText(true);
   this->onCurrentCharFormatChanged(ui_.snippetEdit->currentCharFormat());
}


//**********************************************************************************************************************
/// \return true if and only if the widget is in rich text mode.
//**********************************************************************************************************************
bool ComboEditor::isInRichTextMode() const
{
   return ui_.snippetEdit->acceptRichText();
}


//**********************************************************************************************************************
/// \return The plain text content of the editor
//**********************************************************************************************************************
QString ComboEditor::plainText() const
{
   return ui_.snippetEdit->toPlainText();
}


//**********************************************************************************************************************
/// \return The HTML content of the editor
//**********************************************************************************************************************
QString ComboEditor::html() const
{
   return ui_.snippetEdit->toHtml();
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
//
//**********************************************************************************************************************
void ComboEditor::fillFontSizeCombo() const
{
   ui_.comboFontSize->clear();
   QList<qint32> const stdSizes = QFontDatabase::standardSizes();
   for (qint32 size: stdSizes)
      ui_.comboFontSize->addItem(QString::number(size));
   ui_.comboFontSize->setCurrentIndex(stdSizes.indexOf(QApplication::font().pointSize()));
}


//**********************************************************************************************************************
/// \param[in] format The format.
//**********************************************************************************************************************
void ComboEditor::applyFormat(QTextCharFormat const& format)
{
   QTextCursor cursor = ui_.snippetEdit->textCursor();
   cursor.mergeCharFormat(format);
   ui_.snippetEdit->mergeCurrentCharFormat(format);
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


//**********************************************************************************************************************
/// \param[in] format the new character format
//**********************************************************************************************************************
void ComboEditor::onCurrentCharFormatChanged(const QTextCharFormat& format) const
{
   qDebug() << QString("Current font: %1").arg(format.font().family());
   ui_.comboFontFamily->setCurrentIndex(ui_.comboFontFamily->findText(format.font().family()));
   ui_.comboFontSize->setCurrentIndex(ui_.comboFontSize->findText(QString::number(format.font().pointSize())));
}


//**********************************************************************************************************************
/// \param[in] family The font family.
//**********************************************************************************************************************
void ComboEditor::onComboFontFamilyChanged(QString const& family)
{
   QTextCharFormat format;
   format.setFontFamily(family);
   this->applyFormat(format);
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] sizeText The size, as text.
//**********************************************************************************************************************
void ComboEditor::onComboFontSizeChanged(QString const& sizeText)
{
   bool ok = false;
   float const size = sizeText.toFloat(&ok);
   if ((!ok) || (size <= 0.0f))
      return;
   QTextCharFormat format;
   format.setFontPointSize(size);
   this->applyFormat(format);
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}
