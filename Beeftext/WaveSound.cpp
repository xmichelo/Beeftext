/// \file
/// \author 
///
/// \brief Implementation of class for loading and playing Wave sound.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "WaveSound.h"


//****************************************************************************************************************************************************
/// \param[in] path The path of the sound file (which can be a Qt resource
//****************************************************************************************************************************************************
WaveSound::WaveSound(QString const &path) {
    this->load(path);
}


//****************************************************************************************************************************************************
/// The wav file is played asynchrously.
///
/// \return true if and only if the file was successfully played.
//****************************************************************************************************************************************************
bool WaveSound::play() {
    if (data_.isEmpty())
        return false;
    return PlaySound(reinterpret_cast<LPCWSTR>(data_.data()), nullptr, SND_MEMORY | SND_NODEFAULT | SND_ASYNC);
}


//****************************************************************************************************************************************************
/// \param[in] path The path of the Wave file.
//****************************************************************************************************************************************************
void WaveSound::load(QString const &path) {
    data_.clear();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    data_ = file.readAll();
}





