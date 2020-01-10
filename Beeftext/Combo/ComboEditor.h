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

private slots:
   void onEditorContextMenuRequested(QPoint const& pos); ///< Slot for the display of the editor's context menu.
   void onCurrentCharFormatChanged(const QTextCharFormat &format); ///< Slot for change of the edit current character format.
   void onFontChanged(QFont const& font); ///< Slot for the change of the current font.
   void onFontComboChanged(QString const& family); ///< Slot for change of the font family.

private:
   Ui::ComboEditor ui_ {}; ///< The GUI for the widget
};

#endif // BEEFTEXT_COMBO_EDITOR_H
