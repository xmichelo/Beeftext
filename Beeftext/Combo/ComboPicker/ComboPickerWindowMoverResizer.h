/// \file
/// \author 
///
/// \brief Declaration of mover/resizer class for the combo picker window.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_COMBO_PICKER_WINDOW_MOVER_RESIZER_H
#define BEEFTEXT_COMBO_PICKER_WINDOW_MOVER_RESIZER_H


class ComboPickerWindow;


//**********************************************************************************************************************
/// \brief A class for handling moving and resizing of the combo picker widget.
//**********************************************************************************************************************
class ComboPickerWindowMoverResizer
{
public: // data types
   enum class EMode
   {
      Normal = 0, ///< Normal mode.
      ResizingDiagonal = 1, ///< Resizing mode diagonal.
      ResizingHorizontal = 2, ///< Resizing mode horizontal.
      ResizingVertical = 3, ///< Resizing mode vertical.
      Moving = 4
   }; ///< Enumeration for the resizer mode.

public: // member functions
   explicit ComboPickerWindowMoverResizer(ComboPickerWindow& window); ///< Default constructor.
   ComboPickerWindowMoverResizer(ComboPickerWindowMoverResizer const&) = delete; ///< Disabled copy-constructor.
   ComboPickerWindowMoverResizer(ComboPickerWindowMoverResizer&&) = delete; ///< Disabled assignment copy-constructor.
   ~ComboPickerWindowMoverResizer() = default; ///< Destructor.
   ComboPickerWindowMoverResizer& operator=(ComboPickerWindowMoverResizer const&) = delete; ///< Disabled assignment operator.
   ComboPickerWindowMoverResizer& operator=(ComboPickerWindowMoverResizer&&) = delete; ///< Disabled move assignment operator.
   void processMouseMoveEvent();
   void processMousePressEvent(QMouseEvent* event);
   void processMouseReleaseEvent(QMouseEvent* event);

private: // member functions
   EMode resizingModeForCurrentMousePosition() const; ///< Check whether the mouse is over the resize area.
   static QCursor cursorForMode(EMode mode); ///< Retur the cursor for the current mouse position.

private: // data members
   ComboPickerWindow& window_; ///< The widget.
   EMode mode_ { EMode::Normal }; ///< The current resizing mode.
   QPoint prevMousePos_ {}; ///< The previous mouse position.
};


#endif // #ifndef BEEFTEXT_COMBO_PICKER_WINDOW_MOVER_RESIZER_H
