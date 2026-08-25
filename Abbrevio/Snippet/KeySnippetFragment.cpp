/// \file
/// \author 
///
/// \brief Implementation of key snippet fragment.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "KeySnippetFragment.h"
#include "Preferences/PreferencesManager.h"
#include <XMiLib/SystemUtils.h>


namespace {


//****************************************************************************************************************************************************
/// \brief identify a key described by a string.
/// \param[in] keyStr The key
/// \return The key id.
/// \return 0 if the key could not be identified.
//****************************************************************************************************************************************************
quint16 identifyKey(QString const &keyStr) {
    QString const key = keyStr.toLower();
    if (key == "space") return VK_SPACE;
    if (key == "tab") return VK_TAB;
    if ((key == "return") || (key == "enter")) return VK_RETURN;
    if (key == "insert") return VK_HOME;
    if (key == "delete") return VK_DELETE;
    if (key == "home") return VK_HOME;
    if (key == "end") return VK_END;
    if (key == "pageup") return VK_PRIOR;
    if (key == "pagedown") return VK_NEXT;
    if (key == "up") return VK_UP;
    if (key == "down") return VK_DOWN;
    if (key == "left") return VK_LEFT;
    if (key == "right") return VK_RIGHT;
    if (key == "escape") return VK_ESCAPE;
    if (key == "printscreen") return VK_SNAPSHOT;
    if (key == "pause") return VK_PAUSE;
    if (key == "numlock") return VK_NUMLOCK;
    if (key == "volumemute") return VK_VOLUME_MUTE;
    if (key == "volumeup") return VK_VOLUME_UP;
    if (key == "volumedown") return VK_VOLUME_DOWN;
    if (key == "medianexttrack") return VK_MEDIA_NEXT_TRACK;
    if (key == "mediaprevioustrack") return VK_MEDIA_PREV_TRACK;
    if (key == "mediastop") return VK_MEDIA_STOP;
    if (key == "mediaplaypause") return VK_MEDIA_PLAY_PAUSE;
    if (key == "mediaselect") return VK_LAUNCH_MEDIA_SELECT;
    if (key == "windows") return VK_LWIN;
    if (key == "control") return VK_CONTROL;
    if (key == "alt") return VK_LMENU;
    if (key == "shift") return VK_SHIFT;
    for (quint16 i = 1; i <= 24; ++i)
        if (key == QString("f%1").arg(i))
            return VK_F1 + i - 1;
    return 0;
}


} // namespace 


//****************************************************************************************************************************************************
/// \param[in] key The key as text.
/// \param[in] repeatCount The number of time the key should be repeated.
//****************************************************************************************************************************************************
KeySnippetFragment::KeySnippetFragment(QString const &key, qint32 repeatCount)
    : SnippetFragment()
    , key_(identifyKey(key))
    , repeatCount_(repeatCount) {
}


//****************************************************************************************************************************************************
/// \return the type of the fragment
//****************************************************************************************************************************************************
SnippetFragment::EType KeySnippetFragment::type() const {
    return EType::Key;
}


//****************************************************************************************************************************************************
/// \return A string describing the fragment
//****************************************************************************************************************************************************
QString KeySnippetFragment::toString() const {
    return QString("Key fragment: key: 0x%1(%2) - Repeats: %3").arg(key_, 2, 16, QChar('0')).arg(key_).arg(repeatCount_);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void KeySnippetFragment::render() const {
    PreferencesManager const &prefs = PreferencesManager::instance();
    for (qint32 i = 0; i < repeatCount_; ++i) {
        xmilib::synthesizeKeyDownAndUp(key_);
        if (i != repeatCount_ - 1)
            QThread::msleep(prefs.delayBetweenKeystrokesMs());
    }
}
