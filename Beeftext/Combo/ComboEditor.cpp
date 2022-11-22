/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo editor widget.
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.


#include "stdafx.h"
#include "ComboEditor.h"
#include "Dialogs/ShortcutDialog.h"
#include "BeeftextConstants.h"


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the editor.
//****************************************************************************************************************************************************
ComboEditor::ComboEditor(QWidget *parent)
    : QPlainTextEdit(parent) {
    this->setTabChangesFocus(true);
    this->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(this, &QPlainTextEdit::customContextMenuRequested, this, &ComboEditor::onEditorContextMenuRequested);
}


//****************************************************************************************************************************************************
/// \return The content of the edit
//****************************************************************************************************************************************************
QString ComboEditor::plainText() const {
    // rawtext() preserve non-breaking space that are removed by toPlainText(), but replaces spaces with the
    // unicode paragraph separator (U+2029) so we replace them with '\n'.
    return this->document()->toRawText().replace(QString::fromUtf8(QByteArray("\xe2\x80\xa9")), QString("\n"), Qt::CaseInsensitive);
}


//****************************************************************************************************************************************************
/// \return The menu.
//****************************************************************************************************************************************************
QMenu *ComboEditor::createComboVariableMenu() {
    QMenu *menu = new QMenu(tr("&Insert Variable"), this);

    QAction *action = new QAction(tr("Clip&board Content"), this);
    connect(action, &QAction::triggered, [&]() { this->insertTextInSnippetEdit("#{clipboard}", false); });
    menu->addAction(action);

    QMenu *dtMenu = new QMenu(tr("&Date/Time"));
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
    action = new QAction(tr("&Key"), this);
    connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{key:}", true); });
    menu->addAction(action);
    action = new QAction(tr("&Shortcut"), this);
    connect(action, &QAction::triggered, this, &ComboEditor::insertShortcutVariable);
    menu->addAction(action);
    action = new QAction(tr("&Delay"), this);
    connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{delay:}", true); });
    menu->addAction(action);

    action = new QAction(tr("C&ursor Position"), this);
    connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{cursor}", false); });
    menu->addAction(action);

    QMenu *comboMenu = new QMenu(tr("Co&mbo"), this);
    action = new QAction(tr("Co&mbo"), this);
    connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{combo:}", true); });
    comboMenu->addAction(action);
    action = new QAction(tr("Combo (&uppercase)"), this);
    connect(action, &QAction::triggered, [this]() { this->insertTextInSnippetEdit("#{upper:}", true); });
    comboMenu->addAction(action);
    action = new QAction(tr("Combo (lo&wercase)"), this);
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
    action = new QAction(tr("&PowerShell Script"), this);
    connect(action, &QAction::triggered, this, &ComboEditor::insertPowershellVariable);
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


//****************************************************************************************************************************************************
/// \param[in] text The text to insert
/// \param[in] move1CharLeft Should the cursor be moved by one character to the left after insertion
//****************************************************************************************************************************************************
void ComboEditor::insertTextInSnippetEdit(QString const &text, bool move1CharLeft) {
    QTextCursor cursor = this->textCursor();
    cursor.beginEditBlock();
    cursor.insertText(text);
    if (move1CharLeft) {
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
        cursor.endEditBlock();
        this->setTextCursor(cursor); ///< Required for the cursor position change to take effect
    } else
        cursor.endEditBlock();
}


//****************************************************************************************************************************************************
/// \param[in] pos The cursos position.
//****************************************************************************************************************************************************
void ComboEditor::onEditorContextMenuRequested(QPoint const &pos) {
    QScopedPointer<QMenu, QScopedPointerDeleteLater> const menu(this->createStandardContextMenu(pos));
    menu->addSeparator();
    menu->addMenu(this->createComboVariableMenu());
    menu->exec(this->viewport()->mapToGlobal(pos));
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ComboEditor::insertPowershellVariable() {
    QString const path = QFileDialog::getOpenFileName(this, tr("Select PowerShell script file"), QString(), tr("PowerShell script files (*.ps1);;All files (*.*)"));
    if (!path.isEmpty())
        this->insertTextInSnippetEdit(QString("#{powershell:%1}").arg(path));
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ComboEditor::insertShortcutVariable() {
    SpShortcut const shortcut = ShortcutDialog::run(this, false);
    if (shortcut)
        this->insertTextInSnippetEdit(QString("#{shortcut:%1}").arg(shortcut->toString()));
}


