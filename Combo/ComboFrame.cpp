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
   : QFrame(parent)
{
   ui_.setupUi(this);
   connect(ui_.comboGroupListWidget, &ComboGroupListWidget::selectedGroupChanged, ui_.comboTableWidget,
      &ComboTableWidget::onSelectedGroupChanged);
}


//**********************************************************************************************************************
/// \return The combo group list widget
//**********************************************************************************************************************
ComboGroupListWidget* ComboFrame::comboGroupListWidget()
{
   return ui_.comboGroupListWidget;
}


//**********************************************************************************************************************
/// \return The combo table widget
//**********************************************************************************************************************
ComboTableWidget* ComboFrame::comboTableWidget()
{
   return ui_.comboTableWidget;
}


