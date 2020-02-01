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
   this->onCurrentCharFormatChanged(ui_.snippetEdit->currentCharFormat());
   this->onCursorPositionChanged();
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
/// \param[in] color The color.
//**********************************************************************************************************************
void ComboEditor::setColorButton(QColor const& color) const
{
   QPixmap pix(32, 32);
   pix.fill(color);
   ui_.buttonColor->setIcon(pix);
}


//**********************************************************************************************************************
/// \param[in] format The format.
//**********************************************************************************************************************
void ComboEditor::applyFormat(QTextCharFormat const& format) const
{
   QTextCursor cursor = ui_.snippetEdit->textCursor();
   cursor.mergeCharFormat(format);
   ui_.snippetEdit->mergeCurrentCharFormat(format);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboEditor::updateAlignmentButtonsState() const
{
   Qt::Alignment const alignment = ui_.snippetEdit->alignment();
   {
      QSignalBlocker blocker(ui_.buttonAlignLeft);
      ui_.buttonAlignLeft->setChecked(alignment & Qt::AlignLeft);
   }
   {
      QSignalBlocker blocker(ui_.buttonAlignCenter);
      ui_.buttonAlignCenter->setChecked(alignment & Qt::AlignCenter);
   }
   {
      QSignalBlocker blocker(ui_.buttonAlignRight);
      ui_.buttonAlignRight->setChecked(alignment & Qt::AlignRight);
   }
   {
      QSignalBlocker blocker(ui_.buttonAlignJustify);
      ui_.buttonAlignJustify->setChecked(alignment & Qt::AlignJustify);
   }
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
   ui_.comboFontFamily->setCurrentIndex(ui_.comboFontFamily->findText(format.font().family()));
   ui_.comboFontSize->setCurrentIndex(ui_.comboFontSize->findText(QString::number(format.font().pointSize())));
   this->setColorButton(format.foreground().color());
   {
      QSignalBlocker blocker(ui_.buttonBold);
      ui_.buttonBold->setChecked(format.fontWeight() > QFont::Normal);
   }
   {
      QSignalBlocker blocker(ui_.buttonItalic);
      ui_.buttonItalic->setChecked(format.fontItalic());
   }
   {
      QSignalBlocker blocker(ui_.buttonUnderline);
      ui_.buttonUnderline->setChecked(format.fontUnderline());
   }
   {
      QSignalBlocker blocker(ui_.buttonStrikeout);
      ui_.buttonStrikeout->setChecked(format.fontStrikeOut());
   }
   {
      QSignalBlocker blocker(ui_.buttonHyperlink);
      ui_.buttonHyperlink->setChecked(!format.anchorHref().isEmpty());
   }
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboEditor::onCursorPositionChanged() const
{
   this->updateAlignmentButtonsState();
}


//**********************************************************************************************************************
/// \param[in] family The font family.
//**********************************************************************************************************************
void ComboEditor::onComboFontFamilyChanged(QString const& family) const
{
   QTextCharFormat format;
   format.setFontFamily(family);
   this->applyFormat(format);
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] sizeText The size, as text.
//**********************************************************************************************************************
void ComboEditor::onComboFontSizeChanged(QString const& sizeText) const
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


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboEditor::onButtonColor()
{
   QColor const color = QColorDialog::getColor(ui_.snippetEdit->textColor(), this, tr("Text color"));
   if (!color.isValid())
      return;
   this->setColorButton(color);
   QTextCharFormat format;
   format.setForeground(color);
   this->applyFormat(format);
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] checked Is the button checked?
//**********************************************************************************************************************
void ComboEditor::onButtonBold(bool checked) const
{
   QTextCharFormat format;
   format.setFontWeight(checked ? QFont::Bold : QFont::Normal);
   this->applyFormat(format);
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] checked Is the button checked?
//**********************************************************************************************************************
void ComboEditor::onButtonItalic(bool checked) const
{
   QTextCharFormat format;
   format.setFontItalic(checked);
   this->applyFormat(format);
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] checked Is the button checked?
//**********************************************************************************************************************
void ComboEditor::onButtonUnderline(bool checked) const
{
   QTextCharFormat format;
   format.setFontUnderline(checked);
   this->applyFormat(format);
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] checked Is the button checked?
//**********************************************************************************************************************
void ComboEditor::onButtonStrikeout(bool checked) const
{
   QTextCharFormat format;
   format.setFontStrikeOut(checked);
   this->applyFormat(format);
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] checked Is the button checked?
//**********************************************************************************************************************
void ComboEditor::onButtonAlignLeft(bool checked) const
{
   ui_.snippetEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
   this->updateAlignmentButtonsState();
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] checked Is the button checked?
//**********************************************************************************************************************
void ComboEditor::onButtonAlignCenter(bool checked) const
{
   ui_.snippetEdit->setAlignment(Qt::AlignCenter);
   this->updateAlignmentButtonsState();
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] checked Is the button checked?
//**********************************************************************************************************************
void ComboEditor::onButtonAlignRight(bool checked) const
{
   ui_.snippetEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
   this->updateAlignmentButtonsState();
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] checked Is the button checked?
//**********************************************************************************************************************
void ComboEditor::onButtonAlignJustify(bool checked) const
{
   ui_.snippetEdit->setAlignment(Qt::AlignJustify);
   this->updateAlignmentButtonsState();
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
}


//**********************************************************************************************************************
/// \param[in] checked Is the button checked?
//**********************************************************************************************************************
void ComboEditor::onButtonHyperlink(bool checked)
{
   if (checked)
   {
      bool ok = false;
      QString const url = QInputDialog::getText(this, tr("Enter URL"), tr("Hyperlink URL"), QLineEdit::Normal, 
         QString(), &ok);
      if (!ok)
      {
         QSignalBlocker blocker(ui_.buttonHyperlink);
         ui_.buttonHyperlink->setChecked(false);
         ui_.snippetEdit->setFocus(Qt::NoFocusReason);
         return;
      }
      QTextCharFormat format;
      format.setAnchor(true);
      format.setAnchorHref(url);
      format.setFontUnderline(true);
      format.setForeground(QColor(0, 0, 255));
      this->applyFormat(format);
      ui_.snippetEdit->setFocus(Qt::NoFocusReason);
      return;
   }
   QTextCharFormat format;
   format.setAnchor(false);
   format.setAnchorHref(QString());
   format.setFontUnderline(false);
   format.setForeground(Qt::black);
   this->applyFormat(format);
   ui_.snippetEdit->setFocus(Qt::NoFocusReason);
   return;

}

