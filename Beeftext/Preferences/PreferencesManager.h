/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of preferences manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_PREFERENCES_MANAGER_H
#define BEEFTEXT_PREFERENCES_MANAGER_H


#include "Shortcut.h"
#include "Theme.h"
#include "Combo/MatchingMode.h"
#include "Combo/CaseSensitivity.h"
#include <XMiLib/VersionNumber/VersionNumber.h>


//****************************************************************************************************************************************************
/// \brief Preferences manager class
//****************************************************************************************************************************************************
class PreferencesManager : public QObject {
Q_OBJECT

public: // static member functions
    static PreferencesManager &instance(); ///< Return the only allowed instance of the class

public: // member functions
    PreferencesManager(PreferencesManager const &) = delete; ///< Disabled copy constructor
    PreferencesManager(PreferencesManager &&) = delete; ///< Disabled move constructor
    ~PreferencesManager() override = default; ///< Default destructor
    PreferencesManager &operator=(PreferencesManager const &) = delete; ///< Disabled assignment operator
    PreferencesManager &operator=(PreferencesManager &&) = delete; ///< Disabled move assignment operator
    QSettings &settings(); ///< Returns a reference the settings for the application.
    void init() const; ///< Initialize the preferences manager.
    void reset(); ///< Reset the preferences to their default values
    bool save(QString const &path) const; ///< Save the preference to a JSON file.
    bool load(QString const &path) const; ///< Load the preference from a JSON file.
    void toJsonDocument(QJsonDocument &outDoc) const; ///< Copy the preferences to a JSON document.
    void fromJsonDocument(QJsonDocument const &doc) const; ///< Load the preferences from a JSON document.
    void resetWarnings() const; ///< Reset the warnings
    void setAlreadyLaunched() const; ///< Set the value for the 'First Launch' preference to false
    bool alreadyLaunched() const; ///< Test whether this is the first time the application is launched
    void setFileMarkedForDeletionOnStartup(QString const &path) const; ///< Get the path of the file marked for deletion on startup
    QString fileMarkedForDeletionOnStartup() const; ///< Set the path of the file marked for deletion on startup
    void clearFileMarkedForDeletionOnStartup() const; ///< Reset the file marked for deletion on startup
    void setMainWindowGeometry(QByteArray const &array) const; ///< Set the geometry of the main window
    QByteArray mainWindowGeometry() const; ///< Get the geometry of the main window
    void setMainWindowSplitterState(QByteArray const &array) const; ///< Set the geometry of the main window splitter
    QByteArray mainWindowSplitterState() const; ///< Get the geometry of the main window splitter
    QLocale locale() const; ///< Get the locale
    void setLocale(QLocale const &locale) const; ///< Set the locale
    void setLastUpdateCheckDateTime(QDateTime const &dateTime) const; ///< Set the last time the application checked for update
    QDateTime lastUpdateCheckDateTime() const; ///< Get the date time of the last update check
    void setAutoStartAtLogin(bool value) const; ///< Set the value for the 'Autostart at login' preference
    bool autoStartAtLogin() const; ///< Get the value for the 'Autostart at login' preference
    void setPlaySoundOnCombo(bool value) const; ///< Set the value for the 'Play sound on combo' preference
    bool playSoundOnCombo() const; ///< Set the value for the 'Play sound on combo' preference
    void setUseCustomSound(bool value) const; ///< Set the value for the 'Use custom sound' preference.
    bool useCustomSound() const; ///< Get the value for the 'Use custom sound' preference.
    void setCustomSoundPath(QString const &path) const; ///< Get the value for the 'Custom sound path' preferences.
    QString customSoundPath() const; ///< Set the value for the 'Custom sound path' preferences.
    void setAutoCheckForUpdates(bool value); ///< Set the value for the 'Auto check for updates preference
    bool autoCheckForUpdates() const; ///< Set the value for the 'Auto check for updates preference
    void setUseCustomTheme(bool value) const; ///< Set the value for the 'Use custom theme' preference
    bool useCustomTheme() const; ///< Get the value for the 'Use custom theme' preference
    void setUseAutomaticSubstitution(bool value) const; ///< Set the value for the 'Use automatic substitution' preference
    bool useAutomaticSubstitution() const; ///< Get the value for the 'Use automatic substitution' preference
    void setComboTriggersOnSpace(bool value) const; ///< Set the value for the 'Combo triggers on space' preference.
    bool comboTriggersOnSpace() const; ///< Set the value for the 'Combo triggers on space' preference.
    void setKeepFinalSpaceCharacter(bool value) const; ///< Set the value for the 'Keep final space character' preference.
    bool keepFinalSpaceCharacter() const; ///< Get the value for the 'Keep final space character' preference.
    void setWarnAboutShortComboKeywords(bool value) const; ///< Set the value for the 'Warn about short combo keyword' preference
    bool warnAboutShortComboKeywords() const; ///< Get the value for the 'Warn about short combo keyword' preference
    void setWarnAboutEmptyComboKeywords(bool value) const; ///< Set the value for the 'Warn about empty combo keyword' preference
    bool warnAboutEmptyComboKeywords() const; ///< Get the value for the 'Warn about empty combo keyword' preference
    bool setComboListFolderPath(QString const &path) const; ///< Set the path of the folder for saving the combo list
    QString comboListFolderPath() const; ///< Get the path of the folder for saving the combo list
    static QString defaultComboListFolderPath(); ///< Get the default combo list folder path
    void setComboTriggerShortcut(SpShortcut const &shortcut) const; ///< Set the combo trigger shortcut
    SpShortcut comboTriggerShortcut() const; ///< Retrieve the combo trigger shortcut
    void setDefaultMatchingMode(EMatchingMode mode) const; ///< Set the value for the 'Default matching mode' preference.
    EMatchingMode defaultMatchingMode() const; ///< Get the value for the 'Default matching mode' preference.
    void setDefaultCaseSensitivity(ECaseSensitivity sensitivity) const; ///< Set the value for the 'Default case sensitivity' preference
    ECaseSensitivity defaultCaseSensitivity() const; ///< Set the value for the 'Default case sensitivity' preference
    void setAutoBackup(bool value) const; ///< Set the value for the 'Auto backup' preference
    bool autoBackup() const; ///< Get the value for the 'Auto backup' preference
    void setUseCustomBackupLocation(bool value) const; ///< Set the value for the 'Use custom backup location' preference.
    bool useCustomBackupLocation() const; ///< Get the value for the 'Use custom backup location' preference.
    void setCustomBackupLocation(QString const &path) const; ///< Set the value for the 'Custom backup location' preference.
    QString customBackupLocation() const; ///< Get the value for the 'Custom backup location' preference.
    void setWriteDebugLogFile(bool value); ///< Set the value for the 'Write debug log file' preference.
    bool writeDebugLogFile() const; ///< Set the value for the 'Write debug log file' preference.
    QString lastComboImportExportPath() const; ///< Retrieve the path of the last imported and exported path
    void setLastComboImportExportPath(QString const &path) const; ///< Retrieve the path of the last imported and exported path
    static SpShortcut defaultComboTriggerShortcut(); ///< Reset the combo trigger shortcut to its default value
    bool emojiShortcodesEnabled() const; ///< Are emoji shortcodes enabled
    void setEmojiShortcodeEnabled(bool value) const; ///< Set if the emoji shortcodes are enabled
    QString emojiLeftDelimiter() const; ///< Get the left delimiter for emojis.
    void setEmojiLeftDelimiter(QString const &delimiter) const; ///< Set the left delimiter for emojis.
    QString emojiRightDelimiter() const; ///< Get the right delimiter for emojis.
    void setEmojiRightDelimiter(QString const &delimiter) const; ///< Set the right delimiter for emojis.
    void setShowEmojisInPickerWindow(bool show) const; ///< Set the value for the 'Show Emojis in picker window.
    bool showEmojisInPickerWindow() const; ///< Get the value for the 'Show Emojis in picker window.
    qint32 delayBetweenKeystrokesMs() const; ///< Get the 'delay between keystrokes' when not using the clipboard for combo substitution
    void setDelayBetweenKeystrokesMs(qint32 value) const; ///< Set the 'delay between keystrokes'
    static qint32 minDelayBetweenKeystrokesMs(); ///< Get the minimum value for the 'delay between keystrokes' preference.
    static qint32 maxDelayBetweenKeystrokesMs(); ///< Get the maximum value for the 'delay between keystrokes' preference.
    bool comboPickerEnabled() const; ///< Get the value for the 'Combo picker enabled'  preference.
    void setComboPickerEnabled(bool value) const; ///< Set the value for the 'Combo picker enabled'  preference.
    void setComboPickerShortcut(SpShortcut const &shortcut) const; ///< Set the combo picker shortcut.
    SpShortcut comboPickerShortcut() const; ///< Retrieve the combo picker shortcut.
    static SpShortcut defaultComboPickerShortcut(); ///< Return the default combo picker shortcut.
    void setEnableAppEnableDisableShortcut(bool enable) const; ///< Get the value for the 'Enable app enable/disable' shortcut.
    bool enableAppEnableDisableShortcut() const; ///< Get the value for the 'Enable app enable/disable' shortcut.
    void setAppEnableDisableShortcut(SpShortcut const &shortcut) const; ///< Set the shortcut short to enable/disable the application.
    SpShortcut appEnableDisableShortcut() const; ///< Retrieve the shortcut to enable/disable the application.
    static SpShortcut defaultAppEnableDisableShortcut(); ///< Return the default combo shortcut to enable/disable the application.
    void setBeeftextEnabled(bool enabled) const; ///< Set if beeftext is enabled.
    bool beeftextEnabled() const; ///< Set if beeftext is enabled.
    void setUseLegacyCopyPaste(bool value) const; ///< Set he value for the 'Use legacy copy/paste' preference.
    bool useLegacyCopyPaste() const; ///< Get the value for the 'Use legacy copy/paste' preference.
    bool restoreClipboardAfterSubstitution() const; ///< Get the value for the 'Restore clipboard after substitution' preference.
    void setRestoreClipboardAfterSubstitution(bool value); ///< Set the value for the 'Restore clipboard after substitution' preference.
    void setAlreadyConvertedRichTextCombos(bool value) const; ///< Set the value for the 'Already converted rich text combos' preference.
    bool alreadyConvertedRichTextCombos() const; ///< Get the value for the 'Already converted rich text combos' preference.
    void setUseCustomPowershellVersion(bool value) const; ///< Set the value for the 'Use custom PowerShell version'.
    bool useCustomPowershellVersion() const; ///< Get the value for the 'Use custom PowerShell version'.
    void setCustomPowershellPath(QString const &path) const; ///< Set the value for the 'Custom PowerShell Path'.
    QString customPowershellPath() const; ///< Set the value for the 'Custom PowerShell Path'.
    void setTheme(ETheme theme) const; ///< Set the theme parameter.
    ETheme theme() const; ///< Get the theme.
    void setComboPickerWindowGeometry(QByteArray const &geometry) const; ///< Set the geometry of the combo picker window
    QByteArray comboPickerWindowGeometry() const; ///< Get the geometry of the combo picker.
    void setUseShiftInsertForPasting(bool value) const; ///< Get the value for the 'Shift+Insert for pasting' preference
    bool useShiftInsertForPasting() const; ///< Get the value for the 'Shift+Insert for pasting' preference
    void setSkipVersionNumber(xmilib::VersionNumber const &versionNumber) const; /// Set the value for the 'Skip version number' preference.
    xmilib::VersionNumber getSkipVersionNumber() const; /// Get the value for the 'Skip version number' preference.
    void removeSkipVersionNumber() const; ///< Remove the value for the 'Skip version number' preference.

signals:
    void autoCheckForUpdatesChanged(bool value); ///< Signal emitted when the 'Auto check for updates' preference value changed
    void writeDebugLogFileChanged(bool value); ///< Signal emitted when the 'Write debug log file' preference value changed.s

private: // member functions
    PreferencesManager(); ///< Default constructor
    void applyLocalePreference() const; ///< Apply the preference for the locale
    template<typename T>
    T readSettings(QString const &key, T const &defaultValue = T()) const; ///< Read a value of a given type read from the settings

private: // data members
    class Cache {
    public: // member functions
        explicit Cache(QSettings &settings)
            : settings_(settings) {} ///< Default constructor.
        Cache(Cache const &) = delete; ///< Disabled copy-constructor.
        Cache(Cache &&) = delete; ///< Disabled assignment copy-constructor.
        ~Cache() = default; ///< Destructor.
        Cache &operator=(Cache const &) = delete; ///< Disabled assignment operator.
        Cache &operator=(Cache &&) = delete; ///< Disabled move assignment operator.
        void init(); ///< Initialize the cache.

    public: // data members
        bool useAutomaticSubstitution { true }; ///< Cached value for the 'use automatic substitution' preference value
        bool comboTriggersOnSpace { false }; ///< Cached value for the 'combo trigger on space' preference.
        bool keepFinalSpaceCharacter { false }; ///< Cached value for the 'keep final space character' preference.
        SpShortcut comboTriggerShortcut; ///< Cached value for the 'combo trigger shortcut' preference
        bool comboPickerEnabled { true }; ///< Cached value for the 'Combo picker enabled' preference.
        SpShortcut comboPickerShortcut; ///< Cached value for the 'combo picker shortcut' preference
        EMatchingMode defaultMatchingMode { EMatchingMode::Strict }; ///< Cached value for the 'Default matching mode' preference.
        ECaseSensitivity defaultCaseSensitivity { ECaseSensitivity::CaseSensitive }; ///< Cached valur for the 'Default case sensitivity' preference.
        bool enableAppEnableDisableShortcut { true }; ///< Cached value for the 'app enable/disable shortcut' preference.
        SpShortcut appEnableDisableShortcut; ///< Cached value for the 'app enable/disable shortcut' preference.
        bool emojiShortcodesEnabled { false }; ///< Cached value for the 'emoji shortcodes enabled' preference
        QString emojiLeftDelimiter; ///< Cached value for the 'emoji left delimiter' preference.
        QString emojiRightDelimiter; ///< Cached value for the 'emoji right delimiter' preference.
        bool showEmojisInPickerWindow { false }; ///< Cached value for the 'Show emojis in picker window' preference.
        bool beeftextEnabled { true }; ///< Cached value for the 'Beeftext enabled' preference.
        bool useCustomTheme { true }; ///< Cached value for the 'Use custom theme' preference.
        ETheme theme { ETheme::Light }; ///< Cached value for the 'Theme' preference.
        bool useShiftInsertForPasting { false }; ///< Cached value for use 'Use Shift+Insert for pasting' preference.

    private: // member functions
        void cacheComboTriggerShortcut(); ///< Read the combo trigger shortcut and cache it for faster access
        void cacheComboPickerShortcut(); ///< Read the combo picker shortcut and cache it for faster access
        void cacheAppEnableDisableShortcut(); ///< Read the app enable/disable shortcut and cache it for faster access.
        void cacheThemePrefs(); ///< Read the theme and cache it for faster access.
        EMatchingMode readDefaultMatchingModeFromPreferences() const; ///< Get the value for the 'Default matching mode' preference.
        ECaseSensitivity readDefaultCaseSensitivityFromPreferences() const; ///< Get the value for the 'Default case sensitivity' preference.

    private: // data members
        QSettings &settings_; ///< The settings
    }; ///< A cache for fast access to critical preferences.
    std::unique_ptr<QSettings> settings_ { nullptr }; ///< The Qt settings instance.
    std::unique_ptr<Cache> cache_ { nullptr }; ///< The cache (implemented as pimpl).
};


#endif // #ifndef BEEFTEXT_PREFERENCES_MANAGER_H