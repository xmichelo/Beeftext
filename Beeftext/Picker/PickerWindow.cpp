/// \file
/// \author 
///
/// \brief Implementation of picker window.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PickerWindow.h"
#include "PickerItemDelegate.h"
#include "Emoji/EmojiManager.h"
#include "Combo/ComboManager.h"
#include "Preferences/PreferencesManager.h"
#include "BeeftextUtils.h"
#include "BeeftextConstants.h"


//**********************************************************************************************************************
/// \return A rect indicating the position and size of the foreground window.
//**********************************************************************************************************************
QRect foregroundWindowRect()
{
   HWND const hwnd = GetForegroundWindow();
   RECT r;
   if ((!hwnd) || (!GetWindowRect(hwnd, &r)))
      return QRect();
   return QRect(QPoint(r.left, r.top), QPoint(r.right, r.bottom));
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void showComboPickerWindow()
{
   static PickerWindow window;
   QRect const rect = foregroundWindowRect();
   if (rect.isNull())
      window.move(QCursor::pos());
   else
      window.move(QPoint(rect.left() + ((rect.width() - window.width()) / 2),
         rect.top() + ((rect.height() - window.height()) / 2)));
   window.show();
   window.activateWindow();
   window.raise();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
PickerWindow::PickerWindow()
   : QWidget(nullptr)
   , moverResizer_(*this)
{
   ui_.setupUi(this);
   this->setWindowFlag(Qt::FramelessWindowHint, true);
   this->setAttribute(Qt::WA_TranslucentBackground, true);
   ui_.listViewResults->setModel(&proxyModel_);
   ui_.listViewResults->setItemDelegate(new PickerItemDelegate(ui_.listViewResults));
   proxyModel_.setSourceModel(&model_);
   proxyModel_.sort(0, Qt::DescendingOrder);
   qApp->installEventFilter(this);
   this->restoreGeometry(PreferencesManager::instance().comboPickerWindowGeometry());
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void PickerWindow::keyPressEvent(QKeyEvent* event)
{
   switch (event->key())
   {
   case Qt::Key_Escape:
      this->close();
      break;
   case Qt::Key_Down:
      this->selectNextItem();
      break;
   case Qt::Key_Up:
      this->selectPreviousItem();
      break;
   case Qt::Key_Enter:
   case Qt::Key_Return:
      this->triggerSelectedItem();
      break;
   default:
      QWidget::keyPressEvent(event);
   }
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void PickerWindow::changeEvent(QEvent* event)
{
   if ((event->type() == QEvent::ActivationChange) && !this->isActiveWindow())
      this->close(); // when the dialog looses the focus, we dismiss is because we don't know where the input
         // focus can be now.
   QWidget::changeEvent(event);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PickerWindow::showEvent(QShowEvent*)
{
   ui_.editSearch->setText(QString());
   model_.resetModel(); // forces a sort
   this->selectItemAtIndex(0);
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void PickerWindow::mousePressEvent(QMouseEvent* event)
{
   moverResizer_.processMousePressEvent(event);
   QWidget::mousePressEvent(event);
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void PickerWindow::mouseReleaseEvent(QMouseEvent* event)
{
   moverResizer_.processMouseReleaseEvent(event);
   QWidget::mouseReleaseEvent(event);
}


//**********************************************************************************************************************
/// \param[in] watched The watched object.
/// \param[in] event The event.
//**********************************************************************************************************************
bool PickerWindow::eventFilter(QObject* watched, QEvent* event)
{
   if  (event && (QEvent::MouseMove == event->type()))
      moverResizer_.processMouseMoveEvent();

   return QWidget::eventFilter(watched, event);
}


//**********************************************************************************************************************
/// \param[in] text The new text in the search edit.
//**********************************************************************************************************************
void PickerWindow::onSearchTextChanged(QString const& text)
{
   this->style()->polish(ui_.editSearch);
   proxyModel_.setFilterFixedString(text.trimmed());
   if (proxyModel_.rowCount() > 0)
      this->selectItemAtIndex(0);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PickerWindow::onItemClicked(QModelIndex const&)
{
   this->triggerSelectedItem();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PickerWindow::selectPreviousItem() const
{
   qint32 const index= ui_.listViewResults->currentIndex().row();
   if (index > 0)
      this->selectItemAtIndex(index - 1);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PickerWindow::selectNextItem() const
{
   qint32 const index = this->selectedItemIndex();
   if ((index >= 0) && (index <= proxyModel_.rowCount() - 2))
      this->selectItemAtIndex(index + 1);
}


//**********************************************************************************************************************
/// \return The index of the selected combo.
/// \return -1 if no combo is selected.
//**********************************************************************************************************************
qint32 PickerWindow::selectedItemIndex() const
{
   QModelIndex const modelIndex = ui_.listViewResults->currentIndex();
   return modelIndex.isValid() ? modelIndex.row() : -1;
}


//**********************************************************************************************************************
/// \param[in] index The index of the selected combo.
//**********************************************************************************************************************
void PickerWindow::selectItemAtIndex(qint32 index) const
{
   ui_.listViewResults->setCurrentIndex(proxyModel_.index(index, 0));
   ui_.editSearch->setFocus();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PickerWindow::triggerSelectedItem()
{
   this->close();
   QModelIndex const index = ui_.listViewResults->currentIndex();
   if (!index.isValid())
      return;
   bool const isCombo = (constants::Combo == index.data(constants::TypeRole).value<constants::EITemType>());
   SpCombo const combo = (isCombo ? index.data(constants::PointerRole).value<SpCombo>() : nullptr);
   SpEmoji const emoji = (isCombo ? nullptr : index.data(constants::PointerRole).value<SpEmoji>());
   std::chrono::milliseconds constexpr delay(200);
   if (combo)
      QTimer::singleShot(delay, [combo]() { combo->performSubstitution(true); });
   else
      QTimer::singleShot(delay, [emoji]() { 
         if ((!isBeeftextTheForegroundApplication()) && 
            !EmojiManager::instance().isExcludedApplication(getActiveExecutableFileName()))
         {
            performTextSubstitution(0, emoji->value(), -1, ETriggerSource::ComboPicker);
            emoji->setlastUseDateTime(QDateTime::currentDateTime());
         }
      });
}
