HEADERS += ./BeeftextConstants.h \
    ./BeeftextGlobals.h \
    ./BeeftextUtils.h \
    ./ClipboardManager.h \
    ./I18nManager.h \
    ./MimeDataUtils.h \
    ./Shortcut.h \
    ./stdafx.h \
    ./LatestVersionInfo.h \
    ./AboutDialog.h \
    ./MainWindow.h \
    ./InputManager.h \
    ./PreferencesManager.h \
    ./ShortcutDialog.h \
    ./PreferencesDialog.h \
    ./Combo/Combo.h \
    ./Combo/ComboVariable.h \
    ./Combo/ComboKeywordValidator.h \
    ./Combo/ComboList.h \
    ./Combo/ComboManager.h \
    ./Combo/ComboDialog.h \
    ./Combo/ComboSortFilterProxyModel.h \
    ./Combo/ComboImportDialog.h \
    ./Combo/ComboFrame.h \
    ./Combo/ComboTableWidget.h \
    ./Update/UpdateCheckWorker.h \
    ./Update/UpdateManager.h \
    ./Update/UpdateDialog.h \
    ./Group/Group.h \
    ./Group/GroupList.h \
    ./Group/GroupDialog.h \
    ./Group/GroupComboBox.h \
    ./Group/GroupListWidget.h \
    ./Backup/BackupManager.h \
    ./Backup/BackupRestoreDialog.h
SOURCES += ./AboutDialog.cpp \
    ./BeeftextConstants.cpp \
    ./BeeftextGlobals.cpp \
    ./BeeftextUtils.cpp \
    ./ClipboardManager.cpp \
    ./I18nManager.cpp \
    ./InputManager.cpp \
    ./LatestVersionInfo.cpp \
    ./main.cpp \
    ./MainWindow.cpp \
    ./MimeDataUtils.cpp \
    ./PreferencesDialog.cpp \
    ./PreferencesManager.cpp \
    ./Shortcut.cpp \
    ./ShortcutDialog.cpp \
    ./stdafx.cpp \
    ./Combo/Combo.cpp \
    ./Combo/ComboDialog.cpp \
    ./Combo/ComboFrame.cpp \
    ./Combo/ComboImportDialog.cpp \
    ./Combo/ComboList.cpp \
    ./Combo/ComboManager.cpp \
    ./Combo/ComboSortFilterProxyModel.cpp \
    ./Combo/ComboKeywordValidator.cpp \
    ./Combo/ComboTableWidget.cpp \
    ./Combo/ComboVariable.cpp \
    ./Update/UpdateCheckWorker.cpp \
    ./Update/UpdateDialog.cpp \
    ./Update/UpdateManager.cpp \
    ./Group/Group.cpp \
    ./Group/GroupComboBox.cpp \
    ./Group/GroupDialog.cpp \
    ./Group/GroupList.cpp \
    ./Group/GroupListWidget.cpp \
    ./Backup/BackupManager.cpp \
    ./Backup/BackupRestoreDialog.cpp
FORMS += ./MainWindow.ui \
    ./ShortcutDialog.ui \
    ./AboutDialog.ui \
    ./PreferencesDialog.ui \
    ./Combo/ComboDialog.ui \
    ./Combo/ComboImportDialog.ui \
    ./Combo/ComboFrame.ui \
    ./Combo/ComboTableWidget.ui \
    ./Update/UpdateDialog.ui \
    ./Group/GroupDialog.ui \
    ./Group/GroupListWidget.ui \
    ./Backup/BackupRestoreDialog.ui
TRANSLATIONS += ./Translations/beeftext_fr.ts
RESOURCES += Beeftext.qrc
