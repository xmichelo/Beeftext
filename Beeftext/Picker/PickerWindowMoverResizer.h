/// \file
/// \author 
///
/// \brief Declaration of mover/resizer class for the picker window.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_PICKER_WINDOW_MOVER_RESIZER_H
#define BEEFTEXT_PICKER_WINDOW_MOVER_RESIZER_H


class PickerWindow;


//****************************************************************************************************************************************************
/// \brief A class for handling moving and resizing of the picker widget.
//****************************************************************************************************************************************************
class PickerWindowMoverResizer {
public: // data types
    enum class EMode {
        Normal = 0, ///< Normal mode.
        ResizingDiagonal = 1, ///< Resizing mode diagonal.
        ResizingHorizontal = 2, ///< Resizing mode horizontal.
        ResizingVertical = 3, ///< Resizing mode vertical.
        Moving = 4
    }; ///< Enumeration for the resizer mode.

public: // member functions
    explicit PickerWindowMoverResizer(PickerWindow &window); ///< Default constructor.
    PickerWindowMoverResizer(PickerWindowMoverResizer const &) = delete; ///< Disabled copy-constructor.
    PickerWindowMoverResizer(PickerWindowMoverResizer &&) = delete; ///< Disabled assignment copy-constructor.
    ~PickerWindowMoverResizer() = default; ///< Destructor.
    PickerWindowMoverResizer &operator=(PickerWindowMoverResizer const &) = delete; ///< Disabled assignment operator.
    PickerWindowMoverResizer &operator=(PickerWindowMoverResizer &&) = delete; ///< Disabled move assignment operator.
    void processMouseMoveEvent();
    void processMousePressEvent(QMouseEvent const *event);
    void processMouseReleaseEvent(QMouseEvent const *event);

private: // member functions
    EMode resizingModeForCurrentMousePosition() const; ///< Check whether the mouse is over the resize area.
    static QCursor cursorForMode(EMode mode); ///< Retur the cursor for the current mouse position.

private: // data members
    PickerWindow &window_; ///< The widget.
    EMode mode_ { EMode::Normal }; ///< The current resizing mode.
    QPoint prevMousePos_ {}; ///< The previous mouse position.
};


#endif // #ifndef BEEFTEXT_PICKER_WINDOW_MOVER_RESIZER_H
