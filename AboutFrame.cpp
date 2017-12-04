/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of the 'About' frame class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "AboutFrame.h"
#include "BeeftextConstants.h"


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the frame
//**********************************************************************************************************************
AboutFrame::AboutFrame(QWidget* parent)
   :  QFrame(parent)
{
   ui_.setupUi(this);
   ui_.labelText->setText(ui_.labelText->text().arg(constants::kVersionMajor).arg(constants::kVersionMinor));
}

