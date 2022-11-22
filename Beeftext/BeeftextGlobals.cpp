/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of project global variables
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"
#include "BeeftextConstants.h"
#include "Preferences/PreferencesManager.h"
#include <XMiLib/Exception.h>


namespace globals {


namespace {


//****************************************************************************************************************************************************
/// \return A reference to the shared pointer to the application debug log
//****************************************************************************************************************************************************
xmilib::SpDebugLog &debugLogSharedPointer() {
    static xmilib::SpDebugLog log = std::make_shared<xmilib::DebugLog>();
    return log;
}


//****************************************************************************************************************************************************
/// \return The path of the JSON file containing the list of sensitive applications
//****************************************************************************************************************************************************
QString sensitiveApplicationsFilePath() {
    return QDir(appDataDir()).absoluteFilePath("sensitiveApps.json");
}


//****************************************************************************************************************************************************
/// \return The path of the JSON file containing the list of excluded applications
//****************************************************************************************************************************************************
QString excludedApplicationsFilePath() {
    return QDir(appDataDir()).absoluteFilePath("excludedApps.json");
}


} // anonymous namespace


//****************************************************************************************************************************************************
/// \return The application debug log
//****************************************************************************************************************************************************
xmilib::DebugLog &debugLog() {
    xmilib::SpDebugLog const &log = debugLogSharedPointer();
    Q_ASSERT(log);
    if (!log)
        throw xmilib::Exception("The debug log could not be instantiated.");
    return *log.get();
}


//****************************************************************************************************************************************************
/// \return An reference to the debug log window.
//****************************************************************************************************************************************************
xmilib::DebugLogWindow &debugLogWindow() {
    static xmilib::DebugLogWindow window(debugLogSharedPointer());
    return window;
}


//****************************************************************************************************************************************************
// \return A reference to the list of sensitive applications.
//****************************************************************************************************************************************************
ProcessListManager &sensitiveApplications() {
    static std::unique_ptr<ProcessListManager> manager;
    if (manager)
        return *manager;

    manager = std::make_unique<ProcessListManager>(QObject::tr(R"(<html><head/><body><p>Use this dialog
      to list sensitive applications that do not work correctly with Beeftext because they do not support standard 
      copy-paste using Ctrl+V.</p><p>List applications using their process name (e.g, notepad.exe). Wildcards 
      are accepted.</p></body></html>)"));
    manager->setFilePath(sensitiveApplicationsFilePath());
    if (!manager->load())
        manager->addProcesses({ "mintty.exe", "putty.exe", "powershell.exe", "kitty*.exe", "ConEmu*.exe" });

    return *manager;
}


//****************************************************************************************************************************************************
// \return A reference to the list of excluded applications.
//****************************************************************************************************************************************************
ProcessListManager &excludedApplications() {
    static std::unique_ptr<ProcessListManager> manager;
    if (manager)
        return *manager;

    manager = std::make_unique<ProcessListManager>(QObject::tr(R"(<html><head/><body><p>Use this dialog
      to list excluded applications. Beeftext will not perform substitution in these applications.</p>
      <p>List applications using their process name (e.g, notepad.exe). Wildcards 
      are accepted.</p></body></html>)"));
    manager->setFilePath(excludedApplicationsFilePath());
    manager->load();
    return *manager;
}


//****************************************************************************************************************************************************
/// \return true iff the application was compiled with Visual Studio.
//****************************************************************************************************************************************************
bool wasAppBuiltWithVisualStudio() {
#ifdef _MSC_VER
    return true;
#else
    return false;
#endif
}


//****************************************************************************************************************************************************
/// \return true if and only if the application was built with GCC
//****************************************************************************************************************************************************
bool wasAppBuiltWithGcc() {
#ifdef __GNUC__
    return true;
#else
    return false;
#endif
}


//****************************************************************************************************************************************************
// \return the Visual studio version used to compile the application.
// \return a null string if the application was not compiled using Visual Studio.
//****************************************************************************************************************************************************
QString getVisualStudioVersion() {
#ifdef _MSC_VER
    if (!wasAppBuiltWithVisualStudio())
        return QString();
    QString const vsStr = "Visual Studio";
    if constexpr (_MSC_VER >= 1930)
        return vsStr + " 2022";
    // ReSharper disable once CppUnreachableCode
    if constexpr (_MSC_VER >= 1920)
        return vsStr + " 2019";
    if constexpr (_MSC_VER >= 1910)
        return vsStr + " 2017";
    if constexpr (_MSC_VER >= 1900)
        return vsStr.arg(" 2015");
    if constexpr (_MSC_VER >= 1800)
        return vsStr.arg(" 2013");
    if constexpr (_MSC_VER >= 1700)
        return vsStr.arg(" 2012");
    if constexpr (_MSC_VER >= 1600)
        return vsStr.arg(" 2010");
    if constexpr (_MSC_VER >= 1500)
        return vsStr.arg(" 2008");
    if constexpr (_MSC_VER >= 1400)
        return vsStr.arg(" 2005");
    if constexpr (_MSC_VER >= 1310)
        return vsStr.arg(" .NET 2003");
    if constexpr (_MSC_VER >= 1300)
        return vsStr.arg(" .NET 2002");
    if constexpr (_MSC_VER >= 1200)
        return vsStr.arg(" 6.0");
    return vsStr;
#else
    return QString();
#endif// #ifdef _MSC_VER
}


//****************************************************************************************************************************************************
/// \return The version number of GCC this
//****************************************************************************************************************************************************
QString getGccVersion() {
#ifdef __GNUC__
    return QString("GCC v%1.%2").arg(__GNUC__).arg(__GNUC_MINOR__);
#else
    return QString();
#endif
}


//****************************************************************************************************************************************************
/// \return The build architecture
//****************************************************************************************************************************************************
QString getBuildArchitecture() {
#if defined(_M_AMD64) || defined(__x86_64__)
    return "64bit";
#elif defined(_M_IX86) || defined(__i386__)
    return "32bit";
#else
    return "Unknown architecture";  // NOLINT(clang-diagnostic-unreachable-code-return)
#endif
}


//****************************************************************************************************************************************************
/// \return the compiler (or IDE) version used to build the application
//****************************************************************************************************************************************************
QString getBuildCompilerVersion() {
    if (wasAppBuiltWithVisualStudio())
        return getVisualStudioVersion();
    if (wasAppBuiltWithGcc())
        return getGccVersion();
    return QString();
}


//****************************************************************************************************************************************************
/// \return The build configuration for the application (Debug or Release).
//****************************************************************************************************************************************************
QString getBuildConfiguration() {
#ifdef NDEBUG
    return "Release";
#else
    return "Debug";
#endif
}


//****************************************************************************************************************************************************
/// \return A string containing build information.
//****************************************************************************************************************************************************
QString getBuildInfo() {
    return QString("%1 - %2 (%3) - Qt v%4")
        .arg(getBuildCompilerVersion(), getBuildConfiguration(), getBuildArchitecture(), QLibraryInfo::version().toString());
}


//****************************************************************************************************************************************************
/// \return The location of the local storage folder for the application
//****************************************************************************************************************************************************
QString appDataDir() {
    return isInPortableMode() ? portableModeDataFolderPath() :
           QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}


//****************************************************************************************************************************************************
/// \return The path of the folder containing the application provided translations.
//****************************************************************************************************************************************************
QString translationRootFolderPath() {
    return QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("Translations");
}


//****************************************************************************************************************************************************
/// \return The path of the folder containing the user-provided translations.
//****************************************************************************************************************************************************
QString userTranslationRootFolderPath() {
    return QDir(appDataDir()).absoluteFilePath("Translations");
}


//****************************************************************************************************************************************************
/// \return The absolute path of the log file
//****************************************************************************************************************************************************
QString logFilePath() {
    return QDir(appDataDir()).absoluteFilePath("log.txt");
}


//****************************************************************************************************************************************************
/// \return The path of the backup folder
//****************************************************************************************************************************************************
QString backupFolderPath() {
    PreferencesManager const &prefs = PreferencesManager::instance();
    QString defaultPath = defaultBackupFolderPath();
    if (!prefs.useCustomBackupLocation())
        return defaultPath;
    QString const customPath = PreferencesManager::instance().customBackupLocation();
    return customPath.isEmpty() ? defaultPath : customPath;
}


//****************************************************************************************************************************************************
/// \return The default path of the backup folder
//****************************************************************************************************************************************************
QString defaultBackupFolderPath() {
    return QDir(appDataDir()).absoluteFilePath("Backup");
}


//****************************************************************************************************************************************************
/// \return The absolute path of the folder where the user data is stored when the that is used when the application is 
/// in portable mode
//****************************************************************************************************************************************************
QString portableModeDataFolderPath() {
    QDir const appDir(QCoreApplication::applicationDirPath());
    return usePortableAppsFolderLayout() ? appDir.absoluteFilePath("../../Data/settings")
                                         : appDir.absoluteFilePath("Data");
}


//****************************************************************************************************************************************************
/// \return The path of the settings file that is used when the application is in portable mode
//****************************************************************************************************************************************************
QString portableModeSettingsFilePath() {
    return QDir(portableModeDataFolderPath()).absoluteFilePath("Settings.ini");
}


//****************************************************************************************************************************************************
/// \return The path of the JSON file containing the list of application excluded from emoji substitution.
//****************************************************************************************************************************************************
QString emojiExcludedAppsFilePath() {
    return QDir(appDataDir()).absoluteFilePath("emojiExcludedApps.json");
}


//****************************************************************************************************************************************************
/// \return the color of disabled items in tables and list views.
//****************************************************************************************************************************************************
QColor disabledTextColorInTablesAndLists() {
    PreferencesManager const &prefs = PreferencesManager::instance();
    return (prefs.useCustomTheme() && (ETheme::Dark == prefs.theme())) ? QColor(0x55, 0x55, 0x55)
                                                                       : QColor(0xa0, 0xa0, 0xa0);
}


//****************************************************************************************************************************************************
/// \return The filter for the backup files dialog.
//****************************************************************************************************************************************************
QString backupFileDialogFilter() {
    return QObject::tr("Beeftext backup files (*.%1);;All files (*.*)").arg(constants::backupFileExtension);
}


//****************************************************************************************************************************************************
/// \return The filter for the JSON files dialog.
//****************************************************************************************************************************************************
QString jsonFileDialogFilter() {
    return QObject::tr("JSON files (*.json);;All files (*.*)");
}


//****************************************************************************************************************************************************
/// \return The filter for the JSON & CSV files dialog.
//****************************************************************************************************************************************************
QString jsonCsvFileDialogFilter() {
    return QObject::tr("JSON & CSV files (*.json *.csv);;JSON files (*.json);;"
                       "CSV files (*.csv);;All files (*.*)");
}


//****************************************************************************************************************************************************
/// \return The filter for the CSV files dialog.
//****************************************************************************************************************************************************
QString csvFileDialogFilter() {
    return QObject::tr("CSV files (*.csv);;All files (*.*)");
}


} // namespace globals
