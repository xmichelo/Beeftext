/// \file
/// \author 
///
/// \brief Declaration of emoji class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_EMOJI_H
#define BEEFTEXT_EMOJI_H


//**********************************************************************************************************************
/// \brief Emoji class.
//**********************************************************************************************************************
class Emoji
{
public: // member functions
   Emoji(QString const& shortcode, QString const& emoji, QString const& category); ///< Default constructor.
   Emoji(Emoji const&) = delete; ///< Disabled copy-constructor.
   Emoji(Emoji&&) = delete; ///< Disabled assignment copy-constructor.
   ~Emoji() = default; ///< Destructor.
   Emoji& operator=(Emoji const&) = delete; ///< Disabled assignment operator.
   Emoji& operator=(Emoji&&) = delete; ///< Disabled move assignment operator.
   void setShortcode(QString const& shortcode); ///< Set the shortcode for the emoji.
   QString shortcode() const; /// Get the shortcode.
   void setValue(QString const& value); ///< Set the value.
   QString value() const; ///< Get the value.
   void setCategory(QString const& category); ///< Set the category.
   QString category() const; ///< Get the category.

private:
   QString shortcode_; ///< The shortcode for the emoji.
   QString value_; ///< The emoji.
   QString category_; ///< The category.
};


typedef std::shared_ptr<Emoji> SpEmoji; ///< Type definition for shared pointer to emoji.
typedef QHash<QString, SpEmoji> EmojiList; ///<Type definition for a list of emojis.


#endif // #ifndef BEEFTEXT_EMOJI_H
