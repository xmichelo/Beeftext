/// \file
/// \author 
///
/// \brief Declaration of function related to the emoji last use file.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_EMOJI_LAST_USE_FILE_H
#define BEEFTEXT_EMOJI_LAST_USE_FILE_H


#include "Emoji/EmojiList.h"


void loadEmojiLastUseDateTimes(EmojiList const &emojiList); ///< Load the emoji last date/times from file.
void saveEmojiLastUseDateTimes(EmojiList const &emojiList); ///< Save the emoji last date/times to file.



#endif // #ifndef BEEFTEXT_EMOJI_LAST_USE_FILE_H
