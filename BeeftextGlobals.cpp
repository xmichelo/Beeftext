/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of global variables
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextGlobals.h"


//**********************************************************************************************************************
/// Making MainWindow a singleton class would be better, but Qt memory management does not allow it (random crashes on
/// exit).
///
/// \return The main window instance
//**********************************************************************************************************************
MainWindow& globals::mainWindow()
{
   static MainWindow window;
   return window;
}
