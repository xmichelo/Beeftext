/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of of combo editor widget.
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.


#ifndef BEEFTEXT_COMBO_EDITOR_H
#define BEEFTEXT_COMBO_EDITOR_H


#include "ui_ComboEditor.h"


//**********************************************************************************************************************
/// \brief Combo editor widget
//**********************************************************************************************************************
class ComboEditor: public QWidget
{
   Q_OBJECT
public: // member functions
   explicit ComboEditor(QWidget* parent = nullptr); ///< Default constructor.
   ComboEditor(ComboEditor const&) = delete; ///< Disabled copy-constructor.
   ComboEditor(ComboEditor&&) = delete; ///< Disabled assignment copy-constructor.
   ~ComboEditor() = default; ///< Destructor.
   ComboEditor& operator=(ComboEditor const&) = delete; ///< Disabled assignment operator.
   ComboEditor& operator=(ComboEditor&&) = delete; ///< Disabled move assignment operator.
   SnippetEdit* snippetEdit() const; ///< Return the snippet edit.
   void setRichTextMode(bool richTextMode) const; ///< Set whether the editor is in rich text mode
   bool isInRichTextMode() const; ///< Check whether the widget is in rich text mode.
   QString plainText() const; ///< Return the plain text content of the editor.
   QString html() const; ///< Return the HTML content of the editor.

private: // data members
   QMenu* createComboVariableMenu(); ///< Create the combo variable menu.
   void insertTextInSnippetEdit(QString const& text, bool move1CharLeft = false) const; ///< Insert some text at the current cursor position in the snippet text edit control.
   void fillFontSizeCombo() const; ///< Fill the font size combo.
   void setColorButton(QColor const& color) const; ///< Setup the color button.
   void applyFormat(QTextCharFormat const& format) const; ///< Apply the format to the current selection.

private slots:
   void onEditorContextMenuRequested(QPoint const& pos); ///< Slot for the display of the editor's context menu.
   void onCurrentCharFormatChanged(const QTextCharFormat &format) const; ///< Slot for change of the edit current character format.
   void onComboFontFamilyChanged(QString const& family) const; ///< Slot for change of the value of the 
   void onComboFontSizeChanged(QString const& sizeText) const; ///< Slot for the change of the value in the font size combo.
   void onButtonColor(); ///< Slot for the color button.
   void onButtonBold(bool checked) const; ///< Slot for the bold button.
   void onButtonItalic(bool checked) const; ///< Slot for the italic button.
   void onButtonUnderline(bool checked) const; ///< Slot for the underline button.
   void onButtonStrikeout(bool checked) const; ///< Slot for the strikeout button.

private:
   Ui::ComboEditor ui_ {}; ///< The GUI for the widget
};

#endif // BEEFTEXT_COMBO_EDITOR_H
