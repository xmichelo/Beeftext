/// \file
/// \author 
///
/// \brief Declaration of class for loading and playing Wave sound.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_WAVE_SOUND_H
#define BEEFTEXT_WAVE_SOUND_H


//****************************************************************************************************************************************************
/// \brief A class for loading and playing a Wave sound
//****************************************************************************************************************************************************
class WaveSound {
public: // member functions
    explicit WaveSound(QString const &path); ///< Default constructor.
    WaveSound(WaveSound const &) = delete; ///< Disabled copy-constructor.
    WaveSound(WaveSound &&) = delete; ///< Disabled assignment copy-constructor.
    ~WaveSound() = default; ///< Destructor.
    WaveSound &operator=(WaveSound const &) = delete; ///< Disabled assignment operator.
    WaveSound &operator=(WaveSound &&) = delete; ///< Disabled move assignment operator.
    bool play();

private: // member functions
    void load(QString const &path); ///< The the wav file.

private: // data members
    QByteArray data_; ///< the wav file data
};


#endif // #ifndef BEEFTEXT_WAVE_SOUND_H
