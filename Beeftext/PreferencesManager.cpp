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
#include "Combo/ComboManager.h"
#include "Backup/BackupManager.h"
#include "BeeftextUtils.h"
#include "BeeftextGlobals.h"
#include "BeeftextConstants.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {


QString const kKeyAlreadyLaunched = "AlreadyLaunched"; ///< The settings key for the "Already launched" indicator
QString const kKeyAppEnableShortcutKeyCode = "AppEnableDisableShortcutKeyCode"; ///< The setting key for the app enable/disable shortcut key code.
QString const kKeyAppEnableShortcutModifiers = "AppEnableDisableShortcutModifiers"; ///< The setting key for the app enable/disable shortcut modifiers.
QString const kKeyAppEnableShortcutScanCode = "AppEnableDisableShortcutScanCode"; ///< The setting key for the app enable/disable shortcut scan code.
QString const kKeyAppExePath = "AppExePath"; ///< The settings key for the application executable path
QString const kKeyAutoBackup = "AutoBackup"; ///< The setting key for the 'Auto backup' preference
QString const kKeyAutoCheckForUpdates = "AutoCheckForUpdate"; ///< The settings key for the 'Autostart at login' preference
QString const kKeyAutoStart = R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run)"; ///< The registry key for autostart
QString const kKeyAutoStartAtLogin = "AutoStartAtLogin"; ///< The settings key for the 'Autostart at login' preference
QString const kKeyBeeftextEnabled = "BeefextEnabled"; ///< The setting key for the 'Beeftext enabled' preference.
QString const kKeyComboListFolderPath = "ComboListFolderPath"; ///< The setting key for the combo list folder path
QString const kKeyComboPickerEnabled = "ComboPickerEnabled"; ///< The setting key for the 'Combo picker enabled' preference.
QString const kKeyComboPickerShortcutModifiers = "ComboPickerShortcutModifiers"; ///< The setting key for the combo picker shortcut modifiers
QString const kKeyComboPickerShortcutKeyCode = "ComboPickerShortcutKeyCode"; ///< The setting key for the combo picker shortcut key code
QString const kKeyComboPickerShortcutScanCode = "ComboPickerShortcutScanCode"; ///< The setting key for the combo picker shortcut scan code
QString const kKeyComboTriggerShortcutModifiers = "ComboTriggerShortcutModifiers"; ///< The setting key for the combo trigger shortcut modifiers
QString const kKeyComboTriggerShortcutKeyCode = "ComboTriggerShortcutKeyCode"; ///< The setting key for the combo trigger shortcut key code
QString const kKeyComboTriggerShortcutScanCode = "ComboTriggerShortcutScanCode"; ///< The setting key for the combo trigger shortcut scan code
QString const kKeyCustomBackupLocation = "CustomBackupLocation"; ///< The settings key for the 'Custom backup location' preference.
QString const kKeyCustomSoundPath = "CustomSoundPath"; ///< The settings key for the 'Custom sound path' preference.
QString const kKeyDelayBetweenKeystrokes = "DelayBetweenKeystrokes"; ///< The setting key for the 'Delay between keystrokes'preferences value
QString const kKeyEmojiLeftDelimiter = "EmojiLeftDelimiter"; ///< The setting key for the emoji left delimiter.
QString const kKeyEmojiRightDelimiter = "EmojiRightDelimiter"; ///< The setting key for the emoji right delimiter.
QString const kKeyEmojiShortcodesEnabled = "EmojiShortcodesEnabled"; ///< The setting key for the 'Emoji shortcodes enabled'
QString const kKeyEnableAppEnableDisableShortcut ="EnableAppEnableDisableShortcut"; ///< The setting key for the 'enable app enable/disable shortcut' preference.
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
QString const kKeyWriteDebugLogFile = "WriteDebugLogFile"; ///< The setting key for the 'Write debug log file' preference.


SpShortcut const kDefaultAppEnableDisableShortcut = std::make_shared<Shortcut>(Qt::AltModifier | Qt::ShiftModifier
   | Qt::ControlModifier,'V', 0x2f); ///< The default value for the 'combo trigger shortcut' preference
bool const kDefaultAutoBackup = true; ///< The default value for the 'Auto backup' preference
bool const kDefaultAutoCheckForUpdates = true; ///< The default value for the 'Auto check for update preference
bool const kDefaultAutoStartAtLogin = false; ///< The default value for the 'Autostart at login' preference
bool const kDefaultBeeftextEnabled = true; ///< The default value for the 'Beeftext is enabled' preference.
bool const kDefaultComboPickerEnabled = true; ///< The default value for the 'Combo picker enabled' preference.
SpShortcut const kDefaultComboTriggerShortcut = std::make_shared<Shortcut>(Qt::AltModifier | Qt::ShiftModifier
   | Qt::ControlModifier, 'B', 0x30); ///< The default value for the 'combo trigger shortcut' preference
qint32 const kDefaultDelayBetweenKeystrokesMs = 12; ///< The default valur for the 'Delay between keystrokes' preference.
QString const kDefaultEmojiLeftDelimiter = "|"; ///< The default left delimiter for emojis
QString const kDefaultEmojiRightDelimiter = "|"; ///< The default left delimiter for emojis
bool const kDefaultEmojiShortcodesEnabled = false; ///< The default value for the 'Emoji shortcodes enabled' preference.
bool const kDefaultEnableAppEnableDisableShortcut = false; ///< The default value for the 'enable app enable/disable shortcut' preference.
QString const kDefaultLastComboImportExportPath = QDir(QStandardPaths::writableLocation(
   QStandardPaths::DesktopLocation)).absoluteFilePath("Combos.json");///< The default value for the 'Last combo import/export path' preference
qint32 const kMinValueDelayBetweenKeystrokesMs = 0; ///< The default valur for the 'Delay between keystrokes' preference.
qint32 const kMaxValueDelayBetweenKeystrokesMs = 500; ///< The default valur for the 'Delay between keystrokes' preference.
bool const kDefaultPlaySoundOnCombo = true; ///< The default value for the 'Play sound on combo' preference
bool const kDefaultUseAutomaticSubstitution = true; ///< The default value for the 'Use automatic substitution' preference
bool const kDefaultUseCustomBackupLocation = false; ///< The default value for the 'Use custom backup location' preference.
bool const kDefaultUseCustomSound = false; ///< The default value for the 'Use custom sound' preference.
bool const kDefaultUseCustomTheme = true; ///< The default value for the 'Use custom theme' preference
bool const kDefaultWarnAboutShortComboKeyword = true; ///< The default value for the 'Warn about short combo keyword' preference
bool const kDefaultWriteDebugLogFile = true; ///< The default value for the 'Write debug log file' preference


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
{
   // portable edition use a different storage method for preferences
   settings_ = isInPortableMode()
      ? std::make_unique<QSettings>(globals::portableModeSettingsFilePath(), QSettings::IniFormat)
      : std::make_unique<QSettings>(constants::kOrganizationName, constants::kApplicationName);
   this->init();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesManager::init()
{
   // Cache often accessed values
   cachedUseAutomaticSubstitution_ = this->readSettings<bool>(kKeyUseAutomaticSubstitution,
      kDefaultUseAutomaticSubstitution);
   this->cacheComboTriggerShortcut();
   cachedComboPickerEnabled_ = this->readSettings<bool>(kKeyComboPickerEnabled, kDefaultComboPickerEnabled);
   this->cacheComboPickerShortcut();
   cachedEmojiShortcodesEnabled_ = this->readSettings<bool>(kKeyEmojiShortcodesEnabled,
      kDefaultEmojiShortcodesEnabled);
   cachedEnableAppEnableDisableShortcut_ = this->readSettings<bool>(kKeyEnableAppEnableDisableShortcut, 
      kDefaultEnableAppEnableDisableShortcut);
   this->cacheAppEnableDisableShortcut();
   cachedEmojiLeftDelimiter_ = this->readSettings<QString>(kKeyEmojiLeftDelimiter, kDefaultEmojiLeftDelimiter);
   cachedEmojiRightDelimiter_ = this->readSettings<QString>(kKeyEmojiRightDelimiter,
      kDefaultEmojiRightDelimiter);
   cachedBeeftextEnabled_ = this->readSettings<bool>(kKeyBeeftextEnabled, kDefaultBeeftextEnabled);
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
   this->setAppEnableDisableShortcut(defaultAppEnableDisableShortcut());
   this->setAutoBackup(kDefaultAutoBackup);
   this->setAutoCheckForUpdates(kDefaultAutoCheckForUpdates);
   this->setComboPickerEnabled(kDefaultComboPickerEnabled);
   this->setComboPickerShortcut(defaultComboPickerShortcut());
   this->setComboTriggerShortcut(kDefaultComboTriggerShortcut);
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
   this->setUseCustomBackupLocation(kDefaultUseCustomBackupLocation);
   this->setUseCustomTheme(kDefaultUseCustomTheme);
   this->setUseCustomSound(kDefaultUseCustomSound);
   this->setWarnAboutShortComboKeywords(kDefaultWarnAboutShortComboKeyword);
   this->setWriteDebugLogFile(kDefaultWriteDebugLogFile);

   this->resetWarnings();
   if (!isInPortableMode())
   {
      this->setAutoStartAtLogin(kDefaultAutoStartAtLogin);
      this->setComboListFolderPath(globals::appDataDir());
   }
}


//**********************************************************************************************************************
/// \param[in] path The path of the file to save to.
/// \return true if and only if the operation was completed successfully.
//**********************************************************************************************************************
bool PreferencesManager::save(QString const& path) const
{
   try
   {
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
   catch (Exception const& e)
   {
      globals::debugLog().addError(e.qwhat());
      return false;
   }
}


//**********************************************************************************************************************
/// \param[in] path The path of the file to load from.
/// \return true if and only if the operation was completed successfully.
//**********************************************************************************************************************
bool PreferencesManager::load(QString const& path)
{
   try
   {
      QFile file(path);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         throw Exception("The output file could not be opened.");
      QJsonDocument const doc = QJsonDocument::fromJson(file.readAll());
      if (doc.isNull())
         throw Exception("The input file is not a valid JSON file.");
      this->fromJsonDocument(doc);
      return true;
   }
   catch (Exception const& e)
   {
      globals::debugLog().addError(e.qwhat());
      return false;
   }
}


//**********************************************************************************************************************
/// \param[in] object The JSON object.
/// \param[in] key The key.
/// \return The value associated with the key
//**********************************************************************************************************************
template <typename T> T objectValue(QJsonObject const& object, QString const& key)
{
   if (!object.contains(key))
      throw Exception(QString("Could not find entry %1 in imported preferences file.").arg(key));
   QVariant const v = object[key].toVariant();
   if (!v.canConvert<T>())
      throw Exception(QString("The value for entry %1 in imported preferences file is invalid.").arg(key));
   return v.value<T>();
}


//**********************************************************************************************************************
/// \brief Return a serilizable array containing a QVariant.
///
/// \param[in] v The QVariant.
/// \return a serilizable array containing a QVariant.
//**********************************************************************************************************************
QByteArray variantToByteArray(QVariant const& v)
{
   QByteArray data;
   QDataStream stream(&data, QIODevice::WriteOnly);
   stream << v;
   return data;
}


//**********************************************************************************************************************
/// \brief Deserialize a QVariant from a byte array
///
/// \param[in] data The byte array.
/// \return The deserialized QVariant
//**********************************************************************************************************************
template <typename T> T byteArrayToVariant(QByteArray const& data)
{
   QByteArray array(data);
   QDataStream stream(&array, QIODevice::ReadOnly);
   QVariant v;
   stream >> v;
   if (!v.canConvert<T>())
      throw Exception("The imported file is invalid.");
   return v.value<T>();
}

//**********************************************************************************************************************
/// \param[out] outDoc The JSON document.
//**********************************************************************************************************************
void PreferencesManager::toJsonDocument(QJsonDocument& outDoc) const
{
   QJsonObject object;
   object[kKeyAppEnableShortcutKeyCode] = qint32(this->readSettings<quint32>(kKeyAppEnableShortcutKeyCode, 0));
   object[kKeyAppEnableShortcutModifiers] = qint32(this->readSettings<quint32>(kKeyAppEnableShortcutModifiers, 0));
   object[kKeyAppEnableShortcutScanCode] = qint32(this->readSettings<quint32>(kKeyAppEnableShortcutScanCode, 0));
   object[kKeyAutoBackup] = this->readSettings<bool>(kKeyAutoBackup, kDefaultAutoBackup);
   object[kKeyAutoCheckForUpdates] = this->readSettings<bool>(kKeyAutoCheckForUpdates, kDefaultAutoCheckForUpdates);
   object[kKeyAutoStartAtLogin] = this->readSettings<bool>(kKeyAutoStartAtLogin, kDefaultAutoStartAtLogin);
   object[kKeyBeeftextEnabled] = this->readSettings<bool>(kKeyBeeftextEnabled, kDefaultBeeftextEnabled);
   object[kKeyComboListFolderPath] = this->readSettings<QString>(kKeyComboListFolderPath, 
      defaultComboListFolderPath());
   object[kKeyComboPickerEnabled] = this->readSettings<bool>(kKeyComboPickerEnabled, kDefaultComboPickerEnabled);
   object[kKeyComboPickerShortcutKeyCode] = qint32(this->readSettings<quint32>(kKeyComboPickerShortcutKeyCode, 0));
   object[kKeyComboPickerShortcutModifiers] = qint32(this->readSettings<quint32>(kKeyComboPickerShortcutModifiers, 0));
   object[kKeyComboPickerShortcutScanCode] = qint32(this->readSettings<quint32>(kKeyComboPickerShortcutScanCode, 0));
   object[kKeyComboTriggerShortcutKeyCode] = qint32(this->readSettings<quint32>(kKeyComboTriggerShortcutKeyCode, 0));
   object[kKeyComboTriggerShortcutModifiers] = 
      qint32(this->readSettings<quint32>(kKeyComboTriggerShortcutModifiers, 0));
   object[kKeyComboTriggerShortcutScanCode] = 
      qint32(this->readSettings<quint32>(kKeyComboTriggerShortcutScanCode, 0));
   object[kKeyCustomBackupLocation] = this->readSettings<QString>(kKeyCustomBackupLocation, 
      globals::defaultBackupFolderPath());
   object[kKeyCustomSoundPath] = this->readSettings<QString>(kKeyCustomSoundPath, QString());
   object[kKeyDelayBetweenKeystrokes] = this->readSettings<qint32>(kKeyDelayBetweenKeystrokes, 
      kDefaultDelayBetweenKeystrokesMs);
   object[kKeyEmojiLeftDelimiter] = this->readSettings<QString>(kKeyEmojiLeftDelimiter, kDefaultEmojiLeftDelimiter);
   object[kKeyEmojiRightDelimiter] = this->readSettings<QString>(kKeyEmojiRightDelimiter, kDefaultEmojiRightDelimiter);
   object[kKeyEmojiShortcodesEnabled] = this->readSettings<bool>(kKeyEmojiShortcodesEnabled, 
      kDefaultEmojiShortcodesEnabled);
   object[kKeyEnableAppEnableDisableShortcut] = this->readSettings<bool>(kKeyEnableAppEnableDisableShortcut, 
      kDefaultEnableAppEnableDisableShortcut);
   object[kKeyGeometry] = QString::fromLocal8Bit(this->readSettings<QByteArray>(kKeyGeometry, QByteArray()).toHex());
   object[kKeyLastComboImportExportPath] = this->readSettings<QString>(kKeyLastComboImportExportPath, QString());
   object[kKeyLastUpdateCheckDateTime] = QString::fromLocal8Bit(variantToByteArray( 
      this->lastUpdateCheckDateTime()).toHex());
   object[kKeyLocale] = QString::fromLocal8Bit(variantToByteArray(this->locale()).toHex());
   object[kKeySplitterState] = QString::fromLocal8Bit(this->readSettings<QByteArray>(kKeySplitterState, 
      QByteArray()).toHex());
   object[kKeyPlaySoundOnCombo] = this->readSettings<bool>(kKeyPlaySoundOnCombo, kDefaultPlaySoundOnCombo);
   object[kKeyUseAutomaticSubstitution] = this->readSettings<bool>(kKeyUseAutomaticSubstitution, 
      kDefaultUseAutomaticSubstitution);
   object[kKeyUseCustomBackupLocation] = this->readSettings<bool>(kKeyUseCustomSound, kDefaultUseCustomBackupLocation);
   object[kKeyUseCustomSound] = this->readSettings<bool>(kKeyUseCustomSound, kDefaultUseCustomSound);
   object[kKeyUseCustomTheme] = this->readSettings<bool>(kKeyUseCustomTheme, kDefaultUseCustomTheme);
   object[kKeyWarnAboutShortComboKeyword] = this->readSettings<bool>(kKeyWarnAboutShortComboKeyword, 
      kDefaultWarnAboutShortComboKeyword);
   object[kKeyWriteDebugLogFile] = this->readSettings<bool>(kKeyWriteDebugLogFile, 
      kDefaultWriteDebugLogFile);

   outDoc = QJsonDocument(object);
}




//**********************************************************************************************************************
/// \param[in] doc The JSON document.
//**********************************************************************************************************************
void PreferencesManager::fromJsonDocument(QJsonDocument const& doc)
{
   QJsonObject const object = doc.object();
   settings_->setValue(kKeyAppEnableShortcutKeyCode, objectValue<quint32>(object, kKeyAppEnableShortcutKeyCode));
   settings_->setValue(kKeyAppEnableShortcutModifiers, objectValue<quint32>(object, kKeyAppEnableShortcutModifiers));
   settings_->setValue(kKeyAppEnableShortcutScanCode, objectValue<quint32>(object, kKeyAppEnableShortcutScanCode));
   settings_->setValue(kKeyAutoBackup, objectValue<bool>(object, kKeyAutoBackup));
   settings_->setValue(kKeyAutoCheckForUpdates, objectValue<bool>(object, kKeyAutoCheckForUpdates));
   settings_->setValue(kKeyAutoStartAtLogin, objectValue<bool>(object, kKeyAutoStartAtLogin));
   settings_->setValue(kKeyBeeftextEnabled, objectValue<bool>(object, kKeyBeeftextEnabled));
   this->setComboListFolderPath(objectValue<QString>(object, kKeyComboListFolderPath));
   settings_->setValue(kKeyComboPickerEnabled, objectValue<bool>(object, kKeyComboPickerEnabled));
   settings_->setValue(kKeyComboPickerShortcutKeyCode, objectValue<quint32>(object, kKeyComboPickerShortcutKeyCode));
   settings_->setValue(kKeyComboPickerShortcutModifiers, objectValue<quint32>(object, kKeyComboPickerShortcutModifiers));
   settings_->setValue(kKeyComboPickerShortcutScanCode, objectValue<quint32>(object, kKeyComboPickerShortcutScanCode));
   settings_->setValue(kKeyComboTriggerShortcutKeyCode, objectValue<quint32>(object, kKeyComboTriggerShortcutKeyCode));
   settings_->setValue(kKeyComboTriggerShortcutModifiers, objectValue<quint32>(object, 
      kKeyComboTriggerShortcutModifiers));
   settings_->setValue(kKeyComboTriggerShortcutScanCode, objectValue<quint32>(object, kKeyComboTriggerShortcutScanCode));
   settings_->setValue(kKeyCustomSoundPath, objectValue<QString>(object, kKeyCustomSoundPath));
   this->setCustomBackupLocation(objectValue<QString>(object, kKeyCustomBackupLocation)); // we call the function because it has side effects
   settings_->setValue(kKeyDelayBetweenKeystrokes, objectValue<qint32>(object, kKeyDelayBetweenKeystrokes));
   settings_->setValue(kKeyEmojiLeftDelimiter, objectValue<QString>(object, kKeyEmojiLeftDelimiter));
   settings_->setValue(kKeyEmojiRightDelimiter, objectValue<QString>(object, kKeyEmojiRightDelimiter));
   settings_->setValue(kKeyEmojiShortcodesEnabled, objectValue<bool>(object, kKeyEmojiShortcodesEnabled));
   settings_->setValue(kKeyEnableAppEnableDisableShortcut, objectValue<bool>(object, 
      kKeyEnableAppEnableDisableShortcut));
   settings_->setValue(kKeyGeometry, QByteArray::fromHex(objectValue<QString>(object, kKeyGeometry).toLocal8Bit()));
   settings_->setValue(kKeyLastComboImportExportPath, objectValue<QString>(object, kKeyLastComboImportExportPath));
   settings_->setValue(kKeyLastUpdateCheckDateTime,byteArrayToVariant<QDateTime>(QByteArray::fromHex(
      objectValue<QString>(object, kKeyLastUpdateCheckDateTime).toLocal8Bit())));
   settings_->setValue(kKeyLocale, byteArrayToVariant<QLocale>(QByteArray::fromHex(objectValue<QString>(object, 
      kKeyLocale).toLocal8Bit())));
   settings_->setValue(kKeyGeometry, QByteArray::fromHex(objectValue<QString>(object, 
      kKeySplitterState).toLocal8Bit()));
   settings_->setValue(kKeyPlaySoundOnCombo, objectValue<bool>(object, kKeyPlaySoundOnCombo));
   settings_->setValue(kKeyUseAutomaticSubstitution, objectValue<bool>(object, kKeyUseAutomaticSubstitution));
   this->setUseCustomBackupLocation(objectValue<bool>(object, kKeyUseCustomBackupLocation)); // we call the function because it has side effects
   settings_->setValue(kKeyUseCustomSound, objectValue<bool>(object, kKeyUseCustomSound));
   settings_->setValue(kKeyUseCustomTheme, objectValue<bool>(object, kKeyUseCustomTheme));
   settings_->setValue(kKeyWarnAboutShortComboKeyword, objectValue<bool>(object, kKeyWarnAboutShortComboKeyword));
   settings_->setValue(kKeyWriteDebugLogFile, objectValue<bool>(object, kKeyWriteDebugLogFile));

   this->init();
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
   return isInPortableMode() ? false : this->readSettings<bool>(kKeyAutoStartAtLogin, kDefaultAutoStartAtLogin);
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
   return this->readSettings<bool>(kKeyPlaySoundOnCombo, kDefaultPlaySoundOnCombo);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preferences.
//**********************************************************************************************************************
void PreferencesManager::setUseCustomSound(bool value) const
{
   settings_->setValue(kKeyUseCustomSound, value);
}


//**********************************************************************************************************************
/// \return The value for the preference.
//**********************************************************************************************************************
bool PreferencesManager::useCustomSound() const
{
   return this->readSettings<bool>(kKeyUseCustomSound, kDefaultUseCustomSound);
}


//**********************************************************************************************************************
/// \param[in] path The path of the custom sound file.
//**********************************************************************************************************************
void PreferencesManager::setCustomSoundPath(QString const& path) const
{
   settings_->setValue(kKeyCustomSoundPath, path);
}


//**********************************************************************************************************************
/// \return The path of the custom sound file.
//**********************************************************************************************************************
QString PreferencesManager::customSoundPath() const
{
   return this->readSettings<QString>(kKeyCustomSoundPath, QString());
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
   return this->readSettings<bool>(kKeyAutoCheckForUpdates, kDefaultAutoCheckForUpdates);
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
   return this->readSettings<bool>(kKeyUseCustomTheme, kDefaultUseCustomTheme);
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
   return this->readSettings<bool>(kKeyWarnAboutShortComboKeyword, kDefaultWarnAboutShortComboKeyword);
}


//**********************************************************************************************************************
/// \param[in] path The path.
//**********************************************************************************************************************
bool PreferencesManager::setComboListFolderPath(QString const& path) const
{
   if (isInPortableMode())
   {
      globals::debugLog().addWarning("Trying to the set the 'combo list folder path' preference while running "
         "in portable mode");
      return false;
   }
   QString const previousPath = QDir::fromNativeSeparators(comboListFolderPath());
   settings_->setValue(kKeyComboListFolderPath, path);
   if (!ComboManager::instance().saveComboListToFile())
   {
      settings_->setValue(kKeyComboListFolderPath, previousPath);
      return false;
   }
   return true;
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
   SpShortcut const newShortcut = shortcut ? shortcut : kDefaultComboTriggerShortcut;
   SpShortcut currentShortcut = this->comboTriggerShortcut();
   if (!currentShortcut)
      currentShortcut = kDefaultComboTriggerShortcut;
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
   return this->readSettings<bool>(kKeyAutoBackup, kDefaultAutoBackup);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setUseCustomBackupLocation(bool value) const
{
   QString const oldPath = globals::backupFolderPath();
   settings_->setValue(kKeyUseCustomBackupLocation, value);
   QString const newPath = globals::backupFolderPath();
   if (QDir(oldPath).canonicalPath() != globals::backupFolderPath())
      BackupManager::moveBackupFolder(oldPath, newPath);
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::useCustomBackupLocation() const
{
   return this->readSettings<bool>(kKeyUseCustomBackupLocation, kDefaultUseCustomBackupLocation);
}


//**********************************************************************************************************************
/// \param[in] path The path of the custom backup location.
//**********************************************************************************************************************
void PreferencesManager::setCustomBackupLocation(QString const& path) const
{
   QString const oldPath = globals::backupFolderPath();
   settings_->setValue(kKeyCustomBackupLocation, path);
   QString const newPath = globals::backupFolderPath();
   if (QDir(oldPath).canonicalPath() != globals::backupFolderPath())
      BackupManager::moveBackupFolder(oldPath, newPath);
}


//**********************************************************************************************************************
/// \return The custom backup location.
//**********************************************************************************************************************
QString PreferencesManager::customBackupLocation() const
{
   return this->readSettings<QString>(kKeyCustomBackupLocation, globals::defaultBackupFolderPath());
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
   return this->readSettings<bool>(kKeyWriteDebugLogFile, kDefaultWriteDebugLogFile);
}


//**********************************************************************************************************************
/// \return The preference value
//**********************************************************************************************************************
QString PreferencesManager::lastComboImportExportPath() const
{
   return this->readSettings<QString>(kKeyLastComboImportExportPath, kDefaultLastComboImportExportPath);
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
   return kDefaultComboTriggerShortcut;
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
   settings_->setValue(kKeyEmojiShortcodesEnabled, value);
}


//**********************************************************************************************************************
/// \return The value for the preference.
//**********************************************************************************************************************
QString PreferencesManager::emojiLeftDelimiter() const
{
   QString const result = cachedEmojiLeftDelimiter_;
   return result.isEmpty() ? kDefaultEmojiLeftDelimiter : result;
}


//**********************************************************************************************************************
/// \param[in] delimiter The value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setEmojiLeftDelimiter(QString const& delimiter)
{
   cachedEmojiLeftDelimiter_ = delimiter;
   settings_->setValue(kKeyEmojiLeftDelimiter, delimiter);
}


//**********************************************************************************************************************
/// \return The value for the preference.
//**********************************************************************************************************************
QString PreferencesManager::emojiRightDelimiter() const
{
   QString const result = cachedEmojiRightDelimiter_;
   return result.isEmpty() ? kDefaultEmojiRightDelimiter : result;
}


//**********************************************************************************************************************
/// \param[in] delimiter The value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setEmojiRightDelimiter(QString const& delimiter)
{
   cachedEmojiRightDelimiter_ = delimiter;
   settings_->setValue(kKeyEmojiRightDelimiter, delimiter);
}


//**********************************************************************************************************************
/// \return The value for the preference.
//**********************************************************************************************************************
qint32 PreferencesManager::delayBetweenKeystrokesMs() const
{
   return qBound<qint32>(kMinValueDelayBetweenKeystrokesMs, this->readSettings<qint32>(kKeyDelayBetweenKeystrokes,
      kDefaultDelayBetweenKeystrokesMs), kMaxValueDelayBetweenKeystrokesMs);
}


//**********************************************************************************************************************
/// \param[in] value The value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setDelayBetweenKeystrokesMs(qint32 value) const
{
   settings_->setValue(kKeyDelayBetweenKeystrokes, qBound<qint32>(kMinValueDelayBetweenKeystrokesMs, value,
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
   settings_->setValue(kKeyComboPickerEnabled, value);
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
   cachedComboTriggerShortcut_ = shortcut ? shortcut : kDefaultComboTriggerShortcut;
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
void PreferencesManager::cacheAppEnableDisableShortcut()
{
   SpShortcut const shortcut = this->readShortcutFromPreferences(kKeyAppEnableShortcutModifiers,
      kKeyAppEnableShortcutKeyCode, kKeyAppEnableShortcutScanCode);
   cachedAppEnableDisableShortcut_ = shortcut ? shortcut : defaultAppEnableDisableShortcut();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PreferencesManager::applyCustomThemePreference() const
{
   if (!this->useCustomTheme())
   {
      qApp->setStyleSheet(QString());
      return;
   }
   QString const resourcePath;
   QFile f(":/MainWindow/Resources/style.qss");
   if (!f.open(QIODevice::ReadOnly))
   {
      globals::debugLog().addInfo(QString("Could not load stylesheet from resource '%1'").arg(resourcePath));
      qApp->setStyleSheet(QString());
      return;
   }
   qApp->setStyleSheet(QString::fromLocal8Bit(f.readAll()));
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

   QSettings(kKeyAutoStart, QSettings::NativeFormat).setValue(constants::kApplicationName,
      QDir::toNativeSeparators(installedPath));
   return true;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PreferencesManager::unregisterApplicationFromAutoStart()
{
   if (!isInPortableMode())
      QSettings(kKeyAutoStart, QSettings::NativeFormat).remove(constants::kApplicationName);
}


//**********************************************************************************************************************
/// \param[in] enable The value for the preference.
//**********************************************************************************************************************
void PreferencesManager::setEnableAppEnableDisableShortcut(bool enable)
{
   settings_->setValue(kKeyEnableAppEnableDisableShortcut, enable);
   cachedEnableAppEnableDisableShortcut_ = enable;
}


//**********************************************************************************************************************
/// \return The value for the preference
//**********************************************************************************************************************
bool PreferencesManager::enableAppEnableDisableShortcut() const
{
   return cachedEnableAppEnableDisableShortcut_;
}


//**********************************************************************************************************************
/// \param[in] shortcut The shortcut.
//**********************************************************************************************************************
void PreferencesManager::setAppEnableDisableShortcut(SpShortcut const& shortcut)
{
   SpShortcut const newShortcut = shortcut ? shortcut : kDefaultAppEnableDisableShortcut;
   SpShortcut currentShortcut = this->appEnableDisableShortcut();
   if (!currentShortcut)
      currentShortcut = kDefaultAppEnableDisableShortcut;
   if (*newShortcut != *currentShortcut)
   {
      settings_->setValue(kKeyAppEnableShortcutModifiers, int(shortcut->nativeModifiers()));
      settings_->setValue(kKeyAppEnableShortcutKeyCode, shortcut->nativeVirtualKey());
      settings_->setValue(kKeyAppEnableShortcutScanCode, shortcut->nativeScanCode());
      cachedAppEnableDisableShortcut_ = newShortcut;
   }
}


//**********************************************************************************************************************
/// \return The shortcut.
//**********************************************************************************************************************
SpShortcut PreferencesManager::appEnableDisableShortcut() const
{
   return cachedAppEnableDisableShortcut_;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
SpShortcut PreferencesManager::defaultAppEnableDisableShortcut()
{
   return kDefaultAppEnableDisableShortcut;
}


//**********************************************************************************************************************
/// \param[in] enabled Is Beeftext enabled?
//**********************************************************************************************************************
void PreferencesManager::setBeeftextEnabled(bool enabled)
{
   if (cachedBeeftextEnabled_ == enabled)
      return;
   settings_->setValue(kKeyBeeftextEnabled, enabled);
   cachedBeeftextEnabled_ = enabled;
}


//**********************************************************************************************************************
/// \return true if and only if Beeftext is enabled.
//**********************************************************************************************************************
bool PreferencesManager::beeftextEnabled() const
{
   return cachedBeeftextEnabled_;
}


