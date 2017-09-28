/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of input manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__INPUT__MANAGER__H
#define BEEFTEXT__INPUT__MANAGER__H


//**********************************************************************************************************************
/// \brief An input manager capture input by keyboard and mouse and process the events 
//**********************************************************************************************************************
class InputManager : public QObject
{
   Q_OBJECT
public: // data types
   enum { 
      kKeyboardStateSize = 256, ///< The size of the keyboard state array
   }; 
   struct KeyStroke {
      quint32 virtualKey; ///< The virtual keyCode
      quint32 scanCode; ///< The scanCode
      quint8 keyboardState[kKeyboardStateSize]; ///< The state of the keyboard at the moment the keystroke occurred
   };
public: // static member functions
   static InputManager& instance(); ///< Return the only allowed instance of the class

public: // member functions
   ~InputManager(); ///< Default destructor

signals:
   void comboBreakerTyped();
   void characterTyped(QChar c);
   void backspaceTyped();

private: // member functions
   InputManager(); ///< Default constructor
   InputManager(InputManager const&) = delete; ///< Disabled copy constructor
   InputManager(InputManager&&) = delete; ///< Disabled move constructor
   InputManager& operator=(InputManager const&) = delete; ///< Disabled assignment operator
   InputManager& operator=(InputManager&&) = delete; ///< Disabled move assignment operator
   void onKeyboardEvent(KeyStroke const& keyStroke); ///< The callback function called at every key event
   QString processKey(KeyStroke const& keystroke, bool& outIsDeadKey); ///< Process a key stroke and return the generated characters 
   void onMouseClickEvent(int nCode, WPARAM wParam, LPARAM lParam); ///< Process a mouse click event

private: // static member functions
   static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam); ///< The keyboard event callback
   static LRESULT CALLBACK mouseProcedure(int nCode, WPARAM wParam, LPARAM lParam); ///< The mouse event callback

private: // data members
   HHOOK keyboardHook_; ///< The handle to the keyboard hook used to be notified of keyboard events
   HHOOK mouseHook_; ///< The handle to the mouse hook used to be notified of mouse event
   KeyStroke deadKey_; ///< The currently active dead key
};




#endif // #ifndef TEXT__WATCH__INPUT__MANAGER__H
