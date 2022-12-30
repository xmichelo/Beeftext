/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of shortcut dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ShortcutDialog.h"
#include "InputManager.h"
#include <XMiLib/XMiLibConstants.h>


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
/// \return The shortcut typed by the user.
/// \return A null pointer if the user cancelled the dialog.
//****************************************************************************************************************************************************
SpShortcut ShortcutDialog::run(QWidget *parent, bool forGlobalRegistering) {
    ShortcutDialog dlg(parent, nullptr, forGlobalRegistering);
    return (Accepted == dlg.exec()) ? dlg.shortcut() : nullptr;
}


//****************************************************************************************************************************************************
/// \param[in,out] inOutShortcut On function entry this value holds the shortcut to be displayed by default in the
/// dialog. On exit this variable contains the shortcut select by the user if he accepted the dialog. If the dialog
/// was cancelled, this content of this variable on exit is undetermined.
/// \param[in] parent The parent widget of the dialog.
/// \return true if and only if the user accepted the dialog.
//****************************************************************************************************************************************************
bool ShortcutDialog::run(QWidget *parent, SpShortcut &inOutShortcut, bool forGlobalRegistering) {
    ShortcutDialog dlg(parent, inOutShortcut, forGlobalRegistering);
    if (Accepted != dlg.exec())
        return false;
    inOutShortcut = dlg.shortcut();
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] shortcut The shortcut
/// \param[in] parent The parent widget of the dialog
//****************************************************************************************************************************************************
ShortcutDialog::ShortcutDialog(QWidget *parent, SpShortcut const &shortcut, bool forGlobalRegistering)
    : QDialog(parent, xmilib::constants::kDefaultDialogFlags)
    , ui_()
    , shortcut_(shortcut)
    , forGlobalRegistering_(forGlobalRegistering) {
    ui_.setupUi(this);
    ui_.labelError->setText(QString());
    this->resize(QDialog::sizeHint());
    this->updateGui();
    InputManager &inputManager = InputManager::instance();
    shortcutsWereEnabled_ = inputManager.isShortcutProcessingEnabled();
    inputManager.setShortcutsProcessingEnabled(false);
    connect(&InputManager::instance(), &InputManager::shortcutPressed, this, &ShortcutDialog::onShortcutPressed);
    connect(ui_.buttonCancel, &QPushButton::clicked, this, &ShortcutDialog::reject);
    connect(ui_.buttonOK, &QPushButton::clicked, this, &ShortcutDialog::accept);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
ShortcutDialog::~ShortcutDialog() {
    InputManager::instance().setShortcutsProcessingEnabled(shortcutsWereEnabled_);
}


//****************************************************************************************************************************************************
/// \return The shortcut
//****************************************************************************************************************************************************
SpShortcut ShortcutDialog::shortcut() const {
    return shortcut_;
}


//****************************************************************************************************************************************************
/// \param[in] shortcut The shortcut
//****************************************************************************************************************************************************
void ShortcutDialog::onShortcutPressed(SpShortcut const &shortcut) {
    if (!this->validateShortcut(shortcut))
        return;
    shortcut_ = shortcut;
    this->updateGui();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ShortcutDialog::updateGui() const {
    ui_.editShortcut->setText(shortcut_ ? shortcut_->toString() : "");
    ui_.buttonOK->setEnabled(shortcut_.get());
}


//****************************************************************************************************************************************************
/// \param[in] shortcut The shortcut.
/// \return true if and only if the shortcut is valid.
//****************************************************************************************************************************************************
bool ShortcutDialog::validateShortcut(SpShortcut const &shortcut) const {
    ui_.labelError->setText(QString());
    if ((!shortcut) || (!shortcut->isValid()))
        return false;
    if (!forGlobalRegistering_)
        return true;
    bool const useWinKey = shortcut->keyboardModifiers().testFlag(Qt::MetaModifier);
    if (useWinKey)
        ui_.labelError->setText(tr("This shortcut cannot use the Windows key."));
    this->updateGui();
    return !useWinKey;
}
