/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_COMBO_MANAGER_H
#define BEEFTEXT_COMBO_MANAGER_H


#include "ComboList.h"
#include "Group/GroupList.h"
#include "WaveSound.h"
#include <XMiLib/RandomNumberGenerator.h>
#include <memory>


//****************************************************************************************************************************************************
/// \brief A class managing the Combo and triggering the substitution when necessary
//****************************************************************************************************************************************************
class ComboManager : public QObject {
Q_OBJECT
public: // static member functions
    static ComboManager &instance(); ///< Returns a reference to the only allowed instance of the class

public: // member functions
    ComboManager(ComboManager const &) = delete; ///< Disabled copy constructor
    ComboManager(ComboManager const &&) = delete; ///< Disabled move constructor
    ~ComboManager() override = default; ///< Default destructor
    ComboManager &operator=(ComboManager const &) = delete; ///< Disabled assignment operator
    ComboManager &operator=(ComboManager const &&) = delete; ///< Disabled move assignment operator
    ComboList &comboListRef(); ///< Return a mutable reference to the combo list
    ComboList const &comboListRef() const; ///< Return a constant reference to the combo list
    GroupList &groupListRef(); ///< Return a mutable reference to the group list
    GroupList const &groupListRef() const; ///< Return a constant reference to the group list
    bool loadComboListFromFile(QString *outErrorMsg = nullptr); ///< Load the combo list from the default file
    bool saveComboListToFile(QString *outErrorMsg = nullptr) const; /// Save the combo list to the default location
    bool restoreBackup(QString const &backupFilePath); /// Restore the combo list from a backup file
    void loadSoundFromPreferences(); ///< Load the combo sound to be played from the preferences
    void playSound() const; ///< Play the combo substitution sound.
signals:
    void comboListWasLoaded() const; ///< Signal emitted when the combo list has been loaded
    void comboListWasSaved() const;  ///< Signal emitted when the combo list has been saved
    void backupWasRestored() const; ///< Signal emitted when a backup has been restored

private: // member functions
    ComboManager(); ///< Default constructor
    void checkAndPerformSubstitution(); ///< Check if a combo or emoji substitution is possible and if so performs it
    bool checkAndPerformComboSubstitution(); ///< check if a combo substitution is possible and if so performs it
    bool checkAndPerformEmojiSubstitution(); ///< check if an emoji substitution is possible and if so performs it

private slots:
    void onComboBreakerTyped(); ///< Slot for the "Combo Breaker Typed" signal
    void onCharacterTyped(QChar c); ///< Slot for the "Character Typed" signal
    void onBackspaceTyped(); ///< Slot for the 'Backspace typed" signal
    void onSubstitutionTriggerShortcut(); ///< Slot for the triggering of the substitution shortcut

private: // data member
    QString currentText_; ///< The current string
    ComboList comboList_; ///< The list of combos
    std::unique_ptr<WaveSound> sound_; ///< The sound to play when a combo is executed
    xmilib::RandomNumberGenerator rng_; ///< The RNG used to pick combos when multiple occurences are found
};


#endif // #ifndef BEEFTEXT_COMBO_MANAGER_H