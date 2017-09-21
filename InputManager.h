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
      kTextBufferSize = 10 ///< The size of the buffer that will receive the text resulting from the processing of the key stroke
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
   void keyboard(int nCode, WPARAM wParam, LPARAM lParam);
   void info(QString const& message);

private: // member functions
   InputManager(); ///< Default constructor
   InputManager(InputManager const&) = delete; ///< Disabled copy constructor
   InputManager(InputManager const&&) = delete; ///< Disabled move constructor
   InputManager& operator=(InputManager const&) = delete; ///< Disabled assignment operator
   void onKeyboardEvent(KeyStroke const& keyStroke); ///< The callback function called at every key event
   QString processKey(KeyStroke const& keystroke); ///< Process a key stroke and return the generated characters 

private: // static member functions
   static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam);

private: // data members
   HHOOK keyboardHook_; ///< The handle to the keyboard hook used to be notified of keyboard events
   KeyStroke deadKey_; ///< The currently active dead key
};




#endif // #ifndef TEXT__WATCH__INPUT__MANAGER__H
