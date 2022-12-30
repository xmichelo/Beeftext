/// \file
/// \author 
///
/// \brief Implementation of combos preferences pane.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PrefPaneCombos.h"
#include "Picker/PickerWindow.h"
#include "Dialogs/ShortcutDialog.h"
#include <XMiLib/GlobalShortcut/GlobalShortcutManager.h>


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the pane.
//****************************************************************************************************************************************************
PrefPaneCombos::PrefPaneCombos(QWidget *parent)
    : PrefPane(parent)
    , prefs_(PreferencesManager::instance()) {
    ui_.setupUi(this);
    fillMatchingModeCombo(*ui_.comboMatchingMode, false);
    fillCaseSensitivityCombo(*ui_.comboCaseSensitivity, false);

    connect(ui_.buttonChangeComboTriggerShortcut, &QPushButton::clicked, this, &PrefPaneCombos::onChangeComboTriggerShortcut);
    connect(ui_.buttonChangePickerWindowShortcut, &QPushButton::clicked, this, &PrefPaneCombos::onChangePickerWindowShortcut);
    connect(ui_.buttonResetComboTriggerShortcut, &QPushButton::clicked, this, &PrefPaneCombos::onResetComboTriggerShortcut);
    connect(ui_.buttonResetPickerWindowShortcut, &QPushButton::clicked, this, &PrefPaneCombos::onResetPickerWindowShortcut);
    connect(ui_.checkComboTriggersOnSpace, &QCheckBox::toggled, this, &PrefPaneCombos::onCheckComboTriggersOnSpace);
    connect(ui_.checkEnablePickerWindow, &QCheckBox::toggled, this, &PrefPaneCombos::onCheckEnablePickerWindow);
    connect(ui_.checkKeepFinalSpaceCharacter, &QCheckBox::toggled, this, &PrefPaneCombos::onCheckKeepFinalSpace);
    connect(ui_.comboCaseSensitivity, &QComboBox::currentIndexChanged, this, &PrefPaneCombos::onChangeDefaultCaseSensitivity);
    connect(ui_.comboMatchingMode, &QComboBox::currentIndexChanged, this, &PrefPaneCombos::onChangeDefaultMatchingMode);
    connect(ui_.radioComboTriggerAuto, &QRadioButton::toggled, this, &PrefPaneCombos::onRadioAutomaticComboTrigger);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneCombos::load() const {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
    if (prefs_.useAutomaticSubstitution()) {
        QSignalBlocker blocker(ui_.radioComboTriggerAuto);
        ui_.radioComboTriggerAuto->setChecked(true);
    } else {
        QSignalBlocker blocker(ui_.radioComboTriggerManual);
        ui_.radioComboTriggerManual->setChecked(true);
    }
    QSignalBlocker blocker(ui_.checkComboTriggersOnSpace);
    ui_.checkComboTriggersOnSpace->setChecked(prefs_.comboTriggersOnSpace());
    blocker = QSignalBlocker(ui_.checkKeepFinalSpaceCharacter);
    ui_.checkKeepFinalSpaceCharacter->setChecked(prefs_.keepFinalSpaceCharacter());
    SpShortcut shortcut = prefs_.comboTriggerShortcut();
    ui_.editComboTriggerShortcut->setText(shortcut ? shortcut->toString() : "");
    selectMatchingModeInCombo(*ui_.comboMatchingMode, prefs_.defaultMatchingMode(), true);
    selectCaseSensitivityInCombo(*ui_.comboCaseSensitivity, prefs_.defaultCaseSensitivity(), true);
    blocker = QSignalBlocker(ui_.checkEnablePickerWindow);
    ui_.checkEnablePickerWindow->setChecked(prefs_.comboPickerEnabled());
    shortcut = prefs_.comboPickerShortcut();
    ui_.editPickerWindowShortcut->setText(shortcut ? shortcut->toString() : "");
#pragma clang diagnostic pop

    this->updateGui();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneCombos::updateGui() const {
    bool const comboTriggerAuto = ui_.radioComboTriggerAuto->isChecked();
    ui_.checkComboTriggersOnSpace->setEnabled(comboTriggerAuto);
    ui_.checkKeepFinalSpaceCharacter->setEnabled(comboTriggerAuto && ui_.checkComboTriggersOnSpace->isChecked());

    QWidgetList widgets = { ui_.editComboTriggerShortcut, ui_.buttonChangeComboTriggerShortcut,
                            ui_.buttonResetComboTriggerShortcut };
    for (QWidget *const widget: widgets)
        widget->setEnabled(ui_.radioComboTriggerManual->isChecked());

    widgets = { ui_.labelPickerWindowShortcut, ui_.editPickerWindowShortcut, ui_.buttonChangePickerWindowShortcut,
                ui_.buttonResetPickerWindowShortcut };
    for (QWidget *const widget: widgets)
        widget->setEnabled(ui_.checkEnablePickerWindow->isChecked());
}


//****************************************************************************************************************************************************
/// \param[in] checked Is the radio button checked
//****************************************************************************************************************************************************
void PrefPaneCombos::onRadioAutomaticComboTrigger(bool checked) const {
    prefs_.setUseAutomaticSubstitution(checked);
    this->updateGui();
}


//****************************************************************************************************************************************************
/// \param[in] checked Is the check box checked.
//****************************************************************************************************************************************************
void PrefPaneCombos::onCheckComboTriggersOnSpace(bool checked) const {
    prefs_.setComboTriggersOnSpace(checked);
    this->updateGui();
}


//****************************************************************************************************************************************************
/// \param[in] checked Is the check box checked.
//****************************************************************************************************************************************************
void PrefPaneCombos::onCheckKeepFinalSpace(bool checked) const {
    prefs_.setKeepFinalSpaceCharacter(checked);
    this->updateGui();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PrefPaneCombos::onChangeComboTriggerShortcut() {
    SpShortcut shortcut = prefs_.comboTriggerShortcut();
    if (!ShortcutDialog::run(this, shortcut, false))
        return;
    prefs_.setComboTriggerShortcut(shortcut);
    ui_.editComboTriggerShortcut->setText(shortcut ? shortcut->toString() : "");
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PrefPaneCombos::onResetComboTriggerShortcut() const {
    SpShortcut const shortcut = PreferencesManager::defaultComboTriggerShortcut();
    prefs_.setComboTriggerShortcut(shortcut);
    ui_.editComboTriggerShortcut->setText(shortcut ? shortcut->toString() : "");
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneCombos::onChangeDefaultMatchingMode() const {
    prefs_.setDefaultMatchingMode(selectedMatchingModeInCombo(*ui_.comboMatchingMode));
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneCombos::onChangeDefaultCaseSensitivity() const {
    prefs_.setDefaultCaseSensitivity(selectedCaseSensitivityInCombo(*ui_.comboCaseSensitivity));
}


//****************************************************************************************************************************************************
/// \param[in] checked Is the radio button checked
//****************************************************************************************************************************************************
void PrefPaneCombos::onCheckEnablePickerWindow(bool checked) {
    prefs_.setComboPickerEnabled(checked);
    if (!checked) {
        xmilib::GlobalShortcutManager::instance().reset();
        this->updateGui();
        return;
    }
    if (!applyComboPickerPreferences())
        this->onChangePickerWindowShortcut();
    this->updateGui();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneCombos::onChangePickerWindowShortcut() {
    SpShortcut shortcut = prefs_.comboPickerShortcut();
    xmilib::GlobalShortcutManager::instance().reset(); // Disable the shortcut
    if (ShortcutDialog::run(this, shortcut, true))
        prefs_.setComboPickerShortcut(shortcut);
    ui_.editPickerWindowShortcut->setText(shortcut ? shortcut->toString() : "");
    this->applyComboPickerShortcut();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneCombos::onResetPickerWindowShortcut() {
    SpShortcut const shortcut = PreferencesManager::defaultComboPickerShortcut();
    prefs_.setComboPickerShortcut(shortcut);
    ui_.editPickerWindowShortcut->setText(shortcut ? shortcut->toString() : "");
    this->applyComboPickerShortcut();
}


//****************************************************************************************************************************************************
/// \param[in] event The event
//****************************************************************************************************************************************************
void PrefPaneCombos::changeEvent(QEvent *event) {
    if (QEvent::LanguageChange == event->type()) {
        ui_.retranslateUi(this);
        SpShortcut const shortcut = prefs_.comboTriggerShortcut();
        ui_.editComboTriggerShortcut->setText(shortcut ? shortcut->toString() : "");

        fillMatchingModeCombo(*ui_.comboMatchingMode, false);
        selectMatchingModeInCombo(*ui_.comboMatchingMode, prefs_.defaultMatchingMode(), true);

        fillCaseSensitivityCombo(*ui_.comboCaseSensitivity, false);
        selectCaseSensitivityInCombo(*ui_.comboCaseSensitivity, prefs_.defaultCaseSensitivity(), true);
    }
    PrefPane::changeEvent(event);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PrefPaneCombos::applyComboPickerShortcut() {
    if (applyComboPickerPreferences()) {
        this->updateGui();
        return;
    }
    QMessageBox::critical(this, tr("Error"), tr("The shortcut could not be registered. Combo picker will be disabled"));
    QSignalBlocker blocker(ui_.checkEnablePickerWindow);
    ui_.checkEnablePickerWindow->setChecked(false);
    this->updateGui();
}


