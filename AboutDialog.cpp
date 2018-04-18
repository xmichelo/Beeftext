/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of the 'About' dialog class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "AboutDialog.h"
#include "BeeftextConstants.h"
#include "BeeftextUtils.h"


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
AboutDialog::AboutDialog(QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
   , ui_()
{
   ui_.setupUi(this);
   this->completeText();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void AboutDialog::completeText() const
{
   ui_.labelText->setText(ui_.labelText->text().arg(constants::kVersionMajor).arg(constants::kVersionMinor)
      .arg(isInPortableMode() ? tr("Portable Edition") : "").arg(QString(__DATE__).right(4))); // we extract the year from the __DATE__ macro
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void AboutDialog::changeEvent(QEvent *event)
{
   if (QEvent::LanguageChange == event->type())
   {
      ui_.retranslateUi(this);
      this->completeText();
   }
   QDialog::changeEvent(event);
}

