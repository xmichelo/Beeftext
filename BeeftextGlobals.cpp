/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of project global variables
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextGlobals.h"


namespace globals {


//**********************************************************************************************************************
/// \return The application debug log
//**********************************************************************************************************************
xmilib::DebugLog& debugLog()
{
   static xmilib::DebugLog log;
   return log;
}


} // namespace globals
