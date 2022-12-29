/// \file
/// \author Xavier Michelon
///
/// \brief 


#include "stdafx.h"
#include "ClipboardManager.h"
#include "ClipboardManagerLegacy.h"
#include "ClipboardManagerDefault.h"
#include "Preferences/PreferencesManager.h"


namespace {


std::unique_ptr<ClipboardManager> clipboardManagerPtr = nullptr; ///< The global variable containing the clipboard manager


}


//****************************************************************************************************************************************************
/// \return A reference to the clipboard manager
//****************************************************************************************************************************************************
ClipboardManager &ClipboardManager::instance() {
    setClipboardManagerType(PreferencesManager::instance().useLegacyCopyPaste() ? EType::Legacy : EType::Default);
    return *clipboardManagerPtr;
}


//****************************************************************************************************************************************************
/// \param[in] type The new clipboard manager type.
//****************************************************************************************************************************************************
void ClipboardManager::setClipboardManagerType(EType type) {
    if (clipboardManagerPtr && (clipboardManagerPtr->type() == type))
        return;
    if (EType::Default == type)
        clipboardManagerPtr = std::make_unique<ClipboardManagerDefault>();
    else
        clipboardManagerPtr = std::make_unique<ClipboardManagerLegacy>();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
ClipboardManager::ClipboardManager()
   : QObject()
{
}
