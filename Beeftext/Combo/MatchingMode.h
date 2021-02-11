/// \file
/// \author 
///
/// \brief Declaration of functions related to the matching mode.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_MATCHING_MODE_H
#define BEEFTEXT_MATCHING_MODE_H


//**********************************************************************************************************************
/// \brief An enumeration for matching mode
//**********************************************************************************************************************
enum class EMatchingMode: qint32
{
   Default = 0, ///< The default matching mode, defined in the application's preferences.
   Strict = 1, ///< Strict matching.
   Loose = 2, ///< Loose matching.
};


#endif // #ifndef BEEFTEXT_MATCHING_MODE_H
