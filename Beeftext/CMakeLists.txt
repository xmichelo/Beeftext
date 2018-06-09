cmake_minimum_required(VERSION 3.10)
project(Beeftext)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


if (NOT DEFINED WIN32)
   if (DEFINED ENV{QTDIR})
      set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH} $ENV{QTDIR})
   else()
      message(FATAL_ERROR "QTDIR needs to be defined and point to the root of your Qt folder. "
         "(e.g. /Users/MyName/Qt/5.10.1/clang_64).")
   endif()
endif()


find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5Multimedia)


include_directories("../..")
include_directories("../Submodules/XMiLib")


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC_MOC_OPTIONS "-bstdafx.h")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(POWERSHELL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Scripts/Powershell)


add_definitions(-DUNICODE -D_UNICODE)


file(TO_NATIVE_PATH "${POWERSHELL_DIR}/CompileTranslationFiles.ps1" TRANS_SRC_DIR)
file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/translations" TRANS_DST_DIR)


add_custom_target(translations
   COMMAND
   powershell.exe -ExecutionPolicy Bypass -NoProfile -NonInteractive -command \"${TRANS_SRC_DIR} ${TRANS_DST_DIR}\" >NUL
)


add_executable(Beeftext
   AboutDialog.cpp
   AboutDialog.h
   BeeftextConstants.cpp
   BeeftextConstants.h
   BeeftextGlobals.cpp
   BeeftextGlobals.h
   BeeftextUtils.cpp
   BeeftextUtils.h
   ClipboardManager.cpp
   ClipboardManager.h
   I18nManager.cpp
   I18nManager.h
   InputManager.cpp
   InputManager.h
   LatestVersionInfo.cpp
   LatestVersionInfo.h
   main.cpp
   MainWindow.cpp
   MainWindow.h
   MimeDataUtils.cpp
   MimeDataUtils.h
   PreferencesDialog.cpp
   PreferencesDialog.h
   PreferencesManager.cpp
   PreferencesManager.h
   Shortcut.cpp
   Shortcut.h
   ShortcutDialog.cpp
   ShortcutDialog.h
   stdafx.cpp
   stdafx.h
   Backup/BackupManager.cpp
   Backup/BackupManager.h
   Backup/BackupRestoreDialog.cpp
   Backup/BackupRestoreDialog.h
   Combo/Combo.cpp
   Combo/Combo.h
   Combo/ComboDialog.cpp
   Combo/ComboDialog.h
   Combo/ComboFrame.cpp
   Combo/ComboFrame.h
   Combo/ComboImportDialog.cpp
   Combo/ComboImportDialog.h
   Combo/ComboKeywordValidator.cpp
   Combo/ComboKeywordValidator.h
   Combo/ComboList.cpp
   Combo/ComboList.h
   Combo/ComboManager.cpp
   Combo/ComboManager.h
   Combo/ComboSortFilterProxyModel.cpp
   Combo/ComboSortFilterProxyModel.h
   Combo/ComboTableWidget.cpp
   Combo/ComboTableWidget.h
   Combo/ComboVariable.cpp
   Combo/ComboVariable.h
   Group/Group.cpp
   Group/Group.h
   Group/GroupComboBox.cpp
   Group/GroupComboBox.h
   Group/GroupDialog.cpp
   Group/GroupDialog.h
   Group/GroupList.cpp
   Group/GroupList.h
   Group/GroupListWidget.cpp
   Group/GroupListWidget.h
   Update/UpdateCheckWorker.cpp
   Update/UpdateCheckWorker.h
   Update/UpdateDialog.cpp
   Update/UpdateDialog.h
   Update/UpdateManager.cpp
   Update/UpdateManager.h
   Beeftext.qrc
   Beeftext.rc
)


add_dependencies(Beeftext translations)


target_link_libraries(Beeftext Qt5::Core)
target_link_libraries(Beeftext Qt5::Gui)
target_link_libraries(Beeftext Qt5::Widgets)
target_link_libraries(Beeftext Qt5::Network)
target_link_libraries(Beeftext Qt5::Multimedia)
target_link_libraries(Beeftext XMiLib)
