/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo frame
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboFrame.h"


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the frame
//**********************************************************************************************************************
ComboFrame::ComboFrame(QWidget* parent)
   : QFrame(parent),
     ui_()
{
   ui_.setupUi(this);
   ui_.comboTableWidget->setGroupListWidget(ui_.groupListWidget);
   connect(ui_.groupListWidget, &GroupListWidget::selectedGroupChanged, ui_.comboTableWidget,
      &ComboTableWidget::onSelectedGroupChanged);
}


//**********************************************************************************************************************
/// \return The combo group list widget
//**********************************************************************************************************************
GroupListWidget* ComboFrame::groupListWidget() const
{
   return ui_.groupListWidget;
}


//**********************************************************************************************************************
/// \return The combo table widget
//**********************************************************************************************************************
ComboTableWidget* ComboFrame::comboTableWidget() const
{
   return ui_.comboTableWidget;
}


