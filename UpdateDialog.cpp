/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of update dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "UpdateDialog.h"
#include "BeeftextConstants.h"


//**********************************************************************************************************************
/// \param[in] latestVersionInfo The latest version information
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
UpdateDialog::UpdateDialog(SPLatestVersionInfo latestVersionInfo, QWidget* parent)
   : QDialog(parent)
   , latestVersionInfo_(latestVersionInfo)
{
   ui_.setupUi(this);
   if (latestVersionInfo_)
   {
      ui_.labelHeader->setText(ui_.labelHeader->text().arg(constants::kApplicationName)
         .arg(latestVersionInfo_->versionMajor()).arg(latestVersionInfo_->versionMinor()));
      ui_.editReleaseNotes->setHtml(latestVersionInfo_->releaseNotes());
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateDialog::onActionInstall()
{
   qDebug() << QString("%1()").arg(__FUNCTION__);
   this->accept();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void UpdateDialog::onActionSkip()
{
   qDebug() << QString("%1()").arg(__FUNCTION__);
   this->reject();
}


