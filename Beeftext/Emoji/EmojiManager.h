/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of emoji manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_EMOJI_MANAGER_H
#define BEEFTEXT_EMOJI_MANAGER_H


#include "value.h"


//**********************************************************************************************************************
/// \brief Emoji manager class
//**********************************************************************************************************************
class EmojiManager
{
public: // static member functions
   static EmojiManager& instance(); ///< Return the only allowed instance of the class
   void loadEmojis(); ///< Load the emoji list from file
   void unloadEmojis(); ///< Unload the emoji list
   QString emoji(QString const& keyword) const; ///< Retrieve the emoji associated to a keyword

public: // member functions
   EmojiManager(EmojiManager const&) = delete; ///< Disabled copy-constructor
   EmojiManager(EmojiManager&&) = delete; ///< Disabled assignment copy-constructor
   ~EmojiManager() = default; ///< Destructor
   EmojiManager& operator=(EmojiManager const&) = delete; ///< Disabled assignment operator
   EmojiManager& operator=(EmojiManager&&) = delete; ///< Disabled move assignment operator
   bool isExcludedApplication(QString const& appExeName) const; ///< Check whether the application is excluded from emoji substitution.
   bool runDialog(QWidget* parent = nullptr); ///< Run the sensitive application dialog

private: // member functions
   EmojiManager(); ///< Default constructor
   bool load(QString const& path); ///< Load the emoji list from file

private: // data members
   EmojiList emojis_; ///< The list of emojis
   QStringList excludedApps_; ///< The list of applications where emoji should not be substituted
};



#endif // #ifndef BEEFTEXT_EMOJI_MANAGER_H
