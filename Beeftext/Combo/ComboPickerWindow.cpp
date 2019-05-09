/// \file
/// \author 
///
/// \brief Implementation of combo picker window
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ComboPickerWindow.h"
#include "ComboManager.h"


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
   switch (event->key())
   {
   case Qt::Key_Escape:
      this->close();
      break;
   case Qt::Key_Down:
      this->selectNextCombo();
      break;
   case Qt::Key_Up:
      this->selectPreviousCombo();
      break;
   case Qt::Key_Enter:
   case Qt::Key_Return:
   {
      SpCombo const combo = this->selectedCombo();
      this->close();
      if (combo)
         QTimer::singleShot(200, [combo]() { combo->insertSnippet(); });
      break;
   }
   default:
      QWidget::keyPressEvent(event);
   }
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
   this->selectComboAtIndex(0);
}


//**********************************************************************************************************************
/// \param[in] text The new text in the search edit.
//**********************************************************************************************************************
void ComboPickerWindow::onSearchTextChanged(QString const& text)
{
   proxyModel_.setFilterFixedString(text.trimmed());
   if (proxyModel_.rowCount() > 0)
      this->selectComboAtIndex(0);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboPickerWindow::selectPreviousCombo() const
{
   qint32 const index= ui_.listViewResults->currentIndex().row();
   if (index > 0)
      this->selectComboAtIndex(index - 1);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboPickerWindow::selectNextCombo() const
{
   qint32 const index = this->selectedComboIndex();
   if ((index >= 0) && (index <= proxyModel_.rowCount() - 2))
      this->selectComboAtIndex(index + 1);
}


//**********************************************************************************************************************
/// \return The index of the selected combo.
/// \return -1 if no combo is selected.
//**********************************************************************************************************************
qint32 ComboPickerWindow::selectedComboIndex() const
{
   QModelIndex const modelIndex = ui_.listViewResults->currentIndex();
   return modelIndex.isValid() ? modelIndex.row() : -1;
}


//**********************************************************************************************************************
/// \return The selected combo.
/// \return A null pointer if no combo is selected.
//**********************************************************************************************************************
SpCombo ComboPickerWindow::selectedCombo() const
{
   QModelIndex const modelIndex = ui_.listViewResults->currentIndex();
   return modelIndex.isValid() ? 
      ComboManager::instance().comboListRef()[proxyModel_.mapToSource(modelIndex).row()] : nullptr;
}


//**********************************************************************************************************************
/// \param[in] index The index of the selected combo.
//**********************************************************************************************************************
void ComboPickerWindow::selectComboAtIndex(qint32 index) const
{
   ui_.listViewResults->setCurrentIndex(proxyModel_.index(index, 0));
   ui_.editSearch->setFocus();
}
