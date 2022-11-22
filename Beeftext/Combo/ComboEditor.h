/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of of combo editor widget.
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.


#ifndef BEEFTEXT_COMBO_EDITOR_H
#define BEEFTEXT_COMBO_EDITOR_H


//****************************************************************************************************************************************************
/// \brief Combo editor widget
//****************************************************************************************************************************************************
class ComboEditor : public QPlainTextEdit {
Q_OBJECT
public: // member functions
    explicit ComboEditor(QWidget *parent = nullptr); ///< Default constructor.
    ComboEditor(ComboEditor const &) = delete; ///< Disabled copy-constructor.
    ComboEditor(ComboEditor &&) = delete; ///< Disabled assignment copy-constructor.
    ~ComboEditor() override = default; ///< Destructor.
    ComboEditor &operator=(ComboEditor const &) = delete; ///< Disabled assignment operator.
    ComboEditor &operator=(ComboEditor &&) = delete; ///< Disabled move assignment operator.
    QString plainText() const; ///< Return the plain text content of the edit.

private: // data members
    QMenu *createComboVariableMenu(); ///< Create the combo variable menu.
    void insertTextInSnippetEdit(QString const &text, bool move1CharLeft = false); ///< Insert some text at the current cursor position in the snippet text edit control.

private slots:
    void onEditorContextMenuRequested(QPoint const &pos); ///< Slot for the display of the editor's context menu.
    void insertPowershellVariable(); ///< Prompt for a script file path and insert a powershell variable at the current cursor position.
    void insertShortcutVariable(); ///< Insert a shortcut variable after display the shortcut dialog.

};


#endif // BEEFTEXT_COMBO_EDITOR_H
