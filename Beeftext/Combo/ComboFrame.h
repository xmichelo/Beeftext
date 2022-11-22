/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo frame
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_COMBO_FRAME_H
#define BEEFTEXT_COMBO_FRAME_H


#include "ui_ComboFrame.h"


//****************************************************************************************************************************************************
/// \brief Combo frame class
//****************************************************************************************************************************************************
class ComboFrame : public QFrame {
Q_OBJECT
public: // member functions
    explicit ComboFrame(QWidget *parent = nullptr); ///< Default constructor
    ComboFrame(ComboFrame const &) = delete; ///< Disabled copy-constructor
    ComboFrame(ComboFrame &&) = delete; ///< Disabled assignment copy-constructor
    ~ComboFrame() override = default; ///< Destructor
    ComboFrame &operator=(ComboFrame const &) = delete; ///< Disabled assignment operator
    ComboFrame &operator=(ComboFrame &&) = delete; ///< Disabled move assignment operator
    GroupListWidget *groupListWidget() const; ///< Return the combo group list widget
    ComboTableWidget *comboTableWidget() const; ///< Return the combo table widget
    QSplitter *splitter() const; ///< Return a pointer to the splitter widget of the frame;

private: // data members
    Ui::ComboFrame ui_; ///< The GUI for the frame
};


#endif // #ifndef BEEFTEXT_COMBO_FRAME_H