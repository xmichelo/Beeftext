/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of a process list manager class (used to manage sensitive and excluded application lists


#include "stdafx.h"
#include "ProcessListManager.h"
#include "BeeftextGlobals.h"
#include <XMiLib/String/StringListEditorDialog.h>
#include <XMiLib/String/StringUtils.h>


using namespace xmilib;


//****************************************************************************************************************************************************
/// \param[in] description A description of the list to be displayed in the edition dialog.
//****************************************************************************************************************************************************
ProcessListManager::ProcessListManager(QString description)
    : description_(description) {
}


//****************************************************************************************************************************************************
/// \param[in] filePath the path of the process list file.
//****************************************************************************************************************************************************
void ProcessListManager::setFilePath(QString const &filePath) {
    filePath_ = filePath;
}


//****************************************************************************************************************************************************
/// If the file does not exist, the list is considered empty.
///
/// \return true if and only if the file was loaded or there was no file.
//****************************************************************************************************************************************************
bool ProcessListManager::load() {
    processList_.clear();
    if (!QFile(filePath_).exists())
        return true;

    QString errorMsg;
    bool const result = loadStringListFromJsonFile(filePath_, processList_, &errorMsg);
    if (!result) {
        globals::debugLog().addError(QString("%1(): %2").arg(__FUNCTION__).arg(errorMsg));
        processList_.clear();
    }
    return result;
}


//****************************************************************************************************************************************************
/// \return true if and only if the list was saved.
//****************************************************************************************************************************************************
bool ProcessListManager::save() const {
    QString errorMsg;
    bool const result = saveStringListToJsonFile(processList_, filePath_, &errorMsg);
    if (!result)
        globals::debugLog().addError(QString("%1(): %2").arg(__FUNCTION__).arg(errorMsg));
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] processName The name of the executable, including its extension (e.g. "putty.exe")
/// \return true if and only if the application support pasting using the Ctrl+V shortcut
//****************************************************************************************************************************************************
bool ProcessListManager::filter(QString const &processName) const {
    std::function<bool(QString const &)> const predicate = [&](QString const &str) -> bool {
        return QRegularExpression(QRegularExpression::wildcardToRegularExpression(str), QRegularExpression::CaseInsensitiveOption).match(processName).hasMatch();
    };
    return processList_.end() != std::find_if(processList_.begin(), processList_.end(), predicate);
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
/// \return true if and only if the user validated the dialog and the list was successfully saved to file
//****************************************************************************************************************************************************
bool ProcessListManager::runDialog(QWidget *parent) {
    StringListEditorDialog dlg(processList_, parent);
    dlg.setHeaderText(description_);

    QPushButton *button = new QPushButton(QObject::tr("&Reset"), &dlg);
    QObject::connect(button, &QPushButton::clicked, [&dlg]() {
        if (QMessageBox::Yes == QMessageBox::question(&dlg, QObject::tr("Reset List"), QObject::tr("Are you sure you want to reset the list ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
            dlg.setStringList(QStringList());
    });
    dlg.addCustomButton(button);
    if (QDialog::Accepted != dlg.exec())
        return false;
    processList_ = dlg.stringList();
    bool const result = this->save();
    if (!result)
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("The file could not be saved."));
    return result;
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ProcessListManager::clear() {
    processList_.clear();
}


//****************************************************************************************************************************************************
/// \param[in] process The process to add (wilcards are accepted).
//****************************************************************************************************************************************************
void ProcessListManager::addProcess(QString const &process) {
    processList_.append(process);
}


//****************************************************************************************************************************************************
/// \param[in] processes The processes to add.
//****************************************************************************************************************************************************
void ProcessListManager::addProcesses(QStringList const &processes) {
    processList_.append(processes);
}

