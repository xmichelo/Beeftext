/// \file
/// \author 
///
/// \brief Implementation of combo picker window
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ComboPickerWindow.h"


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void showComboPickerWindow()
{
   static ComboPickerWindow window;
   window.move(QCursor::pos());
   window.show();
   window.activateWindow();
   window.raise();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
ComboPickerWindow::ComboPickerWindow()
   : QWidget(nullptr)
{
   ui_.setupUi(this);
   this->setWindowFlag(Qt::FramelessWindowHint, true);
   ui_.listViewResults->setModel(&proxyModel_);
   proxyModel_.setSourceModel(&model_);
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void ComboPickerWindow::keyPressEvent(QKeyEvent* event)
{
   if (event->key() == Qt::Key_Escape)
     this->close();
   QWidget::keyPressEvent(event);
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void ComboPickerWindow::changeEvent(QEvent* event)
{
   if ((event->type() == QEvent::ActivationChange) && !this->isActiveWindow())
      this->close(); // when the dialog looses the focus, we dismiss is because we don't know where the input
         // focus can be now.
   QWidget::changeEvent(event);
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void ComboPickerWindow::showEvent(QShowEvent* event)
{
   ui_.editSearch->setText(QString());
}


//**********************************************************************************************************************
/// \param[in] text The new text in the search edit.
//**********************************************************************************************************************
void ComboPickerWindow::onSearchTextChanged(QString const& text)
{
   qDebug() << QString("New search text: %1").arg(text);
   proxyModel_.setFilterFixedString(text.trimmed());
   if (proxyModel_.rowCount() > 0)
      ui_.listViewResults->setCurrentIndex(proxyModel_.index(0, 0));
}


