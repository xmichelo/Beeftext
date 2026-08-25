/// \file
/// \author 
///
/// \brief Implementation of resizer class for the picker window.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PickerWindowMoverResizer.h"
#include "PickerWindow.h"
#include "Preferences/PreferencesManager.h"


namespace {
qint32 constexpr kSizeAreaLength = 20; ///< The length of the size square in pixels.
}


//****************************************************************************************************************************************************
///\ param[in] widget The widget
//****************************************************************************************************************************************************
PickerWindowMoverResizer::PickerWindowMoverResizer(PickerWindow &window)
    : window_(window) {
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PickerWindowMoverResizer::processMouseMoveEvent() {
    QPoint const pos = QCursor::pos();
    EMode const modeCandidate = resizingModeForCurrentMousePosition();
    if (EMode::Normal == mode_) {

        window_.setCursor(cursorForMode(modeCandidate));
        return;
    }
    qint32 const dx = ((mode_ == EMode::ResizingHorizontal) || (mode_ == EMode::ResizingDiagonal))
                      || (mode_ == EMode::Moving) ? pos.x() - prevMousePos_.x() : 0;
    qint32 const dy = ((mode_ == EMode::ResizingVertical) || (mode_ == EMode::ResizingDiagonal))
                      || (mode_ == EMode::Moving) ? pos.y() - prevMousePos_.y() : 0;

    prevMousePos_ = pos;
    if (dx || dy) {
        if (EMode::Moving == mode_)
            window_.move(window_.x() + dx, window_.y() + dy);
        else
            window_.resize(window_.width() + dx, window_.height() + dy);
    }
}


//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void PickerWindowMoverResizer::processMousePressEvent(QMouseEvent const *event) {
    if ((!event) || (Qt::LeftButton != event->button()))
        return;
    mode_ = resizingModeForCurrentMousePosition();
    prevMousePos_ = QCursor::pos();
}


//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void PickerWindowMoverResizer::processMouseReleaseEvent(QMouseEvent const *event) {
    if ((!event) || (event->button() != Qt::LeftButton))
        return;
    if (mode_ == EMode::Normal)
        return;
    PreferencesManager::instance().setComboPickerWindowGeometry(window_.saveGeometry());
    mode_ = EMode::Normal;
}


//****************************************************************************************************************************************************
/// \return true if the mouse is over the resize area zone (the lower right corner of the window).
//****************************************************************************************************************************************************
PickerWindowMoverResizer::EMode PickerWindowMoverResizer::resizingModeForCurrentMousePosition() const {
    QPoint const pos = window_.mapFromGlobal(QCursor::pos());
    qint32 const x = pos.x(), y = pos.y();
    QSize const size = window_.size();
    qint32 const w = size.width(), h = size.height();
    if ((y >= 0) && (y < kSizeAreaLength))
        return EMode::Moving;
    bool const xInZone = (x < w) && (x >= w - kSizeAreaLength);
    bool const yInZone = (y < h) && (y >= h - kSizeAreaLength);
    if (xInZone)
        return yInZone ? EMode::ResizingDiagonal : EMode::ResizingHorizontal;
    return yInZone ? EMode::ResizingVertical : EMode::Normal;
}


//****************************************************************************************************************************************************
/// \param[in] mode The mode.
/// \return The cursor for the mode.
//****************************************************************************************************************************************************
QCursor PickerWindowMoverResizer::cursorForMode(EMode mode) {
    switch (mode) {
    case EMode::ResizingDiagonal:
        return Qt::SizeFDiagCursor;
    case EMode::ResizingHorizontal:
        return Qt::SizeHorCursor;
    case EMode::ResizingVertical:
        return Qt::SizeVerCursor;
    case EMode::Moving:
        return Qt::SizeAllCursor;
    case EMode::Normal:
    default:
        return Qt::ArrowCursor;
    }
}
