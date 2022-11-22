/// \file
/// \author 
///
/// \brief Declaration of dialog class for interactively providing the value of a variable
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_VARIABLE_INPUT_DIALOG_H
#define BEEFTEXT_VARIABLE_INPUT_DIALOG_H


//****************************************************************************************************************************************************
/// \brief A dialog class for interactively providing the value of a variable
//****************************************************************************************************************************************************
class VariableInputDialog : public QInputDialog {
Q_OBJECT
public: // member functions
    explicit VariableInputDialog(QString const &description); ///< Default constructor
    VariableInputDialog(VariableInputDialog const &) = delete; ///< Disabled copy-constructor
    VariableInputDialog(VariableInputDialog &&) = delete; ///< Disabled assignment copy-constructor
    ~VariableInputDialog() override = default; ///< Destructor
    VariableInputDialog &operator=(VariableInputDialog const &) = delete; ///< Disabled assignment operator
    VariableInputDialog &operator=(VariableInputDialog &&) = delete; ///< Disabled move assignment operator

protected: // member function
    void showEvent(QShowEvent *event) override; ///< Callback for the show event

public: // static member functions
    static bool run(QString const &description, QString &outUserInput); ///< Run the dialog

protected: // member functions
    void changeEvent(QEvent *) override; ///< Change event handler
};


#endif // #ifndef BEEFTEXT_VARIABLE_INPUT_DIALOG_H
