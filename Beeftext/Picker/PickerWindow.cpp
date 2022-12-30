/// \file
/// \author 
///
/// \brief Implementation of picker window.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PickerWindow.h"
#include "PickerItemDelegate.h"
#include "Emoji/EmojiManager.h"
#include "Combo/ComboManager.h"
#include "Clipboard/ClipboardManager.h"
#include "Preferences/PreferencesManager.h"
#include "BeeftextUtils.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include "KeyboardMapper.h"
#include <XMiLib/GlobalShortcut/GlobalShortcutManager.h>


//****************************************************************************************************************************************************
/// \return true if the preference was successfully applied
//****************************************************************************************************************************************************
bool applyComboPickerPreferences() {
    xmilib::GlobalShortcutManager &scManager = xmilib::GlobalShortcutManager::instance();
    PreferencesManager const &prefs = PreferencesManager::instance();
    scManager.reset();
    if (!prefs.comboPickerEnabled())
        return true;

    SpShortcut shortcut = prefs.comboPickerShortcut();
    if (!shortcut)
        shortcut = prefs.defaultComboPickerShortcut();
    KeyboardMapper &mapper = KeyboardMapper::instance();
    xmilib::GlobalShortcut const *sc = scManager.create(mapper.qtModifiersToNativeModifiers(shortcut->keyboardModifiers()), mapper.qtKeyToVirtualKeyCode(shortcut->key()));
    if (!sc)
        return false;
    QObject::connect(sc, &xmilib::GlobalShortcut::triggered, []() { showComboPickerWindow(); });
    return true;
}


//****************************************************************************************************************************************************
/// \brief Return the screen currently containing the cursor
///
/// \return The screen containing the cursor. If it could not be determined, the primary screen is returned
//****************************************************************************************************************************************************
QScreen *screenContainingCursor() {
    QPoint const pos = QCursor::pos();
    for (QScreen *screen: qApp->screens())
        if (screen && screen->geometry().contains(pos))
            return screen;
    return qApp->primaryScreen();
}


//****************************************************************************************************************************************************
/// \brief return the rectangle for a window.
///
/// \param[in] hwnd The window handle.
/// \return The handle for the window.
//****************************************************************************************************************************************************
QRect rectForHwnd(HWND hwnd) {
    RECT r;
    return GetWindowRect(hwnd, &r) ? QRect(QPoint(r.left, r.top), QPoint(r.right, r.bottom)) : QRect();
}


//****************************************************************************************************************************************************
/// \brief Check whether a given window handle is the desktop.
/// \param[in] hwnd The handle of the window to test
/// \return true if the handle is the desktop handle
//****************************************************************************************************************************************************
bool isDesktop(HWND hwnd) {
    // Logic taken from https://stackoverflow.com/questions/8364758/get-handle-to-desktop-shell-window
    WCHAR array[256];
    if (0 == GetClassName(hwnd, array, 256))
        return false;
    QString const className = QString::fromWCharArray(array);
    return (className.compare("Progman", Qt::CaseInsensitive) == 0) ||
           (className.compare("WorkerW", Qt::CaseInsensitive) == 0);
}


//****************************************************************************************************************************************************
/// \brief Retrieve the rectangle for the current window.
/// 
/// \return A rect indicating the position and size of the foreground window. if no windows is on the foreground,
/// the available area of the desktop for monitor the mouse in on is returned.
//****************************************************************************************************************************************************
QRect foregroundWindowRect() {
    HWND const hwnd = GetForegroundWindow();
    if (isDesktop(hwnd)) // Desktop is active, we return the geometry of the monitor containing the mouse cursor
    {
        QScreen const *const screen = screenContainingCursor();
        return screen ? screen->availableGeometry() : QRect();
    }
    return rectForHwnd(hwnd);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void showComboPickerWindow() {
    static PickerWindow window;
    QRect const rect = foregroundWindowRect();
    if (rect.isNull())
        window.move(QCursor::pos());
    else
        window.move(QPoint(rect.left() + ((rect.width() - window.width()) / 2), rect.top() + ((rect.height() - window.height()) / 2)));
    window.show();
    SetForegroundWindow(reinterpret_cast<HWND>(window.winId()));  // NOLINT(performance-no-int-to-ptr)
    qApp->setActiveWindow(&window);
    window.raise();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
PickerWindow::PickerWindow()
    : QWidget(nullptr)
    , moverResizer_(*this) {
    ui_.setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_AlwaysStackOnTop);
    ui_.listViewResults->setModel(&proxyModel_);
    ui_.listViewResults->setItemDelegate(new PickerItemDelegate(ui_.listViewResults));
    proxyModel_.setSourceModel(&model_);
    proxyModel_.sort(0, Qt::DescendingOrder);
    qApp->installEventFilter(this);
    this->restoreGeometry(PreferencesManager::instance().comboPickerWindowGeometry());
    ui_.listViewResults->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui_.editSearch, &QLineEdit::textChanged, this, &PickerWindow::onSearchTextChanged);
    connect(ui_.listViewResults, &QListView::customContextMenuRequested, this, &PickerWindow::onResultListRightClicked);
    connect(ui_.listViewResults, &QListView::clicked, this, &PickerWindow::onItemClicked);
}


//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void PickerWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Down:
        this->selectNextItem();
        break;
    case Qt::Key_Up:
        this->selectPreviousItem();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (event->modifiers().testFlag(Qt::ControlModifier))
            copySelectedItemToClipboard();
        else
            this->triggerSelectedItem();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}


//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void PickerWindow::changeEvent(QEvent *event) {
    if ((event->type() == QEvent::ActivationChange) && !this->isActiveWindow())
        this->close(); // when the dialog looses the focus, we dismiss is because we don't know where the input
    // focus can be now.
    QWidget::changeEvent(event);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PickerWindow::showEvent(QShowEvent *) {
    ui_.editSearch->setText(QString());
    model_.resetModel(); // forces a sort
    this->selectItemAtIndex(0);
}


//****************************************************************************************************************************************************
/// \param[in] event The event
//****************************************************************************************************************************************************
void PickerWindow::mousePressEvent(QMouseEvent *event) {
    moverResizer_.processMousePressEvent(event);
    QWidget::mousePressEvent(event);
}


//****************************************************************************************************************************************************
/// \param[in] event The event
//****************************************************************************************************************************************************
void PickerWindow::mouseReleaseEvent(QMouseEvent *event) {
    moverResizer_.processMouseReleaseEvent(event);
    QWidget::mouseReleaseEvent(event);
}


//****************************************************************************************************************************************************
/// \param[in] watched The watched object.
/// \param[in] event The event.
//****************************************************************************************************************************************************
bool PickerWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event && (QEvent::MouseMove == event->type()))
        moverResizer_.processMouseMoveEvent();

    return QWidget::eventFilter(watched, event);
}


//****************************************************************************************************************************************************
/// \param[in] text The new text in the search edit.
//****************************************************************************************************************************************************
void PickerWindow::onSearchTextChanged(QString const &text) {
    this->style()->polish(ui_.editSearch);
    proxyModel_.setFilterFixedString(text.trimmed());
    if (proxyModel_.rowCount() > 0)
        this->selectItemAtIndex(0);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PickerWindow::onItemClicked(QModelIndex const &) {
    this->triggerSelectedItem();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PickerWindow::selectPreviousItem() const {
    qint32 const index = ui_.listViewResults->currentIndex().row();
    if (index > 0)
        this->selectItemAtIndex(index - 1);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PickerWindow::selectNextItem() const {
    qint32 const index = this->selectedItemIndex();
    if ((index >= 0) && (index <= proxyModel_.rowCount() - 2))
        this->selectItemAtIndex(index + 1);
}


//****************************************************************************************************************************************************
/// \return The index of the selected combo.
/// \return -1 if no combo is selected.
//****************************************************************************************************************************************************
qint32 PickerWindow::selectedItemIndex() const {
    QModelIndex const modelIndex = ui_.listViewResults->currentIndex();
    return modelIndex.isValid() ? modelIndex.row() : -1;
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the selected combo.
//****************************************************************************************************************************************************
void PickerWindow::selectItemAtIndex(qint32 index) const {
    ui_.listViewResults->setCurrentIndex(proxyModel_.index(index, 0));
    ui_.editSearch->setFocus();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PickerWindow::triggerSelectedItem() {
    this->close();
    QModelIndex const index = ui_.listViewResults->currentIndex();
    if (!index.isValid())
        return;
    bool const isCombo = (constants::Combo == index.data(constants::TypeRole).value<constants::EITemType>());
    SpCombo const combo = (isCombo ? index.data(constants::PointerRole).value<SpCombo>() : nullptr);
    SpEmoji const emoji = (isCombo ? nullptr : index.data(constants::PointerRole).value<SpEmoji>());
    std::chrono::milliseconds constexpr delay(200);
    if (combo)
        QTimer::singleShot(delay, [combo]() { combo->performSubstitution(true); });
    else
        QTimer::singleShot(delay, [emoji]() {
            if ((!isBeeftextTheForegroundApplication()) &&
                !EmojiManager::instance().isExcludedApplication(getActiveExecutableFileName())) {
                performTextSubstitution(0, emoji->value(), -1, ETriggerSource::ComboPicker);
                emoji->setlastUseDateTime(QDateTime::currentDateTime());
            }
        });
}


//****************************************************************************************************************************************************
/// \note This function may require user input using a dialog
///
/// \param[in] index the index of the selected item.
/// \return The text for the item
/// \return a null string if the text could not be retrieved
//****************************************************************************************************************************************************
QString PickerWindow::textForItemAtIndex(QModelIndex const &index) {
    if (!index.isValid())
        return QString();

    bool const isCombo = (constants::Combo == index.data(constants::TypeRole).value<constants::EITemType>());
    SpCombo const combo = (isCombo ? index.data(constants::PointerRole).value<SpCombo>() : nullptr);

    QString str;
    if (isCombo) {
        if (!combo) {
            globals::debugLog().addError("Picker window could not retrieve selected combo.");
            return QString();
        }
        bool cancelled = false;
        QString const text = combo->evaluatedSnippet(cancelled);
        return cancelled ? QString() : text;
    }

    SpEmoji const emoji = index.data(constants::PointerRole).value<SpEmoji>();
    if (!emoji) {
        globals::debugLog().addError("Picker window could not retrieve the selected emoji.");
        return QString();
    }
    emoji->setlastUseDateTime(QDateTime::currentDateTime());
    return emoji->value();
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the item.
//****************************************************************************************************************************************************
void PickerWindow::copyItemAtIndexToClipboardAndClose(QModelIndex const &index) {
    QString const text = this->textForItemAtIndex(index);
    if (text.isNull())
        return;
    this->close();
    ClipboardManager::instance().setText(text);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void PickerWindow::copySelectedItemToClipboard() {
    this->copyItemAtIndexToClipboardAndClose(ui_.listViewResults->currentIndex());
}


//****************************************************************************************************************************************************
/// \param[in] p The location of the cursor.
//****************************************************************************************************************************************************
void PickerWindow::onResultListRightClicked(QPoint const &p) {
    this->copyItemAtIndexToClipboardAndClose(ui_.listViewResults->indexAt(p));
}
