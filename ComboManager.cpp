/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo manager class
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboManager.h"
#include "InputManager.h"
#include "BeeftextGlobals.h"


//**********************************************************************************************************************
/// \return A reference to the only allowed instance of the class
//**********************************************************************************************************************
ComboManager& ComboManager::instance()
{
   static ComboManager instance;
   return instance;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
ComboManager::ComboManager()
   : QObject()
{
   InputManager& inputManager = InputManager::instance();
   connect(&inputManager, &InputManager::comboBreakerTyped, this, &ComboManager::onComboBreakerTyped);
   connect(&inputManager, &InputManager::characterTyped, this, &ComboManager::onCharacterTyped);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboManager::onComboBreakerTyped()
{
   currentText_ = QString();
   globals::debugLog().addInfo("Combo was broken.");
}


//**********************************************************************************************************************
/// \param[in] c The character that was typed
//**********************************************************************************************************************
void ComboManager::onCharacterTyped(QChar c)
{
   currentText_.append(c);
   globals::debugLog().addInfo(QString("Character %1 was type. Combo text is now %2").arg(c).arg(currentText_));
}


