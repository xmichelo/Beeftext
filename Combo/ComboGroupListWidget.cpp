/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list widget
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboManager.h"
#include "ComboGroupListWidget.h"


//**********************************************************************************************************************
/// \param[in] parent The parent of the widget
//**********************************************************************************************************************
ComboGroupListWidget::ComboGroupListWidget(QWidget* parent)
   : QWidget(nullptr)
{
   ui_.setupUi(this);
   ui_.listGroup->setModel(&ComboManager::instance().getComboGroupListRef());
   this->setupGroupsMenu();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupListWidget::setupGroupsMenu()
{
   QMenu* menu = new QMenu(this);
   menu->addAction(ui_.actionNewGroup);
   menu->addSeparator();
   menu->addAction(ui_.actionDeleteGroup);
   ui_.buttonGroups->setMenu(menu);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupListWidget::onActionNewGroup()
{
   qDebug() << QString("%1()").arg(__FUNCTION__);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboGroupListWidget::onActionDeleteGroup()
{
   qDebug() << QString("%1()").arg(__FUNCTION__);
}


