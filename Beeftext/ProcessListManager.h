/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of functions related to process list
/// 
/// A process list manager a list of process to filter (because they're sensitive or should be excluded). The list
/// Element listed can contain wildcards.


#ifndef BEEFTEXT_PROCESS_LIST_MANAGER_H
#define BEEFTEXT_PROCESS_LIST_MANAGER_H


//****************************************************************************************************************************************************
/// \brief Sensitive application Manager
//****************************************************************************************************************************************************
class ProcessListManager {
public: // member functions
    explicit ProcessListManager(QString description); ///< Default constructor
    ProcessListManager(ProcessListManager const &) = delete; ///< Disabled copy-constructor
    ProcessListManager(ProcessListManager &&) = delete; ///< Disabled assignment copy-constructor
    ~ProcessListManager() = default; ///< Destructor
    ProcessListManager &operator=(ProcessListManager const &) = delete; ///< Disabled assignment operator
    ProcessListManager &operator=(ProcessListManager &&) = delete; ///< Disabled move assignment operator
    void setFilePath(QString const &filePath); ///< Set the path of the file where the list should be loaded / saved
    bool load(); ///< Load the process list from file.
    bool save() const; ///< Save the process list to file.
    bool filter(QString const &processName) const; ///< Check whether a process belong to the process list
    bool runDialog(QWidget *parent); ///< Run the sensitive application dialog
    void clear(); ///< Clears the list.
    void addProcess(QString const &process); ///< Add an entry to the process list
    void addProcesses(QStringList const &processes); ///< Add an entry to the process list

private: // data members
    QString description_; ///< The description of the list, to be displayed in the edition dialog.
    QString filePath_; ///< The path of the process list file used for I/O.
    QStringList processList_; ///< The list of sensitive applications
};


#endif // #ifndef BEEFTEXT_SENSITIVE_APPLICATIONS_H
