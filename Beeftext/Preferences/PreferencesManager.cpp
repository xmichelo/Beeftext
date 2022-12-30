/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of preferences manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferencesManager.h"
#include "AutoStart.h"
#include "Theme.h"
#include "I18nManager.h"
#include "Picker/PickerWindow.h"
#include "Combo/ComboManager.h"
#include "Clipboard/ClipboardManager.h"
#include "Backup/BackupManager.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QString const kKeyAlreadyLaunched = "AlreadyLaunched"; ///< The settings key for the "Already launched" indicator
QString const kKeyAppEnableShortcut = "AppEnableDisableShortcut"; ///< The setting key for the app enable/disable shortcut modifiers.
QString const kKeyAppEnableShortcutKeyCodeDeprecated = "AppEnableDisableShortcutKeyCode"; ///< The setting key for the app enable/disable shortcut key code.
QString const kKeyAppEnableShortcutModifiersDeprecated = "AppEnableDisableShortcutModifiers"; ///< The setting key for the app enable/disable shortcut modifiers.
QString const kKeyAppEnableShortcutScanCodeDeprecated = "AppEnableDisableShortcutScanCode"; ///< The setting key for the app enable/disable shortcut scan code.
QString const kKeyAutoBackup = "AutoBackup"; ///< The setting key for the 'Auto backup' preference
QString const kKeyAutoCheckForUpdates = "AutoCheckForUpdate"; ///< The settings key for the 'Autostart at login' preference
QString const kKeyAutoStartAtLogin = "AutoStartAtLogin"; ///< The settings key for the 'Autostart at login' preference
QString const kKeyBeeftextEnabled = "BeeftextEnabled"; ///< The setting key for the 'Beeftext enabled' preference.
QString const kKeyComboListFolderPath = "ComboListFolderPath"; ///< The setting key for the combo list folder path
QString const kKeyComboPickerEnabled = "ComboPickerEnabled"; ///< The setting key for the 'Combo picker enabled' preference.
QString const kKeyComboPickerShortcut = "ComboPickerShortcut"; ///< The setting key for the combo picker shortcut.
QString const kKeyComboPickerShortcutModifiersDeprecated = "ComboPickerShortcutModifiers"; ///< The setting key for the combo picker shortcut modifiers
QString const kKeyComboPickerShortcutKeyCodeDeprecated = "ComboPickerShortcutKeyCode"; ///< The setting key for the combo picker shortcut key code
QString const kKeyComboPickerShortcutScanCodeDeprecated = "ComboPickerShortcutScanCode"; ///< The setting key for the combo picker shortcut scan code
QString const kKeyComboTriggerShortcut = "ComboTriggerShortcut"; ///< The setting key for the combo trigger shortcut.
QString const kKeyComboTriggerShortcutModifiersDeprecated = "ComboTriggerShortcutModifiers"; ///< The setting key for the combo trigger shortcut modifiers
QString const kKeyComboTriggerShortcutKeyCodeDeprecated = "ComboTriggerShortcutKeyCode"; ///< The setting key for the combo trigger shortcut key code
QString const kKeyComboTriggerShortcutScanCodeDeprecated = "ComboTriggerShortcutScanCode"; ///< The setting key for the combo trigger shortcut scan code
QString const kKeyDefaultMatchingMode = "DefaultMatchingMode"; ///< The setting key for the default matching mode.
QString const kKeyDefaultCaseSensitivity = "DefaultCaseSensitivity"; ///< The setting key for the 'Default case sensitivity' preference.
QString const kKeyCustomBackupLocation = "CustomBackupLocation"; ///< The settings key for the 'Custom backup location' preference.
QString const kKeyCustomSoundPath = "CustomSoundPath"; ///< The settings key for the 'Custom sound path' preference.
QString const kKeyDelayBetweenKeystrokes = "DelayBetweenKeystrokes"; ///< The setting key for the 'Delay between keystrokes' preferences value
QString const kKeyEmojiLeftDelimiter = "EmojiLeftDelimiter"; ///< The setting key for the emoji left delimiter.
QString const kKeyEmojiRightDelimiter = "EmojiRightDelimiter"; ///< The setting key for the emoji right delimiter.
QString const kKeyEmojiShortcodesEnabled = "EmojiShortcodesEnabled"; ///< The setting key for the 'Emoji shortcodes enabled'
QString const kKeyShowEmojisInPickerWindow = "ShowEmojisInPickerWindow"; ///< The setting key for the 'Show emojis in picker window preference. 
QString const kKeyEnableAppEnableDisableShortcut = "EnableAppEnableDisableShortcut"; ///< The setting key for the 'enable app enable/disable shortcut' preference.
QString const kKeyFileMarkedForDeletion = "markedForDeletion"; ///< The path of the file marked for deletion on next application startup
QString const kKeyGeometry = "Geometry"; ///< The settings key for storing the geometry
QString const kKeyLastComboImportExportPath = "LastComboImportExportPath"; ///< The setting key for 'Last combo import/export path' preference
QString const kKeyLastUpdateCheckDateTime = "LastUpdateCheck"; ///< The setting key for the last update check date/time
QString const kKeyLocale = "Locale"; ///< The settings key for the locale
QString const kKeyPlaySoundOnCombo = "PlaySoundOnCombo"; ///< The settings key for the 'Play sound on combo' preference
QString const kKeySplitterState = "MainWindowSplitterState"; ///< The setting key for storing the main window splitter state.
QString const kKeyUseAutomaticSubstitution = "UseAutomaticSubstitution"; ///< The setting key for the 'Use automatic substitution' preference
QString const kKeyUseCustomBackupLocation = "UseCustomBackupLocation"; ///< The settings key for the 'Use custom backup location' preference.
QString const kKeyUseCustomSound = "UseCustomSound"; ///< The settings key for the 'Use custom sound' preference.
QString const kKeyUseCustomTheme = "UseCustomTheme"; ///< The setting key for the 'Use custom theme' preference
QString const kKeyWarnAboutShortComboKeyword = "WarnAboutShortComboKeyword"; ///< The setting key for the 'Warn about short combo keyword' preference
QString const kKeyWarnAboutEmptyComboKeyword = "WarnAboutEmptyComboKeyword"; ///< The setting key for the 'Warn about empty combo keyword' preference
QString const kKeyWriteDebugLogFile = "WriteDebugLogFile"; ///< The setting key for the 'Write debug log file' preference.
QString const kKeyRichTextDeprecationWarningHasAlreadyBeenDisplayed = "RichTextDeprecationWarningHasAlreadyBeenDisplayed"; ///< The setting key for teh 'Rich Text Deprecation Warning Has Already Been Displayed' preference.
QString const kKeyUseLegacyCopyPaste = "UseLegacyCopyPaste"; ///< The setting key for the 'Use legacy copy/paste' preference.
QString const kKeyRestoreClipboardAfterSubstitution = "RestoreClipboardAfterSubstitution"; ///< The settings key for the 'Restore clipboard after substitution' preference.
QString const kKeyComboTriggersOnSpace = "ComboTriggersOnSpace"; ///< The setting key for the 'Combo triggers on space' preference.
QString const kKeyKeepFinalSpaceCharacter = "KeepFinalSpaceCharacter"; ///< The setting key for the 'Keep final space character' preference.
QString const kKeyAlreadyConvertedRichTextCombos = "AlreadyConvertedRichTextCombos"; ///< The setting key for the 'Already converted rich text combos' preference.
QString const kKeyUseCustomPowershellVersion = "UseCustomPowershellVersion"; ///< The setting key for the 'Use custom PowerShell version'.
QString const kKeyCustomPowershellPath = "CustomPowershellPath"; ///< The setting key for the 'Custom PowerShell path'.
QString const kKeyTheme = "Theme"; ///< The setting key for the 'Theme' preference.
QString const kKeyComboPickerWindowGeometry = "ComboPickerWindowGeometry"; ///< The settings key for the combo picker geometry.
QString const kKeyUseShiftInsertForPasting = "UseShiftInsertForPasting"; ///< The settings key for the 'Use Shift+Insert for pasting' preference
QString const kKeySkipVersionNumber = "SkipVersionNumber"; ///< The settings key for the 'skip version number' preference.


SpShortcut const kDefaultAppEnableDisableShortcut = Shortcut::fromModifiersAndKey(Qt::AltModifier | Qt::ShiftModifier
    | Qt::ControlModifier, Qt::Key('V')); ///< The default value for the 'combo trigger shortcut' preference
bool constexpr kDefaultAutoBackup = true; ///< The default value for the 'Auto backup' preference
bool constexpr kDefaultAutoCheckForUpdates = true; ///< The default value for the 'Auto check for update preference
bool constexpr kDefaultAutoStartAtLogin = false; ///< The default value for the 'Autostart at login' preference
bool constexpr kDefaultBeeftextEnabled = true; ///< The default value for the 'Beeftext is enabled' preference.
bool constexpr kDefaultComboPickerEnabled = true; ///< The default value for the 'Combo picker enabled' preference.
SpShortcut const kDefaultComboTriggerShortcut = Shortcut::fromModifiersAndKey(Qt::AltModifier | Qt::ShiftModifier
    | Qt::ControlModifier, Qt::Key('B')); ///< The default value for the 'combo trigger shortcut' preference
EMatchingMode kDefaultDefaultMatchingMode = EMatchingMode::Strict; ///< The default value for the 'Default matching mode preference'.
ECaseSensitivity kDefaultDefaultCaseSensitivity = ECaseSensitivity::CaseSensitive; ///< The default value for the 'Default case sensitivity' preference.
qint32 constexpr kDefaultDelayBetweenKeystrokesMs = 12; ///< The default valur for the 'Delay between keystrokes' preference.
QString const kDefaultEmojiLeftDelimiter = "|"; ///< The default left delimiter for emojis
QString const kDefaultEmojiRightDelimiter = "|"; ///< The default left delimiter for emojis
bool constexpr kDefaultEmojiShortcodesEnabled = false; ///< The default value for the 'Emoji shortcodes enabled' preference.
bool constexpr kDefaultShowEmojisInPickerWindow = false; ///< The default value for the 'Show emojis in picker window' preference.
bool constexpr kDefaultEnableAppEnableDisableShortcut = false; ///< The default value for the 'enable app enable/disable shortcut' preference.
QString const kDefaultLastComboImportExportPath = QDir(QStandardPaths::writableLocation(
    QStandardPaths::DesktopLocation)).absoluteFilePath("Combos.json");///< The default value for the 'Last combo import/export path' preference
qint32 constexpr kMinValueDelayBetweenKeystrokesMs = 0; ///< The default valur for the 'Delay between keystrokes' preference.
qint32 constexpr kMaxValueDelayBetweenKeystrokesMs = 500; ///< The default valur for the 'Delay between keystrokes' preference.
bool constexpr kDefaultPlaySoundOnCombo = true; ///< The default value for the 'Play sound on combo' preference
bool constexpr kDefaultUseAutomaticSubstitution = true; ///< The default value for the 'Use automatic substitution' preference
bool constexpr kDefaultUseCustomBackupLocation = false; ///< The default value for the 'Use custom backup location' preference.
bool constexpr kDefaultUseCustomSound = false; ///< The default value for the 'Use custom sound' preference.
bool constexpr kDefaultUseCustomTheme = true; ///< The default value for the 'Use custom theme' preference
bool constexpr kDefaultWarnAboutShortComboKeyword = true; ///< The default value for the 'Warn about short combo keyword' preference
bool constexpr kDefaultWarnAboutEmptyComboKeyword = true; ///< The default value for the 'Warn about empty combo keyword' preference
bool constexpr kDefaultWriteDebugLogFile = true; ///< The default value for the 'Write debug log file' preference
bool constexpr kDefaultKeyRichTextDeprecationWarningHasAlreadyBeenDisplayed = false; ///< The default value for the 'Rich Text Deprecation Warning Has Already Been Displayed' preference.
bool constexpr kDefaultUseLegacyCopyPaste = false; ///< The default value for the 'Use legacy copy/paste' preference.
bool constexpr kDefaultRestoreClipboardAfterSubstitution = true; ///< The default value for the 'Restore clipboard after substitution' preference.
bool constexpr kDefaultComboTriggersOnSpace = false; ///< The default value for the 'Combo triggers on space' preference.
bool constexpr kDefaultKeepFinalSpaceCharacter = false; ///< The default value for the 'Combo triggers on space' preference.
bool constexpr kDefaultUseCustomPowershellVersion = false; ///< The default value for the 'Use custom PowerShell version' preference.
ETheme constexpr kDefaultTheme = ETheme::Light; ///< The default value for the theme preference.
bool constexpr kDefaultUseShiftInsertForPasting = false; ///< The default value for the 'Use Shift+Insert for pasting' preference.

}


//****************************************************************************************************************************************************
/// This function returns the default value if the key does not exist in the settings OR if the value is not of the
/// expected data type.
///
/// \param[in] settings The settings to read from.
/// \param[in] key The key to read from
/// \param[in] defaultValue The default value to use if the key does not exist or its value is not of the right type
/// \return The read value
//****************************************************************************************************************************************************
template<typename T>
T readSettings(QSettings &settings, QString const &key, T const &defaultValue) {
    if (!settings.contains(key))
        return defaultValue;
    QVariant const v = settings.value(key, QVariant::fromValue<T>(defaultValue));
    return v.canConvert<T>() ? qvariant_cast<T>(v) : defaultValue;
}


//****************************************************************************************************************************************************
/// This function returns the default value if the key does not exist in the settings OR if the value is not of the
/// expected data type.
///
/// \param[in] key The key to read from
/// \param[in] defaultValue The default value to use if the key does not exist or its value is not of the right type
/// \return The read value
//****************************************************************************************************************************************************
template<typename T>
T PreferencesManager::readSettings(QString const &key, T const &defaultValue) const {
    return ::readSettings<T>(*settings_, key, defaultValue);
}


//****************************************************************************************************************************************************
/// \param[in] modRegKey The registry key for the shortcut's  modifiers.
/// \param[in] vKeyRegKey The registry key for the shortcut's virtual key.
/// \param[in] scanCodeRegKey The registry key for the shortcut's scan code.
/// \return The shortcut, or a null pointer if not found
//****************************************************************************************************************************************************
SpShortcut readShortcutFromPreferencesDeprecated(QSettings &settings, QString const &modRegKey, QString const &vKeyRegKey,
    QString const &scanCodeRegKey) {
    int const intMods = readSettings<qint32>(settings, modRegKey, 0);
    quint32 const vKey = readSettings<quint32>(settings, vKeyRegKey, 0);
    quint32 const scanCode = readSettings<quint32>(settings, scanCodeRegKey, 0);
    if ((!intMods) || (!vKey) || (!scanCode))
        return nullptr;
    SpShortcut const result = std::make_shared<Shortcut>(Qt::KeyboardModifiers(intMods), Qt::Key(vKey));
    return result->isValid() ? result : nullptr;
}


//****************************************************************************************************************************************************
/// \param[in] settings The settings.
/// \param[in] key The key.
/// \return The shortcut read from the preferences.
/// \return A null pointer if the shortcut could not be found
//****************************************************************************************************************************************************
SpShortcut readShortcutFromPreferences(QSettings const &settings, QString const &key) {
    if (!settings.contains(key))
        return nullptr;
    QVariant const var = settings.value(key);
    return (var.isNull() || !var.canConvert<qint32>()) ? nullptr : Shortcut::fromCombined(var.value<qint32>());
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesManager::Cache::init() {
    // Cache often accessed values
    useAutomaticSubstitution = ::readSettings<bool>(settings_, kKeyUseAutomaticSubstitution, kDefaultUseAutomaticSubstitution);
    comboTriggersOnSpace = ::readSettings<bool>(settings_, kKeyComboTriggersOnSpace, kDefaultComboTriggersOnSpace);
    keepFinalSpaceCharacter = ::readSettings<bool>(settings_, kKeyKeepFinalSpaceCharacter, kDefaultKeepFinalSpaceCharacter);
    cacheComboTriggerShortcut();
    comboPickerEnabled = ::readSettings<bool>(settings_, kKeyComboPickerEnabled, kDefaultComboPickerEnabled);
    cacheComboPickerShortcut();
    defaultMatchingMode = this->readDefaultMatchingModeFromPreferences();
    defaultCaseSensitivity = this->readDefaultCaseSensitivityFromPreferences();
    emojiShortcodesEnabled = ::readSettings<bool>(settings_, kKeyEmojiShortcodesEnabled, kDefaultEmojiShortcodesEnabled);
    showEmojisInPickerWindow = ::readSettings<bool>(settings_, kKeyShowEmojisInPickerWindow, kDefaultShowEmojisInPickerWindow);
    enableAppEnableDisableShortcut = ::readSettings<bool>(settings_, kKeyEnableAppEnableDisableShortcut, kDefaultEnableAppEnableDisableShortcut);
    cacheAppEnableDisableShortcut();
    cacheThemePrefs();
    emojiLeftDelimiter = ::readSettings<QString>(settings_, kKeyEmojiLeftDelimiter, kDefaultEmojiLeftDelimiter);
    emojiRightDelimiter = ::readSettings<QString>(settings_, kKeyEmojiRightDelimiter, kDefaultEmojiRightDelimiter);
    beeftextEnabled = ::readSettings<bool>(settings_, kKeyBeeftextEnabled, kDefaultBeeftextEnabled);
    useShiftInsertForPasting = ::readSettings<bool>(settings_, kKeyUseShiftInsertForPasting, kDefaultUseShiftInsertForPasting);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PreferencesManager::Cache::cacheComboTriggerShortcut() {
    comboTriggerShortcut = readShortcutFromPreferences(settings_, kKeyComboTriggerShortcut);
    if (comboTriggerShortcut)
        return;
    comboTriggerShortcut = readShortcutFromPreferencesDeprecated(settings_, kKeyComboTriggerShortcutModifiersDeprecated, kKeyComboTriggerShortcutKeyCodeDeprecated, kKeyComboTriggerShortcutScanCodeDeprecated);
    if (comboTriggerShortcut) {
        settings_.setValue(kKeyComboTriggerShortcut, comboTriggerShortcut->toCombined());
        settings_.remove(kKeyComboTriggerShortcutModifiersDeprecated);
        settings_.remove(kKeyComboTriggerShortcutKeyCodeDeprecated);
        settings_.remove(kKeyComboTriggerShortcutScanCodeDeprecated);
        return;
    }
    comboTriggerShortcut = kDefaultComboTriggerShortcut;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesManager::Cache::cacheComboPickerShortcut() {
    comboPickerShortcut = readShortcutFromPreferences(settings_, kKeyComboPickerShortcut);
    if (comboPickerShortcut)
        return;
    comboPickerShortcut = readShortcutFromPreferencesDeprecated(settings_, kKeyComboPickerShortcutModifiersDeprecated, kKeyComboPickerShortcutKeyCodeDeprecated, kKeyComboPickerShortcutScanCodeDeprecated);
    if (comboPickerShortcut) {
        settings_.setValue(kKeyComboPickerShortcut, comboPickerShortcut->toCombined());
        settings_.remove(kKeyComboPickerShortcutModifiersDeprecated);
        settings_.remove(kKeyComboPickerShortcutKeyCodeDeprecated);
        settings_.remove(kKeyComboPickerShortcutScanCodeDeprecated);
        return;
    }
    comboPickerShortcut = defaultComboPickerShortcut();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesManager::Cache::cacheAppEnableDisableShortcut() {
    appEnableDisableShortcut = readShortcutFromPreferences(settings_, kKeyAppEnableShortcut);
    if (appEnableDisableShortcut)
        return;
    // Does the shortcut exists in deprecated form?
    appEnableDisableShortcut = readShortcutFromPreferencesDeprecated(settings_, kKeyAppEnableShortcutModifiersDeprecated, kKeyAppEnableShortcutKeyCodeDeprecated, kKeyAppEnableShortcutScanCodeDeprecated);
    // if so we upgrade to the new form and remove the deprecated version
    if (appEnableDisableShortcut) {
        settings_.setValue(kKeyAppEnableShortcut, appEnableDisableShortcut->toCombined());
        settings_.remove(kKeyAppEnableShortcutModifiersDeprecated);
        settings_.remove(kKeyAppEnableShortcutKeyCodeDeprecated);
        settings_.remove(kKeyAppEnableShortcutScanCodeDeprecated);
        return;
    }
    appEnableDisableShortcut = kDefaultAppEnableDisableShortcut;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesManager::Cache::cacheThemePrefs() {
    qint32 const intValue = ::readSettings<qint32>(settings_, kKeyTheme, static_cast<qint32>(kDefaultTheme));
    theme = ((intValue < 0) || (intValue >= static_cast<qint32>(ETheme::Count))) ? kDefaultTheme
        : static_cast<ETheme>(intValue);

    useCustomTheme = ::readSettings<bool>(settings_, kKeyUseCustomTheme, kDefaultUseCustomTheme);
}


//****************************************************************************************************************************************************
/// \return The default matching mode read from the preferences
//****************************************************************************************************************************************************
EMatchingMode PreferencesManager::Cache::readDefaultMatchingModeFromPreferences() const {
    EMatchingMode const mode = static_cast<EMatchingMode>(::readSettings<qint32>(settings_, kKeyDefaultMatchingMode, static_cast<qint32>(EMatchingMode::Strict)));
    switch (mode) {
    case EMatchingMode::Strict:
    case EMatchingMode::Loose:
        return mode;
    default:
        return EMatchingMode::Strict;
    }
}


//****************************************************************************************************************************************************
/// \return The default case sensitivity read from the preferences.
//****************************************************************************************************************************************************
ECaseSensitivity PreferencesManager::Cache::readDefaultCaseSensitivityFromPreferences() const {
    ECaseSensitivity const sensitivity = intToCaseSensitivity(::readSettings<qint32>(settings_, kKeyDefaultCaseSensitivity, caseSensitivityToInt(kDefaultDefaultCaseSensitivity)));
    switch (sensitivity) {
    case ECaseSensitivity::CaseSensitive:
    case ECaseSensitivity::CaseInsensitive:
        return sensitivity;
    case ECaseSensitivity::Count:
    case ECaseSensitivity::Default:
    default:
        return kDefaultDefaultCaseSensitivity;
    }
}


//****************************************************************************************************************************************************
/// \return The only allowed instance of the class
//****************************************************************************************************************************************************
PreferencesManager &PreferencesManager::instance() {
    static PreferencesManager prefManager;
    return prefManager;
}


//****************************************************************************************************************************************************
/// \note We set the organization and application names manually in case we want to use the preferences before the call 
/// to QApplication::SetOrganizationName() and QApplication::SetApplicationName()
//****************************************************************************************************************************************************
PreferencesManager::PreferencesManager() {
    // portable edition use a different storage method for preferences
    settings_ = isInPortableMode()
        ? std::make_unique<QSettings>(globals::portableModeSettingsFilePath(), QSettings::IniFormat)
        : std::make_unique<QSettings>(constants::kOrganizationName, constants::kApplicationName);
    cache_ = std::make_unique<Cache>(*settings_);
    this->init();
}


//****************************************************************************************************************************************************
/// \return A reference to the settings object of the manager
//****************************************************************************************************************************************************
QSettings &PreferencesManager::settings() {
    Q_ASSERT(settings_);
    return *settings_;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesManager::init() const {
    cache_->init();
    applyThemePreferences(this->useCustomTheme(), this->theme());
    this->applyLocalePreference();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PreferencesManager::reset() {
    this->setAppEnableDisableShortcut(defaultAppEnableDisableShortcut());
    this->setAutoBackup(kDefaultAutoBackup);
    this->setAutoCheckForUpdates(kDefaultAutoCheckForUpdates);
    this->setComboPickerEnabled(kDefaultComboPickerEnabled);
    this->setComboPickerShortcut(defaultComboPickerShortcut());
    this->setComboTriggerShortcut(kDefaultComboTriggerShortcut);
    this->setDefaultMatchingMode(kDefaultDefaultMatchingMode);
    this->setDefaultCaseSensitivity(kDefaultDefaultCaseSensitivity);
    this->setCustomBackupLocation(globals::defaultBackupFolderPath());
    this->setCustomSoundPath(QString());
    this->setDelayBetweenKeystrokesMs(kDefaultDelayBetweenKeystrokesMs);
    this->setEmojiLeftDelimiter(kDefaultEmojiLeftDelimiter);
    this->setEmojiRightDelimiter(kDefaultEmojiRightDelimiter);
    this->setEmojiShortcodeEnabled(kDefaultEmojiShortcodesEnabled);
    this->setEnableAppEnableDisableShortcut(kDefaultEnableAppEnableDisableShortcut);
    this->setLocale(I18nManager::instance().validateLocale(QLocale::system()));
    this->setPlaySoundOnCombo(kDefaultPlaySoundOnCombo);
    this->setUseAutomaticSubstitution(kDefaultUseAutomaticSubstitution);
    this->setComboTriggersOnSpace(kDefaultComboTriggersOnSpace);
    this->setKeepFinalSpaceCharacter(kDefaultKeepFinalSpaceCharacter);
    this->setUseCustomBackupLocation(kDefaultUseCustomBackupLocation);
    this->setUseCustomTheme(kDefaultUseCustomTheme);
    this->setTheme(static_cast<ETheme>(kDefaultTheme));
    this->setUseCustomSound(kDefaultUseCustomSound);
    this->setWarnAboutShortComboKeywords(kDefaultWarnAboutShortComboKeyword);
    this->setWriteDebugLogFile(kDefaultWriteDebugLogFile);
    this->resetWarnings();
    this->setUseLegacyCopyPaste(kDefaultUseLegacyCopyPaste);
    this->setRestoreClipboardAfterSubstitution(kDefaultRestoreClipboardAfterSubstitution);
    this->setUseShiftInsertForPasting(kDefaultUseShiftInsertForPasting);
    if (!isInPortableMode()) {
        this->setAutoStartAtLogin(kDefaultAutoStartAtLogin);
        this->setComboListFolderPath(globals::appDataDir());
    }
    applyComboPickerPreferences();
}


//****************************************************************************************************************************************************
/// \param[in] path The path of the file to save to.
/// \return true if and only if the operation was completed successfully.
//****************************************************************************************************************************************************
bool PreferencesManager::save(QString const &path) const {
    try {
        QJsonDocument doc;
        this->toJsonDocument(doc);
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            throw Exception("The output file could not be opened.");
        QByteArray const data = doc.toJson();
        if (data.size() != file.write(data))
            throw Exception("An error occurred while trying to write file.");
        return true;
    }
    catch (Exception const &e) {
        globals::debugLog().addError(e.qwhat());
        return false;
    }
}


//****************************************************************************************************************************************************
/// \param[in] path The path of the file to load from.
/// \return true if and only if the operation was completed successfully.
//****************************************************************************************************************************************************
bool PreferencesManager::load(QString const &path) const {
    try {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw Exception("The output file could not be opened.");
        QJsonDocument const doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isNull())
            throw Exception("The input file is not a valid JSON file.");
        this->fromJsonDocument(doc);
        return true;
    }
    catch (Exception const &e) {
        globals::debugLog().addError(e.qwhat());
        return false;
    }
}


//****************************************************************************************************************************************************
/// \param[in] object The JSON object.
/// \param[in] key The key.
/// \return The value associated with the key
//****************************************************************************************************************************************************
template<typename T>
T objectValue(QJsonObject const &object, QString const &key) {
    if (!object.contains(key))
        throw Exception(QString("Could not find entry %1 in imported preferences file.").arg(key));
    QVariant const v = object[key].toVariant();
    if (!v.canConvert<T>())
        throw Exception(QString("The value for entry %1 in imported preferences file is invalid.").arg(key));
    return v.value<T>();
}


//****************************************************************************************************************************************************
/// \param[in] v The QVariant.
/// \return A serializable array containing a QVariant.
//****************************************************************************************************************************************************
QByteArray variantToByteArray(QVariant const &v) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << v;
    return data;
}


//****************************************************************************************************************************************************
/// \brief Deserialize a QVariant from a byte array
///
/// \param[in] data The byte array.
/// \return The deserialized QVariant
//****************************************************************************************************************************************************
template<typename T>
T byteArrayToVariant(QByteArray const &data) {
    QByteArray array(data);
    QDataStream stream(&array, QIODevice::ReadOnly);
    QVariant v;
    stream >> v;
    if (!v.canConvert<T>())
        throw Exception("The imported file is invalid.");
    return v.value<T>();
}


//****************************************************************************************************************************************************
/// \param[out] outDoc The JSON document.
//****************************************************************************************************************************************************
void PreferencesManager::toJsonDocument(QJsonDocument &outDoc) const {
    QJsonObject object;
    object[kKeyAppEnableShortcutKeyCodeDeprecated] = qint32(this->readSettings<quint32>(kKeyAppEnableShortcutKeyCodeDeprecated, 0));
    object[kKeyAppEnableShortcutModifiersDeprecated] = qint32(this->readSettings<quint32>(kKeyAppEnableShortcutModifiersDeprecated, 0));
    object[kKeyAppEnableShortcutScanCodeDeprecated] = qint32(this->readSettings<quint32>(kKeyAppEnableShortcutScanCodeDeprecated, 0));
    object[kKeyAutoBackup] = this->readSettings<bool>(kKeyAutoBackup, kDefaultAutoBackup);
    object[kKeyAutoCheckForUpdates] = this->readSettings<bool>(kKeyAutoCheckForUpdates, kDefaultAutoCheckForUpdates);
    object[kKeyAutoStartAtLogin] = this->readSettings<bool>(kKeyAutoStartAtLogin, kDefaultAutoStartAtLogin);
    object[kKeyBeeftextEnabled] = this->readSettings<bool>(kKeyBeeftextEnabled, kDefaultBeeftextEnabled);
    object[kKeyComboListFolderPath] = this->readSettings<QString>(kKeyComboListFolderPath, defaultComboListFolderPath());
    object[kKeyComboPickerEnabled] = this->readSettings<bool>(kKeyComboPickerEnabled, kDefaultComboPickerEnabled);
    object[kKeyComboPickerShortcutKeyCodeDeprecated] = qint32(this->readSettings<quint32>(kKeyComboPickerShortcutKeyCodeDeprecated, 0));
    object[kKeyComboPickerShortcutModifiersDeprecated] = qint32(this->readSettings<quint32>(kKeyComboPickerShortcutModifiersDeprecated, 0));
    object[kKeyComboPickerShortcutScanCodeDeprecated] = qint32(this->readSettings<quint32>(kKeyComboPickerShortcutScanCodeDeprecated, 0));
    object[kKeyComboTriggerShortcutKeyCodeDeprecated] = qint32(this->readSettings<quint32>(kKeyComboTriggerShortcutKeyCodeDeprecated, 0));
    object[kKeyComboTriggerShortcutModifiersDeprecated] =
        qint32(this->readSettings<quint32>(kKeyComboTriggerShortcutModifiersDeprecated, 0));
    object[kKeyComboTriggerShortcutScanCodeDeprecated] =
        qint32(this->readSettings<quint32>(kKeyComboTriggerShortcutScanCodeDeprecated, 0));
    object[kKeyDefaultMatchingMode] = qint32(this->readSettings<qint32>(kKeyDefaultMatchingMode, static_cast<qint32>(kDefaultDefaultMatchingMode)));
    object[kKeyDefaultCaseSensitivity] = this->readSettings<qint32>(kKeyDefaultCaseSensitivity, qint32(kDefaultDefaultCaseSensitivity));
    object[kKeyCustomBackupLocation] = this->readSettings<QString>(kKeyCustomBackupLocation, globals::defaultBackupFolderPath());
    object[kKeyCustomSoundPath] = this->readSettings<QString>(kKeyCustomSoundPath, QString());
    object[kKeyDelayBetweenKeystrokes] = this->readSettings<qint32>(kKeyDelayBetweenKeystrokes, kDefaultDelayBetweenKeystrokesMs);
    object[kKeyEmojiLeftDelimiter] = this->readSettings<QString>(kKeyEmojiLeftDelimiter, kDefaultEmojiLeftDelimiter);
    object[kKeyEmojiRightDelimiter] = this->readSettings<QString>(kKeyEmojiRightDelimiter, kDefaultEmojiRightDelimiter);
    object[kKeyEmojiShortcodesEnabled] = this->readSettings<bool>(kKeyEmojiShortcodesEnabled, kDefaultEmojiShortcodesEnabled);
    object[kKeyShowEmojisInPickerWindow] = this->readSettings<bool>(kKeyShowEmojisInPickerWindow, kDefaultShowEmojisInPickerWindow);
    object[kKeyEnableAppEnableDisableShortcut] = this->readSettings<bool>(kKeyEnableAppEnableDisableShortcut, kDefaultEnableAppEnableDisableShortcut);
    object[kKeyGeometry] = QString::fromLocal8Bit(this->readSettings<QByteArray>(kKeyGeometry, QByteArray()).toHex());
    object[kKeyLastComboImportExportPath] = this->readSettings<QString>(kKeyLastComboImportExportPath, QString());
    object[kKeyLastUpdateCheckDateTime] = QString::fromLocal8Bit(variantToByteArray(
        this->lastUpdateCheckDateTime()).toHex());
    object[kKeyLocale] = QString::fromLocal8Bit(variantToByteArray(this->locale()).toHex());
    object[kKeySplitterState] = QString::fromLocal8Bit(this->readSettings<QByteArray>(kKeySplitterState, QByteArray()).toHex());
    object[kKeyPlaySoundOnCombo] = this->readSettings<bool>(kKeyPlaySoundOnCombo, kDefaultPlaySoundOnCombo);
    object[kKeyUseAutomaticSubstitution] = this->readSettings<bool>(kKeyUseAutomaticSubstitution, kDefaultUseAutomaticSubstitution);
    object[kKeyComboTriggersOnSpace] = this->readSettings<bool>(kKeyComboTriggersOnSpace, kDefaultComboTriggersOnSpace);
    object[kKeyUseCustomBackupLocation] = this->readSettings<bool>(kKeyUseCustomSound, kDefaultUseCustomBackupLocation);
    object[kKeyUseCustomSound] = this->readSettings<bool>(kKeyUseCustomSound, kDefaultUseCustomSound);
    object[kKeyUseCustomTheme] = this->readSettings<bool>(kKeyUseCustomTheme, kDefaultUseCustomTheme);
    object[kKeyTheme] = this->readSettings<qint32>(kKeyTheme, static_cast<qint32>(kDefaultTheme));
    object[kKeyWarnAboutShortComboKeyword] = this->readSettings<bool>(kKeyWarnAboutShortComboKeyword, kDefaultWarnAboutShortComboKeyword);
    object[kKeyWriteDebugLogFile] = this->readSettings<bool>(kKeyWriteDebugLogFile, kDefaultWriteDebugLogFile);
    object[kKeyRichTextDeprecationWarningHasAlreadyBeenDisplayed] = this->readSettings<bool>(
        kKeyRichTextDeprecationWarningHasAlreadyBeenDisplayed, kDefaultKeyRichTextDeprecationWarningHasAlreadyBeenDisplayed);
    object[kKeyUseLegacyCopyPaste] = this->readSettings<bool>(kKeyUseLegacyCopyPaste, kDefaultUseLegacyCopyPaste);
    object[kKeyRestoreClipboardAfterSubstitution] = this->readSettings(kKeyRestoreClipboardAfterSubstitution, kDefaultRestoreClipboardAfterSubstitution);
    object[kKeyUseShiftInsertForPasting] = this->readSettings<bool>(kKeyUseShiftInsertForPasting, kDefaultUseShiftInsertForPasting);
    outDoc = QJsonDocument(object);
}


//****************************************************************************************************************************************************
/// \param[in] doc The JSON document.
//****************************************************************************************************************************************************
void PreferencesManager::fromJsonDocument(QJsonDocument const &doc) const {
    QJsonObject const object = doc.object();
    settings_->setValue(kKeyAppEnableShortcutKeyCodeDeprecated, objectValue<quint32>(object, kKeyAppEnableShortcutKeyCodeDeprecated));
    settings_->setValue(kKeyAppEnableShortcutModifiersDeprecated, objectValue<quint32>(object, kKeyAppEnableShortcutModifiersDeprecated));
    settings_->setValue(kKeyAppEnableShortcutScanCodeDeprecated, objectValue<quint32>(object, kKeyAppEnableShortcutScanCodeDeprecated));
    settings_->setValue(kKeyAutoBackup, objectValue<bool>(object, kKeyAutoBackup));
    settings_->setValue(kKeyAutoCheckForUpdates, objectValue<bool>(object, kKeyAutoCheckForUpdates));
    settings_->setValue(kKeyAutoStartAtLogin, objectValue<bool>(object, kKeyAutoStartAtLogin));
    settings_->setValue(kKeyBeeftextEnabled, objectValue<bool>(object, kKeyBeeftextEnabled));
    this->setComboListFolderPath(objectValue<QString>(object, kKeyComboListFolderPath));
    settings_->setValue(kKeyComboPickerEnabled, objectValue<bool>(object, kKeyComboPickerEnabled));
    settings_->setValue(kKeyComboPickerShortcutKeyCodeDeprecated, objectValue<quint32>(object, kKeyComboPickerShortcutKeyCodeDeprecated));
    settings_->setValue(kKeyComboPickerShortcutModifiersDeprecated, objectValue<quint32>(object, kKeyComboPickerShortcutModifiersDeprecated));
    settings_->setValue(kKeyComboPickerShortcutScanCodeDeprecated, objectValue<quint32>(object, kKeyComboPickerShortcutScanCodeDeprecated));
    settings_->setValue(kKeyComboTriggerShortcutKeyCodeDeprecated, objectValue<quint32>(object, kKeyComboTriggerShortcutKeyCodeDeprecated));
    settings_->setValue(kKeyComboTriggerShortcutModifiersDeprecated, objectValue<quint32>(object, kKeyComboTriggerShortcutModifiersDeprecated));
    settings_->setValue(kKeyComboTriggerShortcutScanCodeDeprecated, objectValue<quint32>(object, kKeyComboTriggerShortcutScanCodeDeprecated));
    settings_->setValue(kKeyDefaultMatchingMode, objectValue<qint32>(object, kKeyDefaultMatchingMode));
    settings_->setValue(kKeyDefaultCaseSensitivity, objectValue<qint32>(object, kKeyDefaultCaseSensitivity));
    settings_->setValue(kKeyCustomSoundPath, objectValue<QString>(object, kKeyCustomSoundPath));
    this->setCustomBackupLocation(objectValue<QString>(object, kKeyCustomBackupLocation)); // we call the function because it has side effects
    settings_->setValue(kKeyDelayBetweenKeystrokes, objectValue<qint32>(object, kKeyDelayBetweenKeystrokes));
    settings_->setValue(kKeyEmojiLeftDelimiter, objectValue<QString>(object, kKeyEmojiLeftDelimiter));
    settings_->setValue(kKeyEmojiRightDelimiter, objectValue<QString>(object, kKeyEmojiRightDelimiter));
    settings_->setValue(kKeyEmojiShortcodesEnabled, objectValue<bool>(object, kKeyEmojiShortcodesEnabled));
    settings_->setValue(kKeyShowEmojisInPickerWindow, objectValue<bool>(object, kKeyShowEmojisInPickerWindow));
    settings_->setValue(kKeyEnableAppEnableDisableShortcut, objectValue<bool>(object, kKeyEnableAppEnableDisableShortcut));
    settings_->setValue(kKeyGeometry, QByteArray::fromHex(objectValue<QString>(object, kKeyGeometry).toLocal8Bit()));
    settings_->setValue(kKeyLastComboImportExportPath, objectValue<QString>(object, kKeyLastComboImportExportPath));
    settings_->setValue(kKeyLastUpdateCheckDateTime, byteArrayToVariant<QDateTime>(QByteArray::fromHex(
        objectValue<QString>(object, kKeyLastUpdateCheckDateTime).toLocal8Bit())));
    settings_->setValue(kKeyLocale, byteArrayToVariant<QLocale>(QByteArray::fromHex(objectValue<QString>(object, kKeyLocale).toLocal8Bit())));
    settings_->setValue(kKeyGeometry, QByteArray::fromHex(objectValue<QString>(object, kKeySplitterState).toLocal8Bit()));
    settings_->setValue(kKeyPlaySoundOnCombo, objectValue<bool>(object, kKeyPlaySoundOnCombo));
    settings_->setValue(kKeyUseAutomaticSubstitution, objectValue<bool>(object, kKeyUseAutomaticSubstitution));
    settings_->setValue(kKeyComboTriggersOnSpace, objectValue<bool>(object, kKeyComboTriggersOnSpace));
    this->setUseCustomBackupLocation(objectValue<bool>(object, kKeyUseCustomBackupLocation)); // we call the function because it has side effects
    settings_->setValue(kKeyUseCustomSound, objectValue<bool>(object, kKeyUseCustomSound));
    settings_->setValue(kKeyUseCustomTheme, objectValue<bool>(object, kKeyUseCustomTheme));
    settings_->setValue(kKeyTheme, objectValue<qint32>(object, kKeyTheme));
    settings_->setValue(kKeyWarnAboutShortComboKeyword, objectValue<bool>(object, kKeyWarnAboutShortComboKeyword));
    settings_->setValue(kKeyWriteDebugLogFile, objectValue<bool>(object, kKeyWriteDebugLogFile));
    settings_->setValue(kKeyRichTextDeprecationWarningHasAlreadyBeenDisplayed, objectValue<bool>(object, kKeyRichTextDeprecationWarningHasAlreadyBeenDisplayed));
    this->setUseLegacyCopyPaste(objectValue<bool>(object, kKeyUseLegacyCopyPaste));
    settings_->setValue(kKeyRestoreClipboardAfterSubstitution, objectValue<bool>(object, kKeyRestoreClipboardAfterSubstitution));
    this->setUseShiftInsertForPasting(objectValue<bool>(object, kKeyUseShiftInsertForPasting));
    this->init();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesManager::resetWarnings() const {
    this->setWarnAboutShortComboKeywords(true);
    this->setWarnAboutEmptyComboKeywords(true);
}


//****************************************************************************************************************************************************
/// Set the settings value indicating that the application has been launched in the past
//****************************************************************************************************************************************************
void PreferencesManager::setAlreadyLaunched() const {
    settings_->setValue(kKeyAlreadyLaunched, 1);
}


//****************************************************************************************************************************************************
/// Check whether the application has ever been launched or not
//****************************************************************************************************************************************************
bool PreferencesManager::alreadyLaunched() const {
    return this->readSettings<bool>(kKeyAlreadyLaunched, false);
}


//****************************************************************************************************************************************************
/// \param[in] path The path of the file to delete on next application startup
//****************************************************************************************************************************************************
void PreferencesManager::setFileMarkedForDeletionOnStartup(QString const &path) const {
    settings_->setValue(kKeyFileMarkedForDeletion, path);
}


//****************************************************************************************************************************************************
/// \return The path of the file marked for deletion on next application startup
//****************************************************************************************************************************************************
QString PreferencesManager::fileMarkedForDeletionOnStartup() const {
    return readSettings<QString>(kKeyFileMarkedForDeletion);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PreferencesManager::clearFileMarkedForDeletionOnStartup() const {
    settings_->remove(kKeyFileMarkedForDeletion);
}


//****************************************************************************************************************************************************
/// \param[in] array The geometry as a byte array
//****************************************************************************************************************************************************
void PreferencesManager::setMainWindowGeometry(QByteArray const &array) const {
    settings_->setValue(kKeyGeometry, array);
}


//****************************************************************************************************************************************************
/// \return The geometry of the main window as a byte array
//****************************************************************************************************************************************************
QByteArray PreferencesManager::mainWindowGeometry() const {
    return settings_->value(kKeyGeometry).toByteArray();
}


//****************************************************************************************************************************************************
/// \param[in] array The state as a byte array.
//****************************************************************************************************************************************************
void PreferencesManager::setMainWindowSplitterState(QByteArray const &array) const {
    settings_->setValue(kKeySplitterState, array);
}


//****************************************************************************************************************************************************
/// \return The state as a byte array.
//****************************************************************************************************************************************************
QByteArray PreferencesManager::mainWindowSplitterState() const {
    return settings_->value(kKeySplitterState).toByteArray();
}


//****************************************************************************************************************************************************
/// \return the locale
//****************************************************************************************************************************************************
QLocale PreferencesManager::locale() const {
    return I18nManager::instance().validateLocale(this->readSettings<QLocale>(kKeyLocale, QLocale::system()));
}


//****************************************************************************************************************************************************
/// \param[in] locale The locale
//****************************************************************************************************************************************************
void PreferencesManager::setLocale(QLocale const &locale) const {
    if (this->locale() != locale) {
        settings_->setValue(kKeyLocale, locale);
        this->applyLocalePreference();
    }
}


//****************************************************************************************************************************************************
/// \param[in] dateTime The date/time of the last update check
//****************************************************************************************************************************************************
void PreferencesManager::setLastUpdateCheckDateTime(QDateTime const &dateTime) const {
    settings_->setValue(kKeyLastUpdateCheckDateTime, dateTime);
}


//****************************************************************************************************************************************************
/// \return The date/time of the last update check
//****************************************************************************************************************************************************
QDateTime PreferencesManager::lastUpdateCheckDateTime() const {
    QVariant const v = settings_->value(kKeyLastUpdateCheckDateTime);
    return (v.isNull() || !v.canConvert<QDateTime>()) ? QDateTime() : v.toDateTime();
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference
//****************************************************************************************************************************************************
void PreferencesManager::setAutoStartAtLogin(bool value) const {
    if (isInPortableMode()) {
        globals::debugLog().addWarning("Trying to the set the 'auto start at login' preference while running "
                                       "in portable mode");
        return;
    }
    if (this->autoStartAtLogin() != value) {
        settings_->setValue(kKeyAutoStartAtLogin, value);
        applyAutostartParameters();
    }
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::autoStartAtLogin() const {
    return isInPortableMode() ? false : this->readSettings<bool>(kKeyAutoStartAtLogin, kDefaultAutoStartAtLogin);
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference
//****************************************************************************************************************************************************
void PreferencesManager::setPlaySoundOnCombo(bool value) const {
    settings_->setValue(kKeyPlaySoundOnCombo, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::playSoundOnCombo() const {
    return this->readSettings<bool>(kKeyPlaySoundOnCombo, kDefaultPlaySoundOnCombo);
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preferences.
//****************************************************************************************************************************************************
void PreferencesManager::setUseCustomSound(bool value) const {
    settings_->setValue(kKeyUseCustomSound, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::useCustomSound() const {
    return this->readSettings<bool>(kKeyUseCustomSound, kDefaultUseCustomSound);
}


//****************************************************************************************************************************************************
/// \param[in] path The path of the custom sound file.
//****************************************************************************************************************************************************
void PreferencesManager::setCustomSoundPath(QString const &path) const {
    settings_->setValue(kKeyCustomSoundPath, path);
}


//****************************************************************************************************************************************************
/// \return The path of the custom sound file.
//****************************************************************************************************************************************************
QString PreferencesManager::customSoundPath() const {
    return this->readSettings<QString>(kKeyCustomSoundPath, QString());
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference
//****************************************************************************************************************************************************
void PreferencesManager::setAutoCheckForUpdates(bool value) {
    if (this->autoCheckForUpdates() == value)
        return;
    settings_->setValue(kKeyAutoCheckForUpdates, value);
    emit autoCheckForUpdatesChanged(value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::autoCheckForUpdates() const {
    return this->readSettings<bool>(kKeyAutoCheckForUpdates, kDefaultAutoCheckForUpdates);
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference
//****************************************************************************************************************************************************
void PreferencesManager::setUseCustomTheme(bool value) const {
    if (this->useCustomTheme() != value) {
        settings_->setValue(kKeyUseCustomTheme, value);
        cache_->useCustomTheme = value;
        applyThemePreferences(value, this->theme());
    }
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::useCustomTheme() const {
    return cache_->useCustomTheme;
}


//****************************************************************************************************************************************************
/// As the getter for this value is polled frequently (at every keystroke), it is cached
/// \param[in] value The new value for the preference
//****************************************************************************************************************************************************
void PreferencesManager::setUseAutomaticSubstitution(bool value) const {
    cache_->useAutomaticSubstitution = value;
    settings_->setValue(kKeyUseAutomaticSubstitution, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::useAutomaticSubstitution() const {
    return cache_->useAutomaticSubstitution;
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setComboTriggersOnSpace(bool value) const {
    cache_->comboTriggersOnSpace = value;
    settings_->setValue(kKeyComboTriggersOnSpace, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::comboTriggersOnSpace() const {
    return cache_->comboTriggersOnSpace;
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setKeepFinalSpaceCharacter(bool value) const {
    cache_->keepFinalSpaceCharacter = value;
    settings_->setValue(kKeyKeepFinalSpaceCharacter, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::keepFinalSpaceCharacter() const {
    return cache_->keepFinalSpaceCharacter;
}


//****************************************************************************************************************************************************
/// \param[in] value The new value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setWarnAboutShortComboKeywords(bool value) const {
    settings_->setValue(kKeyWarnAboutShortComboKeyword, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::warnAboutShortComboKeywords() const {
    return this->readSettings<bool>(kKeyWarnAboutShortComboKeyword, kDefaultWarnAboutShortComboKeyword);
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setWarnAboutEmptyComboKeywords(bool value) const {
    settings_->setValue(kKeyWarnAboutEmptyComboKeyword, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//
//****************************************************************************************************************************************************
bool PreferencesManager::warnAboutEmptyComboKeywords() const {
    return this->readSettings<bool>(kKeyWarnAboutEmptyComboKeyword, kDefaultWarnAboutEmptyComboKeyword);
}


//****************************************************************************************************************************************************
/// \param[in] path The path.
/// \return true if and only if the operation was successful.
//****************************************************************************************************************************************************
bool PreferencesManager::setComboListFolderPath(QString const &path) const {
    if (isInPortableMode()) {
        globals::debugLog().addWarning("Trying to the set the 'combo list folder path' preference while running in portable mode");
        return false;
    }
    QString const previousPath = QDir::fromNativeSeparators(comboListFolderPath());
    settings_->setValue(kKeyComboListFolderPath, path);
    if (!ComboManager::instance().saveComboListToFile()) {
        settings_->setValue(kKeyComboListFolderPath, previousPath);
        return false;
    }
    return true;
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
QString PreferencesManager::comboListFolderPath() const {
    return isInPortableMode() ? globals::portableModeDataFolderPath() :
        this->readSettings<QString>(kKeyComboListFolderPath, defaultComboListFolderPath());
}


//****************************************************************************************************************************************************
//// \return The path of the default combo list folder
//****************************************************************************************************************************************************
QString PreferencesManager::defaultComboListFolderPath() {
    return isInPortableMode() ? globals::portableModeDataFolderPath() : globals::appDataDir();
}


//****************************************************************************************************************************************************
/// \param[in] shortcut The shortcut
//****************************************************************************************************************************************************
void PreferencesManager::setComboTriggerShortcut(SpShortcut const &shortcut) const {
    SpShortcut const newShortcut = shortcut ? shortcut : kDefaultComboTriggerShortcut;
    SpShortcut currentShortcut = this->comboTriggerShortcut();
    if (!currentShortcut)
        currentShortcut = kDefaultComboTriggerShortcut;
    if (*newShortcut != *currentShortcut) {
        settings_->setValue(kKeyComboTriggerShortcut, newShortcut->toCombined());
        cache_->comboTriggerShortcut = newShortcut;
    }
}


//****************************************************************************************************************************************************
/// \return The shortcut
//****************************************************************************************************************************************************
SpShortcut PreferencesManager::comboPickerShortcut() const {
    return cache_->comboPickerShortcut;
}


//****************************************************************************************************************************************************
/// \brief Return the default combo picker shortcut.
/// 
/// \return The default combo picker shortcut.
//****************************************************************************************************************************************************
SpShortcut PreferencesManager::defaultComboPickerShortcut() {
    return std::make_shared<Shortcut>(Qt::ControlModifier | Qt::AltModifier | Qt::ShiftModifier, Qt::Key_Enter);
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference
//****************************************************************************************************************************************************
void PreferencesManager::setAutoBackup(bool value) const {
    settings_->setValue(kKeyAutoBackup, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::autoBackup() const {
    return this->readSettings<bool>(kKeyAutoBackup, kDefaultAutoBackup);
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setUseCustomBackupLocation(bool value) const {
    QString const oldPath = globals::backupFolderPath();
    settings_->setValue(kKeyUseCustomBackupLocation, value);
    QString const newPath = globals::backupFolderPath();
    if (QDir(oldPath).canonicalPath() != globals::backupFolderPath())
        BackupManager::moveBackupFolder(oldPath, newPath);
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::useCustomBackupLocation() const {
    return this->readSettings<bool>(kKeyUseCustomBackupLocation, kDefaultUseCustomBackupLocation);
}


//****************************************************************************************************************************************************
/// \param[in] path The path of the custom backup location.
//****************************************************************************************************************************************************
void PreferencesManager::setCustomBackupLocation(QString const &path) const {
    QString const oldPath = globals::backupFolderPath();
    settings_->setValue(kKeyCustomBackupLocation, path);
    QString const newPath = globals::backupFolderPath();
    if (QDir(oldPath).canonicalPath() != globals::backupFolderPath())
        BackupManager::moveBackupFolder(oldPath, newPath);
}


//****************************************************************************************************************************************************
/// \return The custom backup location.
//****************************************************************************************************************************************************
QString PreferencesManager::customBackupLocation() const {
    return this->readSettings<QString>(kKeyCustomBackupLocation, globals::defaultBackupFolderPath());
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference
//****************************************************************************************************************************************************
void PreferencesManager::setWriteDebugLogFile(bool value) {
    bool const currentValue = this->writeDebugLogFile();
    if (value == currentValue)
        return;
    settings_->setValue(kKeyWriteDebugLogFile, value);
    emit writeDebugLogFileChanged(value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::writeDebugLogFile() const {
    return this->readSettings<bool>(kKeyWriteDebugLogFile, kDefaultWriteDebugLogFile);
}


//****************************************************************************************************************************************************
/// \return The preference value
//****************************************************************************************************************************************************
QString PreferencesManager::lastComboImportExportPath() const {
    return this->readSettings<QString>(kKeyLastComboImportExportPath, kDefaultLastComboImportExportPath);
}


//****************************************************************************************************************************************************
/// \param[in] path The preference value
//****************************************************************************************************************************************************
void PreferencesManager::setLastComboImportExportPath(QString const &path) const {
    settings_->setValue(kKeyLastComboImportExportPath, path);
}


//****************************************************************************************************************************************************
/// \return The default combo trigger shortcut
//****************************************************************************************************************************************************
SpShortcut PreferencesManager::defaultComboTriggerShortcut() {
    return kDefaultComboTriggerShortcut;
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::emojiShortcodesEnabled() const {
    return cache_->emojiShortcodesEnabled;
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference
//****************************************************************************************************************************************************
void PreferencesManager::setEmojiShortcodeEnabled(bool value) const {
    cache_->emojiShortcodesEnabled = value;
    settings_->setValue(kKeyEmojiShortcodesEnabled, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
QString PreferencesManager::emojiLeftDelimiter() const {
    QString const result = cache_->emojiLeftDelimiter;
    return result.isEmpty() ? kDefaultEmojiLeftDelimiter : result;
}


//****************************************************************************************************************************************************
/// \param[in] delimiter The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setEmojiLeftDelimiter(QString const &delimiter) const {
    cache_->emojiLeftDelimiter = delimiter;
    settings_->setValue(kKeyEmojiLeftDelimiter, delimiter);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
QString PreferencesManager::emojiRightDelimiter() const {
    QString const result = cache_->emojiRightDelimiter;
    return result.isEmpty() ? kDefaultEmojiRightDelimiter : result;
}


//****************************************************************************************************************************************************
/// \param[in] delimiter The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setEmojiRightDelimiter(QString const &delimiter) const {
    cache_->emojiRightDelimiter = delimiter;
    settings_->setValue(kKeyEmojiRightDelimiter, delimiter);
}


//****************************************************************************************************************************************************
/// \param[in] show The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setShowEmojisInPickerWindow(bool show) const {
    cache_->showEmojisInPickerWindow = show;
    settings_->setValue(kKeyShowEmojisInPickerWindow, show);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::showEmojisInPickerWindow() const {
    return cache_->showEmojisInPickerWindow;
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
qint32 PreferencesManager::delayBetweenKeystrokesMs() const {
    return qBound<qint32>(kMinValueDelayBetweenKeystrokesMs, this->readSettings<qint32>(kKeyDelayBetweenKeystrokes, kDefaultDelayBetweenKeystrokesMs), kMaxValueDelayBetweenKeystrokesMs);
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setDelayBetweenKeystrokesMs(qint32 value) const {
    settings_->setValue(kKeyDelayBetweenKeystrokes, qBound<qint32>(kMinValueDelayBetweenKeystrokesMs, value, kMaxValueDelayBetweenKeystrokesMs));
}


//****************************************************************************************************************************************************
/// \return The minimum value for the 'delay between keystrokes' preferences.
//****************************************************************************************************************************************************
qint32 PreferencesManager::minDelayBetweenKeystrokesMs() {
    return kMinValueDelayBetweenKeystrokesMs;
}


//****************************************************************************************************************************************************
/// \return The maximum value for the 'delay between keystrokes' preferences.
//****************************************************************************************************************************************************
qint32 PreferencesManager::maxDelayBetweenKeystrokesMs() {
    return kMaxValueDelayBetweenKeystrokesMs;
}


//****************************************************************************************************************************************************
/// \return the value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::comboPickerEnabled() const {
    return cache_->comboPickerEnabled;
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setComboPickerEnabled(bool value) const {
    cache_->comboPickerEnabled = value;
    settings_->setValue(kKeyComboPickerEnabled, value);
}


//****************************************************************************************************************************************************
/// \return The trigger shortcut
//****************************************************************************************************************************************************
SpShortcut PreferencesManager::comboTriggerShortcut() const {
    return cache_->comboTriggerShortcut;
}


//****************************************************************************************************************************************************
//\ param[in] mode The default matching mode
//****************************************************************************************************************************************************
void PreferencesManager::setDefaultMatchingMode(EMatchingMode mode) const {
    cache_->defaultMatchingMode = mode;
    settings_->setValue(kKeyDefaultMatchingMode, static_cast<qint32>(mode));
}


//****************************************************************************************************************************************************
/// \return The default matching mode.
//****************************************************************************************************************************************************
EMatchingMode PreferencesManager::defaultMatchingMode() const {
    return cache_->defaultMatchingMode;
}


//****************************************************************************************************************************************************
/// \param[in] sensitivity The default case sensitivity.
//****************************************************************************************************************************************************
void PreferencesManager::setDefaultCaseSensitivity(ECaseSensitivity sensitivity) const {
    cache_->defaultCaseSensitivity = sensitivity;
    settings_->setValue(kKeyDefaultCaseSensitivity, caseSensitivityToInt(sensitivity));
}


//****************************************************************************************************************************************************
/// \return The default case sensitivity.
//****************************************************************************************************************************************************
ECaseSensitivity PreferencesManager::defaultCaseSensitivity() const {
    return cache_->defaultCaseSensitivity;
}


//****************************************************************************************************************************************************
/// \param[in] shortcut The shortcut
//****************************************************************************************************************************************************
void PreferencesManager::setComboPickerShortcut(SpShortcut const &shortcut) const {
    SpShortcut const newShortcut = shortcut ? shortcut : defaultComboPickerShortcut();
    SpShortcut currentShortcut = this->comboPickerShortcut();
    if (!currentShortcut)
        currentShortcut = defaultComboPickerShortcut();
    if (*newShortcut != *currentShortcut) {
        settings_->setValue(kKeyComboPickerShortcut, newShortcut->toCombined());
        cache_->comboPickerShortcut = newShortcut;
    }
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void PreferencesManager::applyLocalePreference() const {
    I18nManager::instance().setLocale(this->locale());
}


//****************************************************************************************************************************************************
/// \param[in] enable The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setEnableAppEnableDisableShortcut(bool enable) const {
    settings_->setValue(kKeyEnableAppEnableDisableShortcut, enable);
    cache_->enableAppEnableDisableShortcut = enable;
}


//****************************************************************************************************************************************************
/// \return The value for the preference
//****************************************************************************************************************************************************
bool PreferencesManager::enableAppEnableDisableShortcut() const {
    return cache_->enableAppEnableDisableShortcut;
}


//****************************************************************************************************************************************************
/// \param[in] shortcut The shortcut.
//****************************************************************************************************************************************************
void PreferencesManager::setAppEnableDisableShortcut(SpShortcut const &shortcut) const {
    SpShortcut const newShortcut = shortcut ? shortcut : kDefaultAppEnableDisableShortcut;
    SpShortcut currentShortcut = this->appEnableDisableShortcut();
    if (!currentShortcut)
        currentShortcut = kDefaultAppEnableDisableShortcut;
    if (*newShortcut != *currentShortcut) {
        settings_->setValue(kKeyAppEnableShortcut, shortcut->toCombined());
        cache_->appEnableDisableShortcut = newShortcut;
    }
}


//****************************************************************************************************************************************************
/// \return The shortcut.
//****************************************************************************************************************************************************
SpShortcut PreferencesManager::appEnableDisableShortcut() const {
    return cache_->appEnableDisableShortcut;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
SpShortcut PreferencesManager::defaultAppEnableDisableShortcut() {
    return kDefaultAppEnableDisableShortcut;
}


//****************************************************************************************************************************************************
/// \param[in] enabled Is Beeftext enabled?
//****************************************************************************************************************************************************
void PreferencesManager::setBeeftextEnabled(bool enabled) const {
    if (cache_->beeftextEnabled == enabled)
        return;
    settings_->setValue(kKeyBeeftextEnabled, enabled);
    cache_->beeftextEnabled = enabled;
}


//****************************************************************************************************************************************************
/// \return true if and only if Beeftext is enabled.
//****************************************************************************************************************************************************
bool PreferencesManager::beeftextEnabled() const {
    return cache_->beeftextEnabled;
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setUseLegacyCopyPaste(bool value) const {
    settings_->setValue(kKeyUseLegacyCopyPaste, value);
    ClipboardManager::setClipboardManagerType(value ? ClipboardManager::EType::Legacy :
        ClipboardManager::EType::Default);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::useLegacyCopyPaste() const {
    return readSettings<bool>(kKeyUseLegacyCopyPaste, kDefaultUseLegacyCopyPaste);
}


//****************************************************************************************************************************************************
// \param[in] value The value for the 'Restore clipboard after substitution' preference.
//****************************************************************************************************************************************************
void PreferencesManager::setRestoreClipboardAfterSubstitution(bool value) {
    settings_->setValue(kKeyRestoreClipboardAfterSubstitution, value);
}


//****************************************************************************************************************************************************
// return The value for the 'Restore clipboard after substitution' preference.
//****************************************************************************************************************************************************
bool PreferencesManager::restoreClipboardAfterSubstitution() const {
    return readSettings<bool>(kKeyRestoreClipboardAfterSubstitution, kDefaultRestoreClipboardAfterSubstitution);
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setAlreadyConvertedRichTextCombos(bool value) const {
    settings_->setValue(kKeyAlreadyConvertedRichTextCombos, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::alreadyConvertedRichTextCombos() const {
    return readSettings<bool>(kKeyAlreadyConvertedRichTextCombos, false);
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference
//****************************************************************************************************************************************************
void PreferencesManager::setUseCustomPowershellVersion(bool value) const {
    settings_->setValue(kKeyUseCustomPowershellVersion, value);
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::useCustomPowershellVersion() const {
    return readSettings<bool>(kKeyUseCustomPowershellVersion, kDefaultUseCustomPowershellVersion);
}


//****************************************************************************************************************************************************
/// \param[in] path The path of the PowerShell executable.
//****************************************************************************************************************************************************
void PreferencesManager::setCustomPowershellPath(QString const &path) const {
    settings_->setValue(kKeyCustomPowershellPath, path);
}


//****************************************************************************************************************************************************
/// \return The path of the custom PowerShell executable.
//****************************************************************************************************************************************************
QString PreferencesManager::customPowershellPath() const {
    return readSettings<QString>(kKeyCustomPowershellPath, QString());
}


//****************************************************************************************************************************************************
/// \param[in] theme The theme.
//****************************************************************************************************************************************************
void PreferencesManager::setTheme(ETheme theme) const {
    settings_->setValue(kKeyTheme, static_cast<qint32>(theme));
    cache_->theme = theme;
    applyThemePreferences(this->useCustomTheme(), theme);
}


//****************************************************************************************************************************************************
/// \return The theme.
//****************************************************************************************************************************************************
ETheme PreferencesManager::theme() const {
    return cache_->theme;
}


//****************************************************************************************************************************************************
/// \param[in] geometry The geometry.
//****************************************************************************************************************************************************
void PreferencesManager::setComboPickerWindowGeometry(QByteArray const &geometry) const {
    settings_->setValue(kKeyComboPickerWindowGeometry, geometry);
}


//****************************************************************************************************************************************************
/// \return The geometry.
//****************************************************************************************************************************************************
QByteArray PreferencesManager::comboPickerWindowGeometry() const {
    return readSettings<QByteArray>(kKeyComboPickerWindowGeometry, QByteArray());
}


//****************************************************************************************************************************************************
/// \param[in] value The value for the preference.
//****************************************************************************************************************************************************
void PreferencesManager::setUseShiftInsertForPasting(bool value) const {
    settings_->setValue(kKeyUseShiftInsertForPasting, value);
    cache_->useShiftInsertForPasting = value;
}


//****************************************************************************************************************************************************
/// \return The value for the preference.
//****************************************************************************************************************************************************
bool PreferencesManager::useShiftInsertForPasting() const {
    return cache_->useShiftInsertForPasting;
}


//****************************************************************************************************************************************************
/// \param[in] versionNumber The version number.
//****************************************************************************************************************************************************
void PreferencesManager::setSkipVersionNumber(VersionNumber const &versionNumber) const {
    settings_->setValue(kKeySkipVersionNumber, versionNumber.toString());
}


//****************************************************************************************************************************************************
/// \return The value for the 'Skip version number' preference.
//****************************************************************************************************************************************************
VersionNumber PreferencesManager::getSkipVersionNumber() const {
    return VersionNumber::fromString(readSettings<QString>(kKeySkipVersionNumber, QString()));
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PreferencesManager::removeSkipVersionNumber() const {
    settings_->remove(kKeySkipVersionNumber);
}


