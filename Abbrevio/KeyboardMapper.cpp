/// \file
/// \author 
///
/// \brief Implementation of keyboard related functions
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "KeyboardMapper.h"
#include <XMiLib/Exception.h>


namespace {

qint32 constexpr kVirtualKeyCount = 256; ///< The number of virtual keys

// the base table for mapping. This is part of the Qt source code (plugins/platforms/windows/qwindowskeymapper.cpp),
// but it is not exposed. This table contains 0 for the keys who's mapping depend on the keyboard layout.
QList<quint32> const kBaseKeyTable = {
    // Keyboard mapping table
    // Dec |  Hex | Windows Virtual key
    Qt::Key_unknown, //   0   0x00
    Qt::Key_unknown, //   1   0x01   VK_LBUTTON          | Left mouse button
    Qt::Key_unknown, //   2   0x02   VK_RBUTTON          | Right mouse button
    Qt::Key_Cancel, //   3   0x03   VK_CANCEL           | Control-Break processing
    Qt::Key_unknown, //   4   0x04   VK_MBUTTON          | Middle mouse button
    Qt::Key_unknown, //   5   0x05   VK_XBUTTON1         | X1 mouse button
    Qt::Key_unknown, //   6   0x06   VK_XBUTTON2         | X2 mouse button
    Qt::Key_unknown, //   7   0x07   -- unassigned --
    Qt::Key_Backspace, //   8   0x08   VK_BACK             | BackSpace key
    Qt::Key_Tab, //   9   0x09   VK_TAB              | Tab key
    Qt::Key_unknown, //  10   0x0A   -- reserved --
    Qt::Key_unknown, //  11   0x0B   -- reserved --
    Qt::Key_Clear, //  12   0x0C   VK_CLEAR            | Clear key
    Qt::Key_Enter, //  13   0x0D   VK_RETURN           | Enter key
    Qt::Key_unknown, //  14   0x0E   -- unassigned --
    Qt::Key_unknown, //  15   0x0F   -- unassigned --
    Qt::Key_Shift, //  16   0x10   VK_SHIFT            | Shift key
    Qt::Key_Control, //  17   0x11   VK_CONTROL          | Ctrl key
    Qt::Key_Alt, //  18   0x12   VK_MENU             | Alt key
    Qt::Key_Pause, //  19   0x13   VK_PAUSE            | Pause key
    Qt::Key_CapsLock, //  20   0x14   VK_CAPITAL          | Caps-Lock
    Qt::Key_unknown, //  21   0x15   VK_KANA / VK_HANGUL | IME Kana or Hangul mode
    Qt::Key_unknown, //  22   0x16   -- unassigned --
    Qt::Key_unknown, //  23   0x17   VK_JUNJA            | IME Junja mode
    Qt::Key_unknown, //  24   0x18   VK_FINAL            | IME final mode
    Qt::Key_unknown, //  25   0x19   VK_HANJA / VK_KANJI | IME Hanja or Kanji mode
    Qt::Key_unknown, //  26   0x1A   -- unassigned --
    Qt::Key_Escape, //  27   0x1B   VK_ESCAPE           | Esc key
    Qt::Key_unknown, //  28   0x1C   VK_CONVERT          | IME convert
    Qt::Key_unknown, //  29   0x1D   VK_NONCONVERT       | IME non-convert
    Qt::Key_unknown, //  30   0x1E   VK_ACCEPT           | IME accept
    Qt::Key_Mode_switch, //  31   0x1F   VK_MODECHANGE       | IME mode change request
    Qt::Key_Space, //  32   0x20   VK_SPACE            | Spacebar
    Qt::Key_PageUp, //  33   0x21   VK_PRIOR            | Page Up key
    Qt::Key_PageDown, //  34   0x22   VK_NEXT             | Page Down key
    Qt::Key_End, //  35   0x23   VK_END              | End key
    Qt::Key_Home, //  36   0x24   VK_HOME             | Home key
    Qt::Key_Left, //  37   0x25   VK_LEFT             | Left arrow key
    Qt::Key_Up, //  38   0x26   VK_UP               | Up arrow key
    Qt::Key_Right, //  39   0x27   VK_RIGHT            | Right arrow key
    Qt::Key_Down, //  40   0x28   VK_DOWN             | Down arrow key
    Qt::Key_Select, //  41   0x29   VK_SELECT           | Select key
    Qt::Key_Printer, //  42   0x2A   VK_PRINT            | Print key
    Qt::Key_Execute, //  43   0x2B   VK_EXECUTE          | Execute key
    Qt::Key_Print, //  44   0x2C   VK_SNAPSHOT         | Print Screen key
    Qt::Key_Insert, //  45   0x2D   VK_INSERT           | Ins key
    Qt::Key_Delete, //  46   0x2E   VK_DELETE           | Del key
    Qt::Key_Help, //  47   0x2F   VK_HELP             | Help key
    0, //  48   0x30   (VK_0)              | 0 key
    0, //  49   0x31   (VK_1)              | 1 key
    0, //  50   0x32   (VK_2)              | 2 key
    0, //  51   0x33   (VK_3)              | 3 key
    0, //  52   0x34   (VK_4)              | 4 key
    0, //  53   0x35   (VK_5)              | 5 key
    0, //  54   0x36   (VK_6)              | 6 key
    0, //  55   0x37   (VK_7)              | 7 key
    0, //  56   0x38   (VK_8)              | 8 key
    0, //  57   0x39   (VK_9)              | 9 key
    Qt::Key_unknown, //  58   0x3A   -- unassigned --
    Qt::Key_unknown, //  59   0x3B   -- unassigned --
    Qt::Key_unknown, //  60   0x3C   -- unassigned --
    Qt::Key_unknown, //  61   0x3D   -- unassigned --
    Qt::Key_unknown, //  62   0x3E   -- unassigned --
    Qt::Key_unknown, //  63   0x3F   -- unassigned --
    Qt::Key_unknown, //  64   0x40   -- unassigned --
    0, //  65   0x41   (VK_A)              | A key
    0, //  66   0x42   (VK_B)              | B key
    0, //  67   0x43   (VK_C)              | C key
    0, //  68   0x44   (VK_D)              | D key
    0, //  69   0x45   (VK_E)              | E key
    0, //  70   0x46   (VK_F)              | F key
    0, //  71   0x47   (VK_G)              | G key
    0, //  72   0x48   (VK_H)              | H key
    0, //  73   0x49   (VK_I)              | I key
    0, //  74   0x4A   (VK_J)              | J key
    0, //  75   0x4B   (VK_K)              | K key
    0, //  76   0x4C   (VK_L)              | L key
    0, //  77   0x4D   (VK_M)              | M key
    0, //  78   0x4E   (VK_N)              | N key
    0, //  79   0x4F   (VK_O)              | O key
    0, //  80   0x50   (VK_P)              | P key
    0, //  81   0x51   (VK_Q)              | Q key
    0, //  82   0x52   (VK_R)              | R key
    0, //  83   0x53   (VK_S)              | S key
    0, //  84   0x54   (VK_T)              | T key
    0, //  85   0x55   (VK_U)              | U key
    0, //  86   0x56   (VK_V)              | V key
    0, //  87   0x57   (VK_W)              | W key
    0, //  88   0x58   (VK_X)              | X key
    0, //  89   0x59   (VK_Y)              | Y key
    0, //  90   0x5A   (VK_Z)              | Z key
    Qt::Key_Meta, //  91   0x5B   VK_LWIN             | Left Windows  - MS Natural kbd
    Qt::Key_Meta, //  92   0x5C   VK_RWIN             | Right Windows - MS Natural kbd
    Qt::Key_Menu, //  93   0x5D   VK_APPS             | Application key-MS Natural kbd
    Qt::Key_unknown, //  94   0x5E   -- reserved --
    Qt::Key_Sleep, //  95   0x5F   VK_SLEEP
    Qt::Key_0, //  96   0x60   VK_NUMPAD0          | Numeric keypad 0 key
    Qt::Key_1, //  97   0x61   VK_NUMPAD1          | Numeric keypad 1 key
    Qt::Key_2, //  98   0x62   VK_NUMPAD2          | Numeric keypad 2 key
    Qt::Key_3, //  99   0x63   VK_NUMPAD3          | Numeric keypad 3 key
    Qt::Key_4, // 100   0x64   VK_NUMPAD4          | Numeric keypad 4 key
    Qt::Key_5, // 101   0x65   VK_NUMPAD5          | Numeric keypad 5 key
    Qt::Key_6, // 102   0x66   VK_NUMPAD6          | Numeric keypad 6 key
    Qt::Key_7, // 103   0x67   VK_NUMPAD7          | Numeric keypad 7 key
    Qt::Key_8, // 104   0x68   VK_NUMPAD8          | Numeric keypad 8 key
    Qt::Key_9, // 105   0x69   VK_NUMPAD9          | Numeric keypad 9 key
    Qt::Key_Asterisk, // 106   0x6A   VK_MULTIPLY         | Multiply key
    Qt::Key_Plus, // 107   0x6B   VK_ADD              | Add key
    Qt::Key_unknown, // 108   0x6C   VK_SEPARATOR        | Separator key (locale-dependent)
    Qt::Key_Minus, // 109   0x6D   VK_SUBTRACT         | Subtract key
    Qt::Key_unknown, // 110   0x6E   VK_DECIMAL          | Decimal key (locale-dependent)
    Qt::Key_Slash, // 111   0x6F   VK_DIVIDE           | Divide key
    Qt::Key_F1, // 112   0x70   VK_F1               | F1 key
    Qt::Key_F2, // 113   0x71   VK_F2               | F2 key
    Qt::Key_F3, // 114   0x72   VK_F3               | F3 key
    Qt::Key_F4, // 115   0x73   VK_F4               | F4 key
    Qt::Key_F5, // 116   0x74   VK_F5               | F5 key
    Qt::Key_F6, // 117   0x75   VK_F6               | F6 key
    Qt::Key_F7, // 118   0x76   VK_F7               | F7 key
    Qt::Key_F8, // 119   0x77   VK_F8               | F8 key
    Qt::Key_F9, // 120   0x78   VK_F9               | F9 key
    Qt::Key_F10, // 121   0x79   VK_F10              | F10 key
    Qt::Key_F11, // 122   0x7A   VK_F11              | F11 key
    Qt::Key_F12, // 123   0x7B   VK_F12              | F12 key
    Qt::Key_F13, // 124   0x7C   VK_F13              | F13 key
    Qt::Key_F14, // 125   0x7D   VK_F14              | F14 key
    Qt::Key_F15, // 126   0x7E   VK_F15              | F15 key
    Qt::Key_F16, // 127   0x7F   VK_F16              | F16 key
    Qt::Key_F17, // 128   0x80   VK_F17              | F17 key
    Qt::Key_F18, // 129   0x81   VK_F18              | F18 key
    Qt::Key_F19, // 130   0x82   VK_F19              | F19 key
    Qt::Key_F20, // 131   0x83   VK_F20              | F20 key
    Qt::Key_F21, // 132   0x84   VK_F21              | F21 key
    Qt::Key_F22, // 133   0x85   VK_F22              | F22 key
    Qt::Key_F23, // 134   0x86   VK_F23              | F23 key
    Qt::Key_F24, // 135   0x87   VK_F24              | F24 key
    Qt::Key_unknown, // 136   0x88   -- unassigned --
    Qt::Key_unknown, // 137   0x89   -- unassigned --
    Qt::Key_unknown, // 138   0x8A   -- unassigned --
    Qt::Key_unknown, // 139   0x8B   -- unassigned --
    Qt::Key_unknown, // 140   0x8C   -- unassigned --
    Qt::Key_unknown, // 141   0x8D   -- unassigned --
    Qt::Key_unknown, // 142   0x8E   -- unassigned --
    Qt::Key_unknown, // 143   0x8F   -- unassigned --
    Qt::Key_NumLock, // 144   0x90   VK_NUMLOCK          | Num Lock key
    Qt::Key_ScrollLock, // 145   0x91   VK_SCROLL           | Scroll Lock key
    // Fujitsu/OASYS kbd --------------------
    0, //Qt::Key_Jisho, // 146   0x92   VK_OEM_FJ_JISHO     | 'Dictionary' key /
    //              VK_OEM_NEC_EQUAL  = key on numpad on NEC PC-9800 kbd
    Qt::Key_Massyo, // 147   0x93   VK_OEM_FJ_MASSHOU   | 'Unregister word' key
    Qt::Key_Touroku, // 148   0x94   VK_OEM_FJ_TOUROKU   | 'Register word' key
    0, //Qt::Key_Oyayubi_Left,//149   0x95  VK_OEM_FJ_LOYA  | 'Left OYAYUBI' key
    0, //Qt::Key_Oyayubi_Right,//150  0x96  VK_OEM_FJ_ROYA  | 'Right OYAYUBI' key
    Qt::Key_unknown, // 151   0x97   -- unassigned --
    Qt::Key_unknown, // 152   0x98   -- unassigned --
    Qt::Key_unknown, // 153   0x99   -- unassigned --
    Qt::Key_unknown, // 154   0x9A   -- unassigned --
    Qt::Key_unknown, // 155   0x9B   -- unassigned --
    Qt::Key_unknown, // 156   0x9C   -- unassigned --
    Qt::Key_unknown, // 157   0x9D   -- unassigned --
    Qt::Key_unknown, // 158   0x9E   -- unassigned --
    Qt::Key_unknown, // 159   0x9F   -- unassigned --
    Qt::Key_Shift, // 160   0xA0   VK_LSHIFT           | Left Shift key
    Qt::Key_Shift, // 161   0xA1   VK_RSHIFT           | Right Shift key
    Qt::Key_Control, // 162   0xA2   VK_LCONTROL         | Left Ctrl key
    Qt::Key_Control, // 163   0xA3   VK_RCONTROL         | Right Ctrl key
    Qt::Key_Alt, // 164   0xA4   VK_LMENU            | Left Menu key
    Qt::Key_Alt, // 165   0xA5   VK_RMENU            | Right Menu key
    Qt::Key_Back, // 166   0xA6   VK_BROWSER_BACK     | Browser Back key
    Qt::Key_Forward, // 167   0xA7   VK_BROWSER_FORWARD  | Browser Forward key
    Qt::Key_Refresh, // 168   0xA8   VK_BROWSER_REFRESH  | Browser Refresh key
    Qt::Key_Stop, // 169   0xA9   VK_BROWSER_STOP     | Browser Stop key
    Qt::Key_Search, // 170   0xAA   VK_BROWSER_SEARCH   | Browser Search key
    Qt::Key_Favorites, // 171   0xAB   VK_BROWSER_FAVORITES| Browser Favorites key
    Qt::Key_HomePage, // 172   0xAC   VK_BROWSER_HOME     | Browser Start and Home key
    Qt::Key_VolumeMute, // 173   0xAD   VK_VOLUME_MUTE      | Volume Mute key
    Qt::Key_VolumeDown, // 174   0xAE   VK_VOLUME_DOWN      | Volume Down key
    Qt::Key_VolumeUp, // 175   0xAF   VK_VOLUME_UP        | Volume Up key
    Qt::Key_MediaNext, // 176   0xB0   VK_MEDIA_NEXT_TRACK | Next Track key
    Qt::Key_MediaPrevious, //177 0xB1   VK_MEDIA_PREV_TRACK | Previous Track key
    Qt::Key_MediaStop, // 178   0xB2   VK_MEDIA_STOP       | Stop Media key
    Qt::Key_MediaTogglePlayPause,
    // 179   0xB3   VK_MEDIA_PLAY_PAUSE | Play/Pause Media key
    Qt::Key_LaunchMail, // 180   0xB4   VK_LAUNCH_MAIL      | Start Mail key
    Qt::Key_LaunchMedia, // 181   0xB5   VK_LAUNCH_MEDIA_SELECT Select Media key
    Qt::Key_Launch0, // 182   0xB6   VK_LAUNCH_APP1      | Start Application 1 key
    Qt::Key_Launch1, // 183   0xB7   VK_LAUNCH_APP2      | Start Application 2 key
    Qt::Key_unknown, // 184   0xB8   -- reserved --
    Qt::Key_unknown, // 185   0xB9   -- reserved --
    0, // 186   0xBA   VK_OEM_1            | ';:' for US
    0, // 187   0xBB   VK_OEM_PLUS         | '+' any country
    0, // 188   0xBC   VK_OEM_COMMA        | ',' any country
    0, // 189   0xBD   VK_OEM_MINUS        | '-' any country
    0, // 190   0xBE   VK_OEM_PERIOD       | '.' any country
    0, // 191   0xBF   VK_OEM_2            | '/?' for US
    0, // 192   0xC0   VK_OEM_3            | '`~' for US
    Qt::Key_unknown, // 193   0xC1   -- reserved --
    Qt::Key_unknown, // 194   0xC2   -- reserved --
    Qt::Key_unknown, // 195   0xC3   -- reserved --
    Qt::Key_unknown, // 196   0xC4   -- reserved --
    Qt::Key_unknown, // 197   0xC5   -- reserved --
    Qt::Key_unknown, // 198   0xC6   -- reserved --
    Qt::Key_unknown, // 199   0xC7   -- reserved --
    Qt::Key_unknown, // 200   0xC8   -- reserved --
    Qt::Key_unknown, // 201   0xC9   -- reserved --
    Qt::Key_unknown, // 202   0xCA   -- reserved --
    Qt::Key_unknown, // 203   0xCB   -- reserved --
    Qt::Key_unknown, // 204   0xCC   -- reserved --
    Qt::Key_unknown, // 205   0xCD   -- reserved --
    Qt::Key_unknown, // 206   0xCE   -- reserved --
    Qt::Key_unknown, // 207   0xCF   -- reserved --
    Qt::Key_unknown, // 208   0xD0   -- reserved --
    Qt::Key_unknown, // 209   0xD1   -- reserved --
    Qt::Key_unknown, // 210   0xD2   -- reserved --
    Qt::Key_unknown, // 211   0xD3   -- reserved --
    Qt::Key_unknown, // 212   0xD4   -- reserved --
    Qt::Key_unknown, // 213   0xD5   -- reserved --
    Qt::Key_unknown, // 214   0xD6   -- reserved --
    Qt::Key_unknown, // 215   0xD7   -- reserved --
    Qt::Key_unknown, // 216   0xD8   -- unassigned --
    Qt::Key_unknown, // 217   0xD9   -- unassigned --
    Qt::Key_unknown, // 218   0xDA   -- unassigned --
    0, // 219   0xDB   VK_OEM_4            | '[{' for US
    0, // 220   0xDC   VK_OEM_5            | '\|' for US
    0, // 221   0xDD   VK_OEM_6            | ']}' for US
    0, // 222   0xDE   VK_OEM_7            | ''"' for US
    0, // 223   0xDF   VK_OEM_8
    Qt::Key_unknown, // 224   0xE0   -- reserved --
    Qt::Key_unknown, // 225   0xE1   VK_OEM_AX           | 'AX' key on Japanese AX kbd
    Qt::Key_unknown, // 226   0xE2   VK_OEM_102          | "<>" or "\|" on RT 102-key kbd
    Qt::Key_unknown, // 227   0xE3   VK_ICO_HELP         | Help key on ICO
    Qt::Key_unknown, // 228   0xE4   VK_ICO_00           | 00 key on ICO
    Qt::Key_unknown, // 229   0xE5   VK_PROCESSKEY       | IME Process key
    Qt::Key_unknown, // 230   0xE6   VK_ICO_CLEAR        |
    Qt::Key_unknown, // 231   0xE7   VK_PACKET           | Unicode char as keystrokes
    Qt::Key_unknown, // 232   0xE8   -- unassigned --
    // Nokia/Ericsson definitions ---------------
    Qt::Key_unknown, // 233   0xE9   VK_OEM_RESET
    Qt::Key_unknown, // 234   0xEA   VK_OEM_JUMP
    Qt::Key_unknown, // 235   0xEB   VK_OEM_PA1
    Qt::Key_unknown, // 236   0xEC   VK_OEM_PA2
    Qt::Key_unknown, // 237   0xED   VK_OEM_PA3
    Qt::Key_unknown, // 238   0xEE   VK_OEM_WSCTRL
    Qt::Key_unknown, // 239   0xEF   VK_OEM_CUSEL
    Qt::Key_unknown, // 240   0xF0   VK_OEM_ATTN
    Qt::Key_unknown, // 241   0xF1   VK_OEM_FINISH
    Qt::Key_unknown, // 242   0xF2   VK_OEM_COPY
    Qt::Key_unknown, // 243   0xF3   VK_OEM_AUTO
    Qt::Key_unknown, // 244   0xF4   VK_OEM_ENLW
    Qt::Key_unknown, // 245   0xF5   VK_OEM_BACKTAB
    Qt::Key_unknown, // 246   0xF6   VK_ATTN             | Attn key
    Qt::Key_unknown, // 247   0xF7   VK_CRSEL            | CrSel key
    Qt::Key_unknown, // 248   0xF8   VK_EXSEL            | ExSel key
    Qt::Key_unknown, // 249   0xF9   VK_EREOF            | Erase EOF key
    Qt::Key_Play, // 250   0xFA   VK_PLAY             | Play key
    Qt::Key_Zoom, // 251   0xFB   VK_ZOOM             | Zoom key
    Qt::Key_unknown, // 252   0xFC   VK_NONAME           | Reserved
    Qt::Key_unknown, // 253   0xFD   VK_PA1              | PA1 key
    Qt::Key_Clear, // 254   0xFE   VK_OEM_CLEAR        | Clear key
    0
};


}


//****************************************************************************************************************************************************
/// \return the only allowed instance of the class.
//****************************************************************************************************************************************************
KeyboardMapper &KeyboardMapper::instance() {
    static KeyboardMapper instance;
    return instance;
}


//****************************************************************************************************************************************************
/// \param[in] vkCode The virtual key code
/// \return The Qt key for a given virtual key code
//****************************************************************************************************************************************************
Qt::Key KeyboardMapper::virtualKeyCodeToQtKey(quint32 vkCode) {
    return qtKeysForVirtualKeys_[vkCode];
}


//****************************************************************************************************************************************************
/// \param[in] key The Qt key.
/// \return The virtual key code corresponding to the key or 0 if no virtual key code matches the Qt key.
//****************************************************************************************************************************************************
quint32 KeyboardMapper::qtKeyToVirtualKeyCode(Qt::Key const &key) {
    return virtualKeysForQtKeys_.contains(key) ? virtualKeysForQtKeys_[key] : 0;
}


//****************************************************************************************************************************************************
/// \param[in] modifiers The native modifiers bitfield.
/// \return The Qt modifier bitfield
//****************************************************************************************************************************************************
Qt::KeyboardModifiers KeyboardMapper::nativeModifiersToQtModifiers(quint32 modifiers) {
    Qt::KeyboardModifiers result;
    if (modifiers & MOD_CONTROL)
        result.setFlag(Qt::ControlModifier);
    if (modifiers & MOD_ALT)
        result.setFlag(Qt::AltModifier);
    if (modifiers & MOD_WIN)
        result.setFlag(Qt::MetaModifier);
    if (modifiers & MOD_SHIFT)
        result.setFlag(Qt::ShiftModifier);
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] modifiers The Qt modifiers bitfield.
/// \return The native modifiers bitfield.
//****************************************************************************************************************************************************
quint32 KeyboardMapper::qtModifiersToNativeModifiers(Qt::KeyboardModifiers const &modifiers) {
    quint32 result = 0;
    if (modifiers.testFlag(Qt::ControlModifier))
        result |= MOD_CONTROL;
    if (modifiers.testFlag(Qt::AltModifier))
        result |= MOD_ALT;
    if (modifiers.testFlag(Qt::MetaModifier))
        result |= MOD_WIN;
    if (modifiers.testFlag(Qt::ShiftModifier))
        result |= MOD_SHIFT;
    return result;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
KeyboardMapper::KeyboardMapper() {
    this->computeMappings();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void KeyboardMapper::computeMappings() {
    Q_ASSERT(kBaseKeyTable.size() == kVirtualKeyCount);
    if (kBaseKeyTable.size() != kVirtualKeyCount)
        throw xmilib::Exception("The base key mapping table is invalid.");
    qtKeysForVirtualKeys_.resize(kVirtualKeyCount);
    virtualKeysForQtKeys_.clear();
    for (qint32 vkCode = 0; vkCode < kVirtualKeyCount; ++vkCode) {
        quint32 const key = kBaseKeyTable[vkCode];
        if ((key != 0) && (key != Qt::Key_unknown)) {
            qtKeysForVirtualKeys_[vkCode] = Qt::Key(key);
            virtualKeysForQtKeys_[Qt::Key(key)] = vkCode;
            continue;
        }
        if (key == Qt::Key_unknown) {
            qtKeysForVirtualKeys_[vkCode] = Qt::Key_unknown;
            // note: no mapping for qtUnknown in virtualKeysForQtKeys_
            continue;
        }
        // key == 0 the key is a printable one. We use ToUnicode to determine the actual character.
        // it's obvious for letters and numbers, but virtual key codes for other characters ( [] , {} , !, ...)
        // depend on the keyboard layout.
        BYTE constexpr zeroes[256] = { 0 };
        qsizetype constexpr textBufferSize = 10;
        WCHAR textBuffer[textBufferSize] = { 0 };
        ToUnicode(vkCode, MapVirtualKey(vkCode, MAPVK_VK_TO_VSC), zeroes, textBuffer, textBufferSize, 1 << 2);

        Qt::Key const qtKey = Qt::Key(QChar(textBuffer[0]).toUpper().unicode());
        qtKeysForVirtualKeys_[vkCode] = qtKey;
        virtualKeysForQtKeys_[qtKey] = vkCode;
    }
}
