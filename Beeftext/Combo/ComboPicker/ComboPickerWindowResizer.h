/// \file
/// \author 
///
/// \brief Declaration of resizer class for the combo picker window.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_COMBO_PICKER_WINDOW_RESIZER_H
#define BEEFTEXT_COMBO_PICKER_WINDOW_RESIZER_H


class ComboPickerWindow;


//**********************************************************************************************************************
/// \brief A class for handling resizing of the combo picker widget
//**********************************************************************************************************************
class ComboPickerWindowResizer
{
public: // data types
   enum class EMode
   {
      Normal = 0, ///< Normal mode.
      ResizingDiagonal = 1, ///< Resizing mode diagonal.
      ResizingHorizontal = 2, ///< Resizing mode horizontal.
      ResizingVertical = 3, ///< Resizing mode vertical.
   }; ///< Enumeration for the resizer mode.

public: // member functions
   explicit ComboPickerWindowResizer(ComboPickerWindow& widget); ///< Default constructor.
   ComboPickerWindowResizer(ComboPickerWindowResizer const&) = delete; ///< Disabled copy-constructor.
   ComboPickerWindowResizer(ComboPickerWindowResizer&&) = delete; ///< Disabled assignment copy-constructor.
   ~ComboPickerWindowResizer() = default; ///< Destructor.
   ComboPickerWindowResizer& operator=(ComboPickerWindowResizer const&) = delete; ///< Disabled assignment operator.
   ComboPickerWindowResizer& operator=(ComboPickerWindowResizer&&) = delete; ///< Disabled move assignment operator.
   void processMouseMoveEvent(QMouseEvent* event);
   void processMousePressEvent(QMouseEvent* event);
   void processMouseReleaseEvent(QMouseEvent* event);

private: // member functions
   EMode resizingModeForCurrentMousePosition() const; ///< Check whether the mouse is over the resize area.
   static QCursor cursorForMode(EMode mode); ///< Retur the cursor for the current mouse position.

private: // data members
   ComboPickerWindow& widget_; ///< The widget.
   EMode mode_ { EMode::Normal }; ///< The current resizing mode.
   QPoint prevMousePos_ {}; ///< The previous mouse position.
};


#endif // #ifndef BEEFTEXT_COMBO_PICKER_WINDOW_RESIZER_H
