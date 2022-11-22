/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of input manager
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "InputManager.h"
#include "Preferences/PreferencesManager.h"
#include "ProcessListManager.h"
#include "MainWindow.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"
#include "KeyboardMapper.h"
#include <XMiLib/Exception.h>


using namespace xmilib;


namespace {

qint32 constexpr kTextBufferSize = 10;
///< The size of the buffer that will receive the text resulting from the processing of the key stroke


//****************************************************************************************************************************************************
/// \brief Retrieve the Input local of the currently focused window.
///
/// \param[out] outHkl If the function returns true, this variable holds the input local of the foreground window. If
/// the function returns false, the value of this variable is undetermined on function exit.
/// \return true if and only if the input local of the foreground window could be determined.
//****************************************************************************************************************************************************
bool getForegroundWindowInputLocale(HKL &outHkl) {
    // ReSharper disable once CppLocalVariableMayBeConst
    HWND hwnd = GetForegroundWindow();
    if (!hwnd)
        return false;
    outHkl = GetKeyboardLayout(GetWindowThreadProcessId(hwnd, nullptr));
    return true;
}


//****************************************************************************************************************************************************
/// \brief Check if a keystroke is the manual substitution shortcut.
///
/// \return true if and only if keystroke correspond to the shortcut.
//****************************************************************************************************************************************************
bool isComboTriggerShortcut(SpShortcut const &shortcut) {
    SpShortcut const comboTriggerShortcut = PreferencesManager::instance().comboTriggerShortcut();
    return shortcut && comboTriggerShortcut && (*shortcut == *comboTriggerShortcut);
}


//****************************************************************************************************************************************************
/// \brief Check if a shortcut is the enable/disable shortcut.
///
/// \return true if and only if keystroke correspond to the shortcut
//****************************************************************************************************************************************************
bool isAppEnableDisableShortcut(SpShortcut const &shortcut) {
    SpShortcut const appEnableDisableShortcut = PreferencesManager::instance().appEnableDisableShortcut();
    return shortcut && appEnableDisableShortcut && (*shortcut == *appEnableDisableShortcut);
}


//****************************************************************************************************************************************************
/// \param[in] vkCode The virtual key code.
/// \return true if the key is currently pressed.
//****************************************************************************************************************************************************
bool isKeyPressed(quint16 vkCode) {
    return GetKeyState(vkCode) & 0x80;
}


//****************************************************************************************************************************************************
/// \param[in] keyEvent The Windows-specific key event.
/// \return A null shortcut if the event is produce by a modifier key
/// \return The shortcut.
//****************************************************************************************************************************************************
SpShortcut shortcutFromWindowsKeyEvent(KBDLLHOOKSTRUCT const *keyEvent) {
    if (!keyEvent)
        return nullptr;
    Qt::KeyboardModifiers mods = Qt::NoModifier;
    if (isKeyPressed(VK_SHIFT) || isKeyPressed(VK_LSHIFT) || isKeyPressed(VK_RSHIFT))
        mods |= Qt::ShiftModifier;
    if (isKeyPressed(VK_CONTROL) || isKeyPressed(VK_LCONTROL) || isKeyPressed(VK_RCONTROL))
        mods |= Qt::ControlModifier;
    if (isKeyPressed(VK_MENU) || isKeyPressed(VK_LMENU) || isKeyPressed(VK_RMENU))
        mods |= Qt::AltModifier;
    if (isKeyPressed(VK_RWIN) || isKeyPressed(VK_LWIN))
        mods |= Qt::MetaModifier;
    if (!((mods & Qt::ControlModifier) || (mods & Qt::AltModifier) || (mods & Qt::MetaModifier)))
        return nullptr;

    Qt::Key const key = KeyboardMapper::instance().virtualKeyCodeToQtKey(keyEvent->vkCode);
    if (key == Qt::Key_unknown)
        return nullptr;
    QList<Qt::Key> const modKeys = { Qt::Key_Shift, Qt::Key_Control, Qt::Key_Meta, Qt::Key_Alt, Qt::Key_CapsLock };
    for (Qt::Key const modKey: modKeys)
        if (key == modKey)
            return nullptr;
    return Shortcut::fromModifiersAndKey(mods, key);
}


}


//****************************************************************************************************************************************************
/// This static member function is registered to be called whenever a key event occurs.
/// 
/// \param[in] nCode A code the hook procedure uses to determine how to process the message
/// \param[in] wParam The identifier of the keyboard message
/// \param[in] lParam A pointer to a KBDLLHOOKSTRUCT structure.
//****************************************************************************************************************************************************
LRESULT CALLBACK InputManager::keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam) {
    static ProcessListManager const &processListManager = globals::excludedApplications();
    if (processListManager.filter(getActiveExecutableFileName()))
        return CallNextHookEx(nullptr, nCode, wParam, lParam); // The active app is listed as excluded

    if ((WM_KEYDOWN != wParam) && (WM_SYSKEYDOWN != wParam))
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    KeyStroke keyStroke = { 0, 0, { 0 }};
    KBDLLHOOKSTRUCT const *keyEvent = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam); // NOLINT(performance-no-int-to-ptr)
    if (!keyEvent)
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    SpShortcut const shortcut = shortcutFromWindowsKeyEvent(keyEvent);
    if (shortcut && shortcut->isValid())
        emit instance().shortcutPressed(shortcut); // note that we do not return here, as some combination (e.g. Ctrl+Alt+5 or AltGr+5) may actually lead to some text

    if (!PreferencesManager::instance().beeftextEnabled())
        return CallNextHookEx(nullptr, nCode, wParam, lParam);

    // we ignore shift / caps lock key events
    if ((keyEvent->vkCode == VK_LSHIFT) || (keyEvent->vkCode == VK_RSHIFT) || (keyEvent->vkCode == VK_CAPITAL))
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    keyStroke.virtualKey = keyEvent->vkCode;
    keyStroke.scanCode = keyEvent->scanCode;
    // GetKeyboardState() do not properly report state for modifier keys if the key event in a window other that one
    // from the current process, so we need to manually fetch the valid states manually using GetKeyState()
    // We do not actually need the state of the other key, so we do not event bother calling GetKeyboardState()
    QList<quint32> keyList = {
        VK_SHIFT, VK_LSHIFT, VK_RSHIFT, VK_CONTROL, VK_LCONTROL,
        VK_RCONTROL, VK_MENU, VK_LMENU, VK_RMENU, VK_RWIN, VK_LWIN, VK_CAPITAL
    };
    for (quint32 const key: keyList)
        keyStroke.keyboardState[key] = static_cast<quint8>(GetKeyState(static_cast<quint16>(key)));

    // our event handler will return false if we want to 'intercept' the keystroke and not pass it to the next hook,
    // but the MSDN documentation says we MUST do it if nCode < 0
    if ((!instance().onKeyboardEvent(keyStroke)) && (nCode >= 0))
        return 0;
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}


//****************************************************************************************************************************************************
/// This static member function is registered to be called whenever a mouse event occurs.
/// 
/// \param[in] nCode A code the hook procedure uses to determine how to process the message
/// \param[in] wParam The identifier of the mouse message
/// \param[in] lParam A pointer to a M structure.
//****************************************************************************************************************************************************
LRESULT CALLBACK InputManager::mouseProcedure(int nCode, WPARAM wParam, LPARAM lParam) {
    if (!PreferencesManager::instance().beeftextEnabled())
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    if ((WM_LBUTTONDOWN == wParam) || (WM_RBUTTONDOWN == wParam) || (WM_MOUSEWHEEL == wParam) ||
        (WM_MBUTTONDOWN == wParam)) // note we consider mouse wheel moves as clicks
    {
        instance().onMouseClickEvent(nCode, wParam, lParam);
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}


//****************************************************************************************************************************************************
/// \return The only allowed instance of the class
//****************************************************************************************************************************************************
InputManager &InputManager::instance() {
    static InputManager instance;
    return instance;
}


//****************************************************************************************************************************************************
/// \
//****************************************************************************************************************************************************
void InputManager::onShortcut(SpShortcut const &shortcut) {
    if ((!shortcut) || (!isShortcutProcessingEnabled_))
        return;
    PreferencesManager const &prefs = PreferencesManager::instance();
    if (prefs.enableAppEnableDisableShortcut() && isAppEnableDisableShortcut(shortcut)) {
        emit appEnableDisableShortcutTriggered();
        return;
    }

    if (!prefs.beeftextEnabled())
        return;

    if ((!prefs.useAutomaticSubstitution()) && isComboTriggerShortcut(shortcut)) {
        emit substitutionShortcutTriggered();
        return;
    }
}


//****************************************************************************************************************************************************
///
//****************************************************************************************************************************************************
InputManager::InputManager()
    : QObject(nullptr), useLegacyKeyProcessing_(!isAppRunningOnWindows10OrHigher()) {
    this->enableKeyboardHook();
    connect(this, &InputManager::shortcutPressed, this, &InputManager::onShortcut);
#ifdef NDEBUG
    // to avoid being locked with all input unresponsive when in debug (because one forgot that breakpoints should be
    // avoided, for instance), we only enable the low level mouse hook in release configuration
    this->enableMouseHook();
#endif
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
InputManager::~InputManager() {
    this->disableKeyboardHook();
    this->disableMouseHook();
}


//****************************************************************************************************************************************************
/// \param[in] keyStroke The key stroke
/// \return true if the event can be passed down to the keyboard hooked chain, and false it it should be removed
//****************************************************************************************************************************************************
bool InputManager::onKeyboardEvent(KeyStroke const &keyStroke) {
    // on some layout (e.g. US International, direction key + alt lead to garbage char if ToUnicode is pressed, so
    // we bypass normal processing for those keys(note this is different for the dead key issue described in
    // processKey().
    QList<quint32> const breakers = {
        VK_UP, VK_RIGHT, VK_DOWN, VK_LEFT, VK_PRIOR, VK_NEXT, VK_HOME, VK_END, VK_INSERT,
        VK_DELETE
    };
    if (breakers.contains(keyStroke.virtualKey)) {
        emit comboBreakerTyped();
        return true;
    }

    bool isDeadKey = false;
    QString text = this->processKey(keyStroke, isDeadKey);
    if (text.isEmpty())
        return true;

    PreferencesManager const &prefs = PreferencesManager::instance();
    for (QChar c: text) {
        if (QChar('\b') == c) {
            emit backspaceTyped();
            continue;
        }
        if (!c.isPrint()) {
            emit comboBreakerTyped();
            continue;
        }
        if (c.isSpace()) {
            if (prefs.comboTriggersOnSpace() && prefs.useAutomaticSubstitution())
                emit characterTyped(c);
            else
                emit comboBreakerTyped();
            continue;
        }
        emit characterTyped(c);
    }
    return true;
}


//****************************************************************************************************************************************************
/// \param[in] keyStroke The key stroke
/// \param[out] outIsDeadKey Is the key a dead key
/// \return The text resulting of the keystroke
//****************************************************************************************************************************************************
QString InputManager::processKey(KeyStroke const &keyStroke, bool &outIsDeadKey) {
    // Windows version before Windows 10 build 1607, there is not option to ensure that ToUnicode() / ToUnicodeEx does
    // not modify the keyboard state, which forces us to perform a special treatment for dead keys.
    return useLegacyKeyProcessing_ ? processKeyLegacy(keyStroke, outIsDeadKey) : processKeyModern(keyStroke);
}


//****************************************************************************************************************************************************
/// \param[in] keyStroke The key stroke
/// \return The text resulting of the keystroke
//****************************************************************************************************************************************************
QString InputManager::processKeyModern(KeyStroke const &keyStroke) {
    // Windows allow each window to have its own input locale, so we try to obtain the locale (HKL) of the active window
    // and pass it to ToUnicodeEx(). If we fail to do so we call ToUnicode instead, which use the system-wide locale
    WCHAR textBuffer[kTextBufferSize];
    HKL hkl = nullptr;
    qint32 const size = getForegroundWindowInputLocale(hkl)
                        ? ToUnicodeEx(keyStroke.virtualKey, keyStroke.scanCode, keyStroke.keyboardState, textBuffer, kTextBufferSize
            , 1 << 2, hkl) : ToUnicode(keyStroke.virtualKey, keyStroke.scanCode, keyStroke.keyboardState, textBuffer
            , kTextBufferSize, 1 << 2);
    return size > 0 ? QString::fromWCharArray(textBuffer, size) : QString();
}


//****************************************************************************************************************************************************
/// \param[in] keyStroke The key stroke
/// \param[out] outIsDeadKey Is the key a dead key
/// \return The text resulting of the keystroke
//****************************************************************************************************************************************************
QString InputManager::processKeyLegacy(KeyStroke const &keyStroke, bool &outIsDeadKey) {
    // The core of this function is the call to ToUnicodeEx() - or ToUnicode() - who transforms a keystroke into
    // an actual text output, taking into account the current input locale (a.k.a. keyboard layout).
    // now the tricky part: ToUnicode() "consumes" the dead key that may be stored in the kernel-mode keyboard buffer
    // so we need to manually restore the dead key by calling ToUnicode() again
    WCHAR textBuffer[kTextBufferSize] = { 0 };
    outIsDeadKey = false;
    // for some unkown reasons, in this legacy code ToUnicodeEx cause failures with dead keys in some locales.
    qint32 const size = ToUnicode(keyStroke.virtualKey, keyStroke.scanCode, keyStroke.keyboardState, textBuffer, kTextBufferSize, 0);

    if (-1 == size) {
        // the key is a dead key. We have consumed it so we need to:
        // 1 - Restore it by repeating the call to ToUnicode()
        // 2 - Save the key because we will need to apply it again before the next 'normal' keystroke
        ToUnicode(keyStroke.virtualKey, keyStroke.scanCode, keyStroke.keyboardState, textBuffer, kTextBufferSize, 0);
        deadKey_ = keyStroke;
        outIsDeadKey = true;
        return QString();
    }

    if (size > 0) {
        // The key is a normal key that will result in text output.
        // if the previous key was a dead key, we have already consumed the dead key so we must restore it
        QString result = QString::fromWCharArray(textBuffer, size);
        if (0 != deadKey_.virtualKey) {
            ToUnicode(deadKey_.virtualKey, deadKey_.scanCode, deadKey_.keyboardState, textBuffer, kTextBufferSize, 0);
            deadKey_.virtualKey = 0;
        }
        return result;
    }

    // final case: size is 0, the key is a modifier, we do nothing
    // values of size < -1 also lead here but should not happen according to the documentation for ToUnicode()
    return QString();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void InputManager::onMouseClickEvent(int, WPARAM, LPARAM) {
    emit comboBreakerTyped();
}


//****************************************************************************************************************************************************
/// \return true if and only if the keyboard hook is enabled
//****************************************************************************************************************************************************
bool InputManager::isKeyboardHookEnable() const {
    return keyboardHook_;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void InputManager::enableKeyboardHook() {
    if (keyboardHook_)
        return;
    HMODULE const moduleHandle = GetModuleHandle(nullptr);
    keyboardHook_ = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProcedure, moduleHandle, 0);
    if (!keyboardHook_)
        throw Exception("Could not register a keyboard hook.");
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void InputManager::disableKeyboardHook() {
    if (keyboardHook_) {
        UnhookWindowsHookEx(keyboardHook_);
        keyboardHook_ = nullptr;
    }
}


//****************************************************************************************************************************************************
/// \param[in] enabled The new state of the keyboard hook
/// \return true if and only if the keyboard hook was enabled before the call
//****************************************************************************************************************************************************
bool InputManager::setKeyboardHookEnabled(bool enabled) {
    bool const result = keyboardHook_;
    if (enabled)
        this->enableKeyboardHook();
    else
        this->disableKeyboardHook();
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] enabled Should shortcut processing be enabled.
//****************************************************************************************************************************************************
void InputManager::setShortcutsProcessingEnabled(bool enabled) {
    isShortcutProcessingEnabled_ = enabled;
}


//****************************************************************************************************************************************************
/// \return true if and only if shortcut processing is enabled.
//****************************************************************************************************************************************************
bool InputManager::isShortcutProcessingEnabled() const {
    return isShortcutProcessingEnabled_;
}


//****************************************************************************************************************************************************
/// \return true if and only if the mouse hook is enabled
//****************************************************************************************************************************************************
bool InputManager::isMouseHookEnabled() const {
    return mouseHook_;
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void InputManager::enableMouseHook() {
    if (mouseHook_)
        return;
    HMODULE const moduleHandle = GetModuleHandle(nullptr);
    mouseHook_ = SetWindowsHookEx(WH_MOUSE_LL, mouseProcedure, moduleHandle, 0);
    if (!mouseHook_)
        throw Exception("Could not register a mouse hook.");
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void InputManager::disableMouseHook() {
    if (mouseHook_) {
        UnhookWindowsHookEx(mouseHook_);
        mouseHook_ = nullptr;
    }
}


//****************************************************************************************************************************************************
/// \param[in] enabled The new state of the mouse hook
/// \return true if and only if the mouse hook was enabled before the call 
//****************************************************************************************************************************************************
bool InputManager::setMouseHookEnabled(bool enabled) {
    bool const result = mouseHook_;
    if (enabled)
        this->enableMouseHook();
    else
        this->disableMouseHook();
    return result;
}
