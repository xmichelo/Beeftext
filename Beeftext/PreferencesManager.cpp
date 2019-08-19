/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of preferences manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "PreferencesManager.h"
#include "I18nManager.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"


namespace {


QString const kKeyAlreadyLaunched = "AlreadyLaunched"; ///< The settings key for the "Already launched" indicator
QString const kKeyFileMarkedForDeletion = "markedForDeletion";
///< The path of the file marked for deletion on next application startup
QString const kKeyGeometry = "Geometry"; ///< The settings key for storing the geometry
QString const kKeySplitterState = "MainWindowSplitterState"; ///< The setting key for storing the main window splitter state.
QString const kKeyLocale = "Locale"; ///< The settings key for the locale
QString const kKeyAppExePath = "AppExePath"; ///< The settings key for the application executable path
QString const kKeyPlaySoundOnCombo = "PlaySoundOnCombo"; ///< The settings key for the 'Play sound on combo' preference
QString const kKeyAutoStartAtLogin = "AutoStartAtLogin"; ///< The settings key for the 'Autostart at login' preference
QString const kKeyAutoCheckForUpdates = "AutoCheckForUpdate";
///< The settings key for the 'Autostart at login' preference
QString const kKeyUseClipboardForComboSubstitution = "UseClipboardForComboSubstitution";
///< The setting key for the 'Use clipboard for combo substitution' preference
QString const kKeyUseCustomTheme = "UseCustomTheme"; ///< The setting key for the 'Use custom theme' preference
QString const kKeyUseAutomaticSubstitution = "UseAutomaticSubstitution";
///< The setting key for the 'Use automatic substitution' preference
QString const kKeyWarnAboutShortComboKeyword = "WarnAboutShortComboKeyword";
///< The setting key for the 'Warn about short combo keyword' preference
QString const kKeyLastUpdateCheckDateTime = "LastUpdateCheck"; ///< The setting key for the last update check date/time
QString const kKeyComboListFolderPath = "ComboListFolderPath"; ///< The setting key for the combo list folder path
QString const kKeyComboTriggerShortcutModifiers = "ComboTriggerShortcutModifiers";
///< The setting key for the combo trigger shortcut modifiers
QString const kKeyComboTriggerShortcutKeyCode = "ComboTriggerShortcutKeyCode";
///< The setting key for the combo trigger shortcut key code
QString const kKeyComboTriggerShortcutScanCode = "ComboTriggerShortcutScanCode";
///< The setting key for the combo trigger shortcut scan code
QString const kKeyComboPickerShortcutModifiers = "ComboPickerShortcutModifiers";
///< The setting key for the combo picker shortcut modifiers
QString const kKeyComboPickerShortcutKeyCode = "ComboPickerShortcutKeyCode";
///< The setting key for the combo picker shortcut key code
QString const kKeyComboPickerShortcutScanCode = "ComboPickerShortcutScanCode";
///< The setting key for the combo picker shortcut scan code
QString const kKeyAutoBackup = "AutoBackup"; ///< The setting key for the 'Auto backup' preference
QString const kKeyWriteDebugLogFile = "WriteDebugLogFile";
///< The setting key for the 'Write debug log file' preference.
QString const kKeyLastComboImportExportPath = "LastComboImportExportPath";
///< The setting key for 'Last combo import/export path' preference
QString const kRegKeyAutoStart = R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run)";
///< The registry key for autostart
QString const kRegKeyEmojiShortcodesEnabled = "EmojiShortcodesEnabled";
///< The setting key for the 'Emoji shortcodes enabled'
QString const kRegKeyEmojiLeftDelimiter = "EmojiLeftDelimiter"; ///< The setting key for the emoji left delimiter.
QString const kRegKeyEmojiRightDelimiter = "EmojiRightDelimiter"; ///< The setting key for the emoji right delimiter.
QString const kRegKeyDelayBetweenKeystrokes = "DelayBetweenKeystrokes";
///< The setting key for the 'Delay between keystrokes'preferences value
QString const kRegKeyComboPickerEnabled = "ComboPickerEnabled";
///< The setting key for the 'Combo picker enabled' preference.
bool const kDefaultValuePlaySoundOnCombo = true; ///< The default value for the 'Play sound on combo' preference
bool const kDefaultValueAutoStartAtLogin = false; ///< The default value for the 'Autostart at login' preference
bool const kDefaultValueAutoCheckForUpdates = true; ///< The default value for the 'Auto check for update preference
bool const kDefaultvalueUseClipboardForComboSubstitution = true;
///< The default value for the 'Use clipboard for combo substitution' preference
bool const kDefaultValueUseCustomTheme = true; ///< The default value for the 'Use custom theme' preference
bool const kDefaultValueUseAutomaticSubstitution = true;
///< The default value for the 'Use automatic substitution' preference
bool const kDefaultValueWarnAboutShortComboKeyword = true;
///< The default value for the 'Warn about short combo keyword' preference
bool const kDefaultValueAutoBackup = true; ///< The default value for the 'Auto backup' preference
bool const kDefaultValueWriteDebugLogFile = true; ///< The default value for the 'Write debug log file' preference
QString const kDefaultValueLastComboImportExportPath = QDir(QStandardPaths::writableLocation(
   QStandardPaths::DesktopLocation)).absoluteFilePath("Combos.json");
///< The default value for the 'Last combo import/export path' preference
SpShortcut const kDefaultValueComboTriggerShortcut = std::make_shared<Shortcut>(Qt::AltModifier | Qt::ShiftModifier
   | Qt::ControlModifier, 'B', 48); ///< The default value for the 'combo trigger shortcut' preference
bool const kDefaultValueEmojiShortcodesEnabled = false;
///< The default value for the 'Emoji shortcodes enabled' preference.
QString const kDefaultValueEmojiLeftDelimiter = "|"; ///< The default left delimiter for emojis
QString const kDefaultValueEmojiRightDelimiter = "|"; ///< The default left delimiter for emojis
qint32 const kDefaultValueDelayBetweenKeystrokesMs = 12;
///< The default valur for the 'Delay between keystrokes' preference.
qint32 const kMinValueDelayBetweenKeystrokesMs = 0;
///< The default valur for the 'Delay between keystrokes' preference.
qint32 const kMaxValueDelayBetweenKeystrokesMs = 500;
///< The default valur for the 'Delay between keystrokes' preference.
bool const kDefaultValueComboPickerEnabled = true; ///< The default value for the 'Combo picker enabled' preference.


}


//**********************************************************************************************************************
/// This function returns the default value if the key does not exist in the settings OR if the value is not of the
/// expected data type.
///
/// \param[in] key The key to read from
/// \param[in] defaultValue The default value to use if the key does not exist or its value is not of the right type
/// \return The read value
//**********************************************************************************************************************
template <typename T>
T PreferencesManager::readSettings(QString const& key, T const& defaultValue) const
{
   if (!settings_->contains(key))
      return defaultValue;
   QVariant v = settings_->value(key, QVariant::fromValue<T>(defaultValue));
   return v.canConvert<T>() ? qvariant_cast<T>(v) : defaultValue;
}


//**********************************************************************************************************************
/// \return The only allowed instance of the class
//**********************************************************************************************************************
PreferencesManager& PreferencesManager::instance()
{
   static PreferencesManager prefManager;
   return prefManager;
}


//**********************************************************************************************************************
/// \note We set the organization and application names manually in case we want to use the preferences before the call 
/// to QApplication::SetOrganizationName() and QApplication::SetApplicationName()
//**********************************************************************************************************************
PreferencesManager::PreferencesManager()
   : settings_(nullptr)
{
   // portable edition use a different storage method for preferences
   settings_ = isInPortableMode()
      ? std::make_unique<QSettings>(globals::portableModeSettingsFilePath(), QSettings::IniFormat)
      : std::make_unique<QSettings>(constants::kOrganizationName, constants::kApplicationName);

   // Cache often accessed values
   cachedUseAutomaticSubstitution_ = this->readSettings<bool>(kKeyUseAutomaticSubstitution,
      kDefaultValueUseAutomaticSubstitution);
   this->cacheComboTriggerShortcut();
   cachedComboPickerEnabled_ = this->readSettings<bool>(kRegKeyComboPickerEnabled, kDefaultValueComboPickerEnabled);
   this->cacheComboPickerShortcut();
   cachedEmojiShortcodesEnabled_ = this->readSettings<bool>(kRegKeyEmojiShortcodesEnabled,
      kDefaultValueEmojiShortcodesEnabled);
   cachedEmojiLeftDelimiter_ = this->readSettings<QString>(kRegKeyEmojiLeftDelimiter, kDefaultValueEmojiLeftDelimiter);
   cachedEmojiRightDelimiter_ = this->readSettings<QString>(kRegKeyEmojiRightDelimiter,
      kDefaultValueEmojiRightDelimiter);
   // Some preferences setting need initialization
   this->applyCustomThemePreference();
   this->applyLocalePreference();
   this->applyAutoStartPreference();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesManager::reset()
{
   this->setPlaySoundOnCombo(kDefaultValuePlaySoundOnCombo);
   this->setAutoCheckForUpdates(kDefaultValueAutoCheckForUpdates);
   this->setUseClipboardForComboSubstitution(kDefaultvalueUseClipboardForComboSubstitution);
   this->setUseCustomTheme(kDefaultValueUseCustomTheme);
   this->setUseAutomaticSubstitution(kDefaultValueUseAutomaticSubstitution);
   this->setWarnAboutShortComboKeywords(kDefaultValueWarnAboutShortComboKeyword);
   this->setComboPickerEnabled(kDefaultValueComboPickerEnabled);
   this->setComboTriggerShortcut(kDefaultValueComboTriggerShortcut);
   this->setComboPickerShortcut(defaultComboPickerShortcut());
   this->setAutoBackup(kDefaultValueAutoBackup);
   this->setEmojiShortcodeEnabled(kDefaultValueEmojiShortcodesEnabled);
   this->setEmojiLeftDelimiter(kDefaultValueEmojiLeftDelimiter);
   this->setEmojiRightDelimiter(kDefaultValueEmojiRightDelimiter);
   this->setLocale(I18nManager::instance().validateLocale(QLocale::system()));
   this->setDelayBetweenKeystrokesMs(kDefaultValueDelayBetweenKeystrokesMs);
   this->setWriteDebugLogFile(kDefaultValueWriteDebugLogFile);
   this->resetWarnings();
   if (!isInPortableMode())
   {
      this->setAutoStartAtLogin(kDefaultValueAutoStartAtLogin);
      this->setComboListFolderPath(globals::appDataDir());
   }
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesManager::resetWarnings() const
{
   this->setWarnAboutShortComboKeywords(true);
}


//**********************************************************************************************************************
/// The value for this preference is set by the NSIS installer. 
//**********************************************************************************************************************
QString PreferencesManager::getInstalledApplicationPath() const
{
   if (!settings_->contains(kKeyAppExePath))
      return QString();
   return QDir::fromNativeSeparators(this->readSettings<QString>(kKeyAppExePath));
}


//**********************************************************************************************************************
/// Set the settings value indicating that the application has been launched in the past
//**********************************************************************************************************************
void PreferencesManager::setAlreadyLaunched() const
{
   settings_->setValue(kKeyAlreadyLaunched, 1);
}


//**********************************************************************************************************************
/// Check whether the application has ever been launched or not
//**********************************************************************************************************************
bool PreferencesManager::alreadyLaunched() const
{
   return this->readSettings<bool>(kKeyAlreadyLaunched, false);
}


//**********************************************************************************************************************
/// \param[in] path The path of the file to delete on next application startup
//**********************************************************************************************************************
void PreferencesManager::setFileMarkedForDeletionOnStartup(QString const& path) const
{
   settings_->setValue(kKeyFileMarkedForDeletion, path);
}


//**********************************************************************************************************************
/// \return The path of the file marked for deletion on next application startup
//**********************************************************************************************************************
QString PreferencesManager::fileMarkedForDeletionOnStartup() const
{
   return readSettings<QString>(kKeyFileMarkedForDeletion);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesManager::clearFileMarkedForDeletionOnStartup() const
{
   settings_->remove(kKeyFileMarkedForDeletion);
}


//**********************************************************************************************************************
/// \param[in] array The geometry as a byte array
//**********************************************************************************************************************
void PreferencesManager::setMainWindowGeometry(QByteArray const& array) const
{
   settings_->setValue(kKeyGeometry, array);
}


//**********************************************************************************************************************
/// \return The geometry of the main window as a byte array
//**********************************************************************************************************************
QByteArray PreferencesManager::mainWindowGeometry() const
{
   return settings_->value(kKeyGeometry).toByteArray();
}


//**********************************************************************************************************************
/// \param[in] array The state as a byte array.
//**********************************************************************************************************************
void PreferencesManager::setMainWindowSplitterState(QByteArray const& array) const
{
   settings_->setValue(kKeySplitterState, array);
}


//**********************************************************************************************************************
/// \return The state as a byte array.
//**********************************************************************************************************************
QByteArray PreferencesManager::mainWindowSplitterState() const
{
   return settings_->value(kKeySplitterState).toByteArray();
}


//**********************************************************************************************************************
/// \return the locale
//**********************************************************************************************************************
QLocale PreferencesManager::locale() const
{
   return I18nManager::instance().validateLocale(this->readSettings<QLocale>(kKeyLocale, QLocale::system()));
}


//**********************************************************************************************************************
/// \param[in] locale The locale
//**********************************************************************************************************************
void PreferencesManager::setLocale(QLocale const& locale) const
{
   if (this->locale() != locale)
   {
      settings_->setValue(kKeyLocale, locale);
      this->applyLocalePreference();
   }
}


//**********************************************************************************************************************
/// \param[in] dateTime The date/time of the last update check
//**********************************************************************************************************************
void PreferencesManager::setLastUpdateCheckDateTime(QDateTime const& dateTime) const
{
   settings_->setValue(kKeyLastUpdateCheckDateTime, dateTime);
}


//**********************************************************************************************************************
/// \return The date/time of the last update check
//**********************************************************************************************************************
QDateTime PreferencesManager::lastUpdateCheckDateTime() const
{
   QVariant const v = settings_->value(kKeyLastUpdateCheckDateTime);
   return (v.isNull() || !v.canConvert<QDateTime>()) ? QDateTime() : v.toDateTime();
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setAutoStartAtLogin(bool value) const
{
   if (isInPortableMode())
   {
      globals::debugLog().addWarning("Trying to the set the 'auto start at login' preference while running "
         "in portable mode");
      return;
   }
   if (this->autoStartAtLogin() != value)
   {
      settings_->setValue(kKeyAutoStartAtLogin, value);
      this->applyAutoStartPreference();
   }
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::autoStartAtLogin() const
{
   return isInPortableMode() ? false : this->readSettings<bool>(kKeyAutoStartAtLogin, kDefaultValueAutoStartAtLogin);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setPlaySoundOnCombo(bool value) const
{
   settings_->setValue(kKeyPlaySoundOnCombo, value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::playSoundOnCombo() const
{
   return this->readSettings<bool>(kKeyPlaySoundOnCombo, kDefaultValuePlaySoundOnCombo);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setAutoCheckForUpdates(bool value)
{
   if (this->autoCheckForUpdates() == value)
      return;
   settings_->setValue(kKeyAutoCheckForUpdates, value);
   emit autoCheckForUpdatesChanged(value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::autoCheckForUpdates() const
{
   return this->readSettings<bool>(kKeyAutoCheckForUpdates, kDefaultValueAutoCheckForUpdates);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setUseClipboardForComboSubstitution(bool value) const
{
   settings_->setValue(kKeyUseClipboardForComboSubstitution, value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::useClipboardForComboSubstitution() const
{
   return this->readSettings<bool>(kKeyUseClipboardForComboSubstitution, kDefaultvalueUseClipboardForComboSubstitution);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setUseCustomTheme(bool value) const
{
   if (this->useCustomTheme() != value)
   {
      settings_->setValue(kKeyUseCustomTheme, value);
      this->applyCustomThemePreference();
   }
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::useCustomTheme() const
{
   return this->readSettings<bool>(kKeyUseCustomTheme, kDefaultValueUseCustomTheme);
}


//**********************************************************************************************************************
/// As the getter for this value is polled frequently (at every keystroke), it is cached
/// \param[in] value The new value for the preference
//**********************************************************************************************************************
void PreferencesManager::setUseAutomaticSubstitution(bool value)
{
   cachedUseAutomaticSubstitution_ = value;
   settings_->setValue(kKeyUseAutomaticSubstitution, value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::useAutomaticSubstitution() const
{
   return cachedUseAutomaticSubstitution_;
}


//**********************************************************************************************************************
/// \param[in] value The new value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setWarnAboutShortComboKeywords(bool value) const
{
   settings_->setValue(kKeyWarnAboutShortComboKeyword, value);
}


//**********************************************************************************************************************
/// \return The value for the preference.
//**********************************************************************************************************************
bool PreferencesManager::warnAboutShortComboKeywords() const
{
   return this->readSettings<bool>(kKeyWarnAboutShortComboKeyword, kDefaultValueWarnAboutShortComboKeyword);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setComboListFolderPath(QString const& value) const
{
   if (isInPortableMode())
      globals::debugLog().addWarning("Trying to the set the 'combo list folder path' preference while running "
         "in portable mode");
   else
      settings_->setValue(kKeyComboListFolderPath, value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
QString PreferencesManager::comboListFolderPath() const
{
   return isInPortableMode() ? globals::portableModeDataFolderPath() :
      this->readSettings<QString>(kKeyComboListFolderPath, defaultComboListFolderPath());
}


//**********************************************************************************************************************
//// \return The path of the default combo list folder
//**********************************************************************************************************************
QString PreferencesManager::defaultComboListFolderPath()
{
   return isInPortableMode() ? globals::portableModeDataFolderPath() : globals::appDataDir();
}


//**********************************************************************************************************************
/// \param[in] shortcut The shortcut
//**********************************************************************************************************************
void PreferencesManager::setComboTriggerShortcut(SpShortcut const& shortcut)
{
   SpShortcut const newShortcut = shortcut ? shortcut : kDefaultValueComboTriggerShortcut;
   SpShortcut currentShortcut = this->comboTriggerShortcut();
   if (!currentShortcut)
      currentShortcut = kDefaultValueComboTriggerShortcut;
   if (*newShortcut != *currentShortcut)
   {
      settings_->setValue(kKeyComboTriggerShortcutModifiers, int(shortcut->nativeModifiers()));
      settings_->setValue(kKeyComboTriggerShortcutKeyCode, shortcut->nativeVirtualKey());
      settings_->setValue(kKeyComboTriggerShortcutScanCode, shortcut->nativeScanCode());
      cachedComboTriggerShortcut_ = newShortcut;
   }
}


//**********************************************************************************************************************
/// \return The shortcut
//**********************************************************************************************************************
SpShortcut PreferencesManager::comboPickerShortcut() const
{
   return cachedComboPickerShortcut_;
}


//**********************************************************************************************************************
/// \brief Return the default combo picker shortcut.
/// 
/// The shortcut is different on Windows 7 because Win+Shift is reserver for language switching on this platform.
/// 
/// \return The default combo picker shortcut.
//**********************************************************************************************************************
SpShortcut PreferencesManager::defaultComboPickerShortcut()
{
   QString const pType = QSysInfo::productType();
   QString const pVersion = QSysInfo::productVersion();
   if ((0 == pType.compare("windows", Qt::CaseInsensitive)) && (pVersion.startsWith('7'))) // Windows 7
      return std::make_shared<Shortcut>(Qt::MetaModifier | Qt::ControlModifier, 'B', 48);
   else
      return std::make_shared<Shortcut>(Qt::MetaModifier | Qt::ShiftModifier, 13 /*Enter*/, 28);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setAutoBackup(bool value) const
{
   settings_->setValue(kKeyAutoBackup, value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::autoBackup() const
{
   return this->readSettings<bool>(kKeyAutoBackup, kDefaultValueAutoBackup);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setWriteDebugLogFile(bool value)
{
   bool const currentValue = this->writeDebugLogFile();
   if (value == currentValue)
      return;
   settings_->setValue(kKeyWriteDebugLogFile, value);
   emit writeDebugLogFileChanged(value);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::writeDebugLogFile() const
{
   return this->readSettings<bool>(kKeyWriteDebugLogFile, kDefaultValueWriteDebugLogFile);
}


//**********************************************************************************************************************
/// \return The preference value
//**********************************************************************************************************************
QString PreferencesManager::lastComboImportExportPath() const
{
   return this->readSettings<QString>(kKeyLastComboImportExportPath, kDefaultValueLastComboImportExportPath);
}


//**********************************************************************************************************************
/// \param[in] path The preference value
//**********************************************************************************************************************
void PreferencesManager::setLastComboImportExportPath(QString const& path) const
{
   settings_->setValue(kKeyLastComboImportExportPath, path);
}


//**********************************************************************************************************************
/// \return The default combo trigger shortcut
//**********************************************************************************************************************
SpShortcut PreferencesManager::defaultComboTriggerShortcut()
{
   return kDefaultValueComboTriggerShortcut;
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::emojiShortcodesEnabled() const
{
   return cachedEmojiShortcodesEnabled_;
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference
//**********************************************************************************************************************
void PreferencesManager::setEmojiShortcodeEnabled(bool value)
{
   cachedEmojiShortcodesEnabled_ = value;
   settings_->setValue(kRegKeyEmojiShortcodesEnabled, value);
}


//**********************************************************************************************************************
/// \return The value for the preference.
//**********************************************************************************************************************
QString PreferencesManager::emojiLeftDelimiter() const
{
   QString const result = cachedEmojiLeftDelimiter_;
   return result.isEmpty() ? kDefaultValueEmojiLeftDelimiter : result;
}


//**********************************************************************************************************************
/// \param[in] delimiter The value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setEmojiLeftDelimiter(QString const& delimiter)
{
   cachedEmojiLeftDelimiter_ = delimiter;
   settings_->setValue(kRegKeyEmojiLeftDelimiter, delimiter);
}


//**********************************************************************************************************************
/// \return The value for the preference.
//**********************************************************************************************************************
QString PreferencesManager::emojiRightDelimiter() const
{
   QString const result = cachedEmojiRightDelimiter_;
   return result.isEmpty() ? kDefaultValueEmojiRightDelimiter : result;
}


//**********************************************************************************************************************
/// \param[in] delimiter The value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setEmojiRightDelimiter(QString const& delimiter)
{
   cachedEmojiRightDelimiter_ = delimiter;
   settings_->setValue(kRegKeyEmojiRightDelimiter, delimiter);
}


//**********************************************************************************************************************
/// \return The value for the preference.
//**********************************************************************************************************************
qint32 PreferencesManager::delayBetweenKeystrokesMs() const
{
   return qBound<qint32>(kMinValueDelayBetweenKeystrokesMs, this->readSettings<qint32>(kRegKeyDelayBetweenKeystrokes,
      kDefaultValueDelayBetweenKeystrokesMs), kMaxValueDelayBetweenKeystrokesMs);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setDelayBetweenKeystrokesMs(qint32 value) const
{
   settings_->setValue(kRegKeyDelayBetweenKeystrokes, qBound<qint32>(kMinValueDelayBetweenKeystrokesMs, value,
      kMaxValueDelayBetweenKeystrokesMs));
}


//**********************************************************************************************************************
/// \return The minimum value for the 'delay between keystrokes' preferences.
//**********************************************************************************************************************
qint32 PreferencesManager::minDelayBetweenKeystrokesMs()
{
   return kMinValueDelayBetweenKeystrokesMs;
}


//**********************************************************************************************************************
/// \return The maximum value for the 'delay between keystrokes' preferences.
//**********************************************************************************************************************
qint32 PreferencesManager::maxDelayBetweenKeystrokesMs()
{
   return kMaxValueDelayBetweenKeystrokesMs;
}


//**********************************************************************************************************************
/// \return the value for the preference.
//**********************************************************************************************************************
bool PreferencesManager::comboPickerEnabled() const
{
   return cachedComboPickerEnabled_;
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setComboPickerEnabled(bool value)
{
   cachedComboPickerEnabled_ = value;
   settings_->setValue(kRegKeyComboPickerEnabled, value);
}


//**********************************************************************************************************************
/// \return The trigger shortcut
//**********************************************************************************************************************
SpShortcut PreferencesManager::comboTriggerShortcut() const
{
   return cachedComboTriggerShortcut_;
}


//**********************************************************************************************************************
/// \param[in] shortcut The shortcut
//**********************************************************************************************************************
void PreferencesManager::setComboPickerShortcut(SpShortcut const& shortcut)
{
   SpShortcut const newShortcut = shortcut ? shortcut : defaultComboPickerShortcut();
   SpShortcut currentShortcut = this->comboPickerShortcut();
   if (!currentShortcut)
      currentShortcut = defaultComboPickerShortcut();
   if (*newShortcut != *currentShortcut)
   {
      settings_->setValue(kKeyComboPickerShortcutModifiers, int(shortcut->nativeModifiers()));
      settings_->setValue(kKeyComboPickerShortcutKeyCode, shortcut->nativeVirtualKey());
      settings_->setValue(kKeyComboPickerShortcutScanCode, shortcut->nativeScanCode());
      cachedComboPickerShortcut_ = newShortcut;
   }
}


//**********************************************************************************************************************
/// \param[in] modRegKey The registry key for the shortcut's  modifiers.
/// \param[in] vKeyRegKey The registry key for the shortcut's virtual key.
/// \param[in] scanCodeRegKey The registry key for the shortcut's scan code.
/// \return The shortcut, or a null pointer if not found
//**********************************************************************************************************************
SpShortcut PreferencesManager::readShortcutFromPreferences(QString const& modRegKey, QString const& vKeyRegKey,
   QString const& scanCodeRegKey) const
{
   int const intMods = this->readSettings<quint32>(modRegKey, 0);
   quint32 const vKey = this->readSettings<quint32>(vKeyRegKey, 0);
   quint32 const scanCode = this->readSettings<quint32>(scanCodeRegKey, 0);
   if ((!intMods) || (!vKey) || (!scanCode))
      return nullptr;
   SpShortcut const result = std::make_shared<Shortcut>(Qt::KeyboardModifiers(intMods), vKey, scanCode);
   return result->isValid() ? result : nullptr;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesManager::cacheComboTriggerShortcut()
{
   SpShortcut const shortcut = this->readShortcutFromPreferences(kKeyComboTriggerShortcutModifiers,
      kKeyComboTriggerShortcutKeyCode, kKeyComboTriggerShortcutScanCode);
   cachedComboTriggerShortcut_ = shortcut ? shortcut : kDefaultValueComboTriggerShortcut;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesManager::cacheComboPickerShortcut()
{
   SpShortcut const shortcut = this->readShortcutFromPreferences(kKeyComboPickerShortcutModifiers,
      kKeyComboPickerShortcutKeyCode, kKeyComboPickerShortcutScanCode);
   cachedComboPickerShortcut_ = shortcut ? shortcut : defaultComboPickerShortcut();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesManager::applyCustomThemePreference() const
{
   dynamic_cast<QApplication *>(QCoreApplication::instance())->setStyleSheet(this->useCustomTheme()
      ? constants::kStyleSheet : QString());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesManager::applyAutoStartPreference() const
{
   if (isInPortableMode())
      return;
   if (this->autoStartAtLogin())
   {
      if (!registerApplicationForAutoStart())
         globals::debugLog().addWarning("Could not register the application for automatic startup on login.");
   }
   else
      unregisterApplicationFromAutoStart();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesManager::applyLocalePreference() const
{
   I18nManager::instance().setLocale(this->locale());
}


//**********************************************************************************************************************
/// To register the application for auto start at login, we use a registry key that contains the path of the installed
/// application. This key is written by the NSIS installer script. As a consequence, if the application has not be
/// installed using the installer (for example on a development system), this function will fail
///
/// \return true if the operation was successful
//**********************************************************************************************************************
bool PreferencesManager::registerApplicationForAutoStart() const
{
   if (isInPortableMode())
      return false;
   QString const installedPath = this->getInstalledApplicationPath();
   if (installedPath.isEmpty() || (!QFileInfo(installedPath).exists()))
      return false;

   QSettings(kRegKeyAutoStart, QSettings::NativeFormat).setValue(constants::kApplicationName,
      QDir::toNativeSeparators(installedPath));
   return true;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesManager::unregisterApplicationFromAutoStart()
{
   if (!isInPortableMode())
      QSettings(kRegKeyAutoStart, QSettings::NativeFormat).remove(constants::kApplicationName);
}
