/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboManager.h"
#include "LastUse/ComboLastUseFile.h"
#include "WaveSound.h"
#include "InputManager.h"
#include "Preferences/PreferencesManager.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "Backup/BackupManager.h"
#include "Emoji/EmojiManager.h"


using namespace xmilib;


//****************************************************************************************************************************************************
/// \return A reference to the only allowed instance of the class
//****************************************************************************************************************************************************
ComboManager &ComboManager::instance() {
    static ComboManager instance;
    return instance;
}


//****************************************************************************************************************************************************
/// \return A mutable reference to the combo list
//****************************************************************************************************************************************************
ComboList &ComboManager::comboListRef() {
    return comboList_;
}


//****************************************************************************************************************************************************
/// \return A constant reference to the combo list
//****************************************************************************************************************************************************
ComboList const &ComboManager::comboListRef() const {
    return comboList_;
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
ComboManager::ComboManager() {
    // We used queued connections to minimize the time spent in the keyboard hook
    InputManager const &inputManager = InputManager::instance();
    connect(&inputManager, &InputManager::comboBreakerTyped, this, &ComboManager::onComboBreakerTyped, Qt::QueuedConnection);
    connect(&inputManager, &InputManager::characterTyped, this, &ComboManager::onCharacterTyped, Qt::QueuedConnection);
    connect(&inputManager, &InputManager::backspaceTyped, this, &ComboManager::onBackspaceTyped, Qt::QueuedConnection);
    connect(&inputManager, &InputManager::substitutionShortcutTriggered, this, &ComboManager::onSubstitutionTriggerShortcut, Qt::QueuedConnection);
    QString errMsg;

    if (QFileInfo(QDir(PreferencesManager::instance().comboListFolderPath())
        .absoluteFilePath(ComboList::defaultFileName)).exists()) // we avoid displaying an error on first launch
    {
        if (!this->loadComboListFromFile(&errMsg))
            QMessageBox::critical(nullptr, tr("Error"), errMsg);
    } else
        comboList_.ensureCorrectGrouping();
    this->loadSoundFromPreferences();
}


//****************************************************************************************************************************************************
/// \return A reference to the combo group attached to the combo list
//****************************************************************************************************************************************************
GroupList &ComboManager::groupListRef() {
    return comboList_.groupListRef();
}


//****************************************************************************************************************************************************
/// \return A constant reference to the combo group attached to the combo list
//****************************************************************************************************************************************************
GroupList const &ComboManager::groupListRef() const {
    return comboList_.groupListRef();
}


//****************************************************************************************************************************************************
/// \param[out] outErrorMsg If not null the function returns false, this variable will contain a description 
/// of the error.
//****************************************************************************************************************************************************
bool ComboManager::loadComboListFromFile(QString *outErrorMsg) {
    BackupManager::instance().cleanup();
    bool inOlderFormat = false;
    QString const &path = QDir(PreferencesManager::instance().comboListFolderPath())
        .absoluteFilePath(ComboList::defaultFileName);
    if (!comboList_.load(path, &inOlderFormat, outErrorMsg))
        return false;
    bool wasInvalid = false;
    comboList_.ensureCorrectGrouping(&wasInvalid);
    if (inOlderFormat || wasInvalid) {
        if (!this->saveComboListToFile(outErrorMsg))
            globals::debugLog().addWarning(inOlderFormat ?
                                           "Could not upgrade the combo list file to the newest format version." :
                                           "Could not save the combo list file after fixing the grouping of combos.");
        else
            globals::debugLog().addInfo(inOlderFormat ? "The combo list file was upgraded to the latest format version." :
                                        "The combo list file was successfully saved after fixing the the grouping of combos.");
    }
    loadComboLastUseDateTimes(comboList_);
    emit comboListWasLoaded();
    return true;
}


//****************************************************************************************************************************************************
/// \param[out] outErrorMsg If not null the function returns false, this variable will contain a description 
/// of the error.
/// \return true if and only if the operation completed successfully
//****************************************************************************************************************************************************
bool ComboManager::saveComboListToFile(QString *outErrorMsg) const {
    PreferencesManager const &prefs = PreferencesManager::instance();
    QString const filePath = QDir(prefs.comboListFolderPath()).absoluteFilePath(ComboList::defaultFileName);
    if (prefs.autoBackup())
        BackupManager::instance().archive(filePath);
    bool const result = comboList_.save(filePath, true, outErrorMsg);
    if (result)
        emit comboListWasSaved();
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] backupFilePath The path of the backup file
/// \return true if the backup was correctly restored
//****************************************************************************************************************************************************
bool ComboManager::restoreBackup(QString const &backupFilePath) {
    bool inOlderFormat = false;
    QString outErrorMsg;
    if (!comboList_.load(backupFilePath, &inOlderFormat, &outErrorMsg))
        return false;
    comboList_.ensureCorrectGrouping();
    emit backupWasRestored();
    return this->saveComboListToFile();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ComboManager::loadSoundFromPreferences() {
    PreferencesManager const &prefs = PreferencesManager::instance();
    QString const customSoundPath = prefs.customSoundPath();
    bool useCustomSound = prefs.useCustomSound();
    if (!prefs.playSoundOnCombo()) {
        sound_.reset();
        return;
    }
    if (useCustomSound) // if the custom sound file is not available, we reverse to the default one
    {
        QFileInfo const fi(customSoundPath);
        if ((!fi.exists()) || (!fi.isFile()) || (!fi.isReadable())) {
            useCustomSound = false;
            prefs.setUseCustomSound(false);
        }
    }
    sound_ = std::make_unique<WaveSound>(useCustomSound ? customSoundPath : ":/MainWindow/Resources/Notification.wav");
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ComboManager::playSound() const {
    if (sound_)
        sound_->play();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ComboManager::checkAndPerformSubstitution() {
    if (!this->checkAndPerformComboSubstitution())
        this->checkAndPerformEmojiSubstitution();
}


//****************************************************************************************************************************************************
/// \return true if a substitution was available and it was performed or cancelled because Beeftext is the active 
/// application.
//****************************************************************************************************************************************************
bool ComboManager::checkAndPerformComboSubstitution() {
    PreferencesManager const &prefs = PreferencesManager::instance();
    bool const triggersOnSpace = (prefs.useAutomaticSubstitution() && prefs.comboTriggersOnSpace());

    if (triggersOnSpace) {
        bool const cond = (!currentText_.isEmpty()) && currentText_.back().isSpace();
        Q_ASSERT(cond);
        if (!cond)
            return false;
        currentText_.chop(1); // the last character is a space, and we want to remove it before matching keywords
    }

    VecSpCombo result;
    for (SpCombo const &combo: comboList_)
        if (combo && combo->isUsable() && combo->matchesForInput(currentText_))
            result.push_back(combo);

    if (result.empty()) {
        if (triggersOnSpace)
            this->onComboBreakerTyped();
        return false;
    }

    SpCombo const combo = result[result.size() > 1 ? static_cast<quint32>(rng_.get()) % result.size() : 0];
    if ((!isBeeftextTheForegroundApplication()) &&
        (combo->performSubstitution(false) && PreferencesManager::instance().playSoundOnCombo()) && sound_)
        sound_->play(); // in Beeftext windows, substitution is disabled
    this->onComboBreakerTyped();
    return true;
}


//****************************************************************************************************************************************************
/// \return true if a substitution was available and it was performed or cancelled because Beeftext is the active 
/// application.
//****************************************************************************************************************************************************
bool ComboManager::checkAndPerformEmojiSubstitution() {
    QString currentText = currentText_;
    PreferencesManager const &prefs = PreferencesManager::instance();
    if (!prefs.emojiShortcodesEnabled())
        return false;
    QString const leftDelimiter = prefs.emojiLeftDelimiter();
    QString const rightDelimiter = prefs.emojiRightDelimiter();

    // first we validate the right delimiter, if any
    if (!currentText.endsWith(rightDelimiter))
        return false;
    currentText = currentText.chopped(rightDelimiter.size());

    // we try to locate the left delimiter
    qsizetype const index = currentText.lastIndexOf(leftDelimiter);
    if (-1 == index) // not found
        return false;

    // finally we isolate the keyword and perform the substitution, if any
    QString const keyword = currentText.right(currentText.size() - (index + leftDelimiter.size()));
    EmojiManager const &emojisManager = EmojiManager::instance();
    SpEmoji const emoji = emojisManager.find(keyword);
    if (!emoji)
        return false;
    bool result = false;
    if ((!isBeeftextTheForegroundApplication()) &&
        !EmojiManager::instance().isExcludedApplication(getActiveExecutableFileName())) {
        performTextSubstitution(qint32(keyword.size() + rightDelimiter.size() + leftDelimiter.size()), emoji->value(), -1, ETriggerSource::Keyword);
        emoji->setlastUseDateTime(QDateTime::currentDateTime());
        if (PreferencesManager::instance().playSoundOnCombo() && sound_)
            sound_->play();
        result = true;
    }
    this->onComboBreakerTyped();
    return result;
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ComboManager::onComboBreakerTyped() {
    currentText_ = QString();
}


//****************************************************************************************************************************************************
/// \param[in] c The character that was typed
//****************************************************************************************************************************************************
void ComboManager::onCharacterTyped(QChar c) {
    PreferencesManager const &prefs = PreferencesManager::instance();
    currentText_.append(c);
    if ((!prefs.useAutomaticSubstitution()) || (prefs.comboTriggersOnSpace() && (!c.isSpace())))
        return;
    this->checkAndPerformSubstitution();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ComboManager::onBackspaceTyped() {
    currentText_.chop(1);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ComboManager::onSubstitutionTriggerShortcut() {
    if (!PreferencesManager::instance().useAutomaticSubstitution())
        this->checkAndPerformSubstitution();
}
