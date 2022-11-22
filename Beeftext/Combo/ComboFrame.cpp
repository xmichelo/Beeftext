/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo frame
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboFrame.h"
#include "Group/GroupListWidget.h"
#include "ComboTableWidget.h"


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the frame
//****************************************************************************************************************************************************
ComboFrame::ComboFrame(QWidget *parent)
    : QFrame(parent), ui_() {
    ui_.setupUi(this);
    ui_.comboTableWidget->setGroupListWidget(ui_.groupListWidget);
    connect(ui_.groupListWidget, &GroupListWidget::selectedGroupChanged, ui_.comboTableWidget, &ComboTableWidget::onSelectedGroupChanged);
    ui_.splitter->setStretchFactor(0, 0);
    ui_.splitter->setStretchFactor(1, 1);
}


//****************************************************************************************************************************************************
/// \return The combo group list widget
//****************************************************************************************************************************************************
GroupListWidget *ComboFrame::groupListWidget() const {
    return ui_.groupListWidget;
}


//****************************************************************************************************************************************************
/// \return The combo table widget
//****************************************************************************************************************************************************
ComboTableWidget *ComboFrame::comboTableWidget() const {
    return ui_.comboTableWidget;
}


//****************************************************************************************************************************************************
/// \return A pointer to the splitter widget of the frame.
//****************************************************************************************************************************************************
QSplitter *ComboFrame::splitter() const {
    return ui_.splitter;
}


