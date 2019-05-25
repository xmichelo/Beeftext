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


namespace {
   qint32 kItemVMargin = 2; ///< The size of the vertical margin of an item.
   qint32 kItemHMargin = 10; ///< The size of the horizontal margin of an item.
}


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
/// \return The big font used in item rendering.
//**********************************************************************************************************************
QFont bigFont()
{
   QFont font;
   font.setPointSize(12);
   font.setBold(true);
   font.setItalic(false);
   return font;
}


//**********************************************************************************************************************
/// \return The small font used in item rendering.
//**********************************************************************************************************************
QFont smallFont()
{
   QFont font;
   font.setPointSize(8);
   font.setBold(true);
   font.setItalic(false);
   return font;
   
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void showComboPickerWindow()
{
   static ComboPickerWindow window;
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
/// \param[in] painter The painter.
/// \param[in] option The style options.
/// \param[in] index The index of the item to paint.
//**********************************************************************************************************************
void ResultItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   // we draw the background. Color depends on wether the item is selected or not.
   bool const selected = option.state & QStyle::State_Selected ;
   painter->setBrush(selected ? QColor(80, 140, 200) : QColor(255, 255, 255));
   painter->setPen(Qt::NoPen);
   painter->drawRect(option.rect);

   // we exclude the margin from the rendering rect.
   QRect const rect = option.rect.adjusted(kItemHMargin, kItemVMargin, -kItemHMargin, -kItemVMargin);
   
   // First line (combo name) use big font.
   QFont const bFont = bigFont();
   QFontMetrics const bMetrics(bFont);
   // second line (combo keyword) use small font.
   QFont const sFont = smallFont();

   painter->setFont(bFont);
   painter->setPen(selected ? Qt::white : QColor(78,78, 78));
   painter->drawText(QPoint(rect.left(), rect.top() + bMetrics.ascent()), 
      bMetrics.elidedText(index.data(Qt::DisplayRole).toString(), Qt::ElideRight, rect.width()));
   painter->setPen(QColor(200,200,200));
   painter->setFont(sFont);
   painter->drawText(QPoint(rect.left(), rect.bottom() - QFontMetrics(sFont).descent()), 
      index.data(ComboList::KeywordRole).toString());
}


//**********************************************************************************************************************
/// \param[in] option The style options.
/// \param[in] index The index of the item to paint.
//**********************************************************************************************************************
QSize ResultItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   // we compute the size hint for the item. width is trivial, but height needs attention. 
   // An item contains one line of text in big font, one line of text with small font, and there are margins.
   return QSize(option.rect.width(), QFontMetrics(bigFont()).height() + QFontMetrics(smallFont()).height()
      + 2 * kItemVMargin);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
ComboPickerWindow::ComboPickerWindow()
   : QWidget(nullptr)
{
   ui_.setupUi(this);
   this->setWindowFlag(Qt::FramelessWindowHint, true);
   this->setAttribute(Qt::WA_TranslucentBackground, true);
   ui_.listViewResults->setModel(&proxyModel_);
   ui_.listViewResults->setItemDelegate(new ResultItemDelegate(ui_.listViewResults));
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
