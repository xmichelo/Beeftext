/// \file
/// \author 
///
/// \brief Implementation of resizer class for the combo picker window.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "ComboPickerWindowResizer.h"
#include "ComboPickerWindow.h"


namespace {
   qint32 const kSizeAreaLength = 20; ///< The length of the size square in pixels.
}


//**********************************************************************************************************************
///\ param[in] widget The widget
//**********************************************************************************************************************
ComboPickerWindowResizer::ComboPickerWindowResizer(ComboPickerWindow& widget)
   : widget_(widget)
{
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void ComboPickerWindowResizer::processMouseMoveEvent(QMouseEvent* event)
{
   if (!event)
      return;
   QPoint const pos = event->pos();
   EMode const modeCandidate = resizingModeForCurrentMousePosition();
   if (EMode::Normal == mode_)
   {

      widget_.setCursor(cursorForMode(modeCandidate));
      return;
   }
   qint32 const dx = ((mode_ == EMode::ResizingHorizontal) || (mode_ == EMode::ResizingDiagonal)) ?
      pos.x() - prevMousePos_.x() : 0;
   qint32 const dy = ((mode_ == EMode::ResizingVertical) || (mode_ == EMode::ResizingDiagonal)) ?
      pos.y() - prevMousePos_.y() : 0;

   if (dx || dy)
      widget_.resize(widget_.width() + dx, widget_.height() + dy);
   prevMousePos_ = pos;
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void ComboPickerWindowResizer::processMousePressEvent(QMouseEvent* event)
{
   if ((!event) || (Qt::LeftButton != event->button()))
      return;
   mode_ = resizingModeForCurrentMousePosition();
   prevMousePos_ = event->pos();
}


//**********************************************************************************************************************
/// \param[in] event The event.
//**********************************************************************************************************************
void ComboPickerWindowResizer::processMouseReleaseEvent(QMouseEvent* event)
{
   if ((!event) || (event->button() != Qt::LeftButton))
      return;
   mode_ = EMode::Normal;
}


//**********************************************************************************************************************
/// \return true if the mouse is over the resize area zone (the lower right corner of the window).
//**********************************************************************************************************************
ComboPickerWindowResizer::EMode ComboPickerWindowResizer::resizingModeForCurrentMousePosition() const
{
   QPoint const pos = widget_.mapFromGlobal(QCursor::pos());
   qint32 const x = pos.x(), y = pos.y();
   QSize const size = widget_.size();
   qint32 const w = size.width(), h = size.height();
   bool const xInZone = (x < w) && (x >= w - kSizeAreaLength);
   bool const yInZone = (y < h) && (y >= h - kSizeAreaLength);
   if (xInZone)
      return yInZone ? EMode::ResizingDiagonal : EMode::ResizingHorizontal;
   return yInZone ? EMode::ResizingVertical : EMode::Normal;
}


//**********************************************************************************************************************
/// \param[in] mode The mode.
/// \return The cursor for the mode.
//**********************************************************************************************************************
QCursor ComboPickerWindowResizer::cursorForMode(EMode mode)
{
   switch (mode)
   {
   case EMode::ResizingDiagonal: return Qt::SizeFDiagCursor;
   case EMode::ResizingHorizontal: return Qt::SizeHorCursor;
   case EMode::ResizingVertical: return Qt::SizeVerCursor;
   case EMode::Normal: default: return Qt::ArrowCursor;
   }
}
