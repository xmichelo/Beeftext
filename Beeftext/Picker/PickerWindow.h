/// \file
/// \author 
///
/// \brief Declaration of picker window.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_COMBO_PICKER_WINDOW_H
#define BEEFTEXT_COMBO_PICKER_WINDOW_H


#include "ui_PickerWindow.h"
#include "PickerModel.h"
#include "PickerWindowMoverResizer.h"
#include "PickerSortFilterProxyModel.h"


void showComboPickerWindow(); ///< Show the combo picker window.
bool applyComboPickerPreferences(); ///< Apply the combo picker preferences


//****************************************************************************************************************************************************
/// \brief The combo picker window class
//****************************************************************************************************************************************************
class PickerWindow : public QWidget {
Q_OBJECT
public: // member functions
    PickerWindow(); ///< Default constructor
    PickerWindow(PickerWindow const &) = delete; ///< Disabled copy-constructor
    PickerWindow(PickerWindow &&) = delete; ///< Disabled assignment copy-constructor
    ~PickerWindow() override = default; ///< Destructor
    PickerWindow &operator=(PickerWindow const &) = delete; ///< Disabled assignment operator
    PickerWindow &operator=(PickerWindow &&) = delete; ///< Disabled move assignment operator

protected:
    void keyPressEvent(QKeyEvent *event) override; ///< Key press event handler.
    void changeEvent(QEvent *event) override; ///< Change event handler.
    void showEvent(QShowEvent *event) override; ///< Show event handler.
    void mousePressEvent(QMouseEvent *event) override; ///< Mouse press event handler.
    void mouseReleaseEvent(QMouseEvent *event) override; ///< Mouse release event handler.
    bool eventFilter(QObject *watched, QEvent *event) override; ///< Event filter handler.

private slots:
    void onSearchTextChanged(QString const &text); ///< Slot for the change of the search text.
    void onItemClicked(QModelIndex const &); ///< Slot for clicking on an item.

private: // member functions
    void selectPreviousItem() const; ///< Select the previous combo in the list.
    void selectNextItem() const; ///< Select the next combo in the list.
    qint32 selectedItemIndex() const; ///< Retrieve the index of the selected combo.
    void selectItemAtIndex(qint32 index) const; ///< Select the combo at a given index
    void triggerSelectedItem(); ///< Trigger the selected combo.
    static QString textForItemAtIndex(QModelIndex const &index); ///< Retrieve the text for the model at index.
    void copyItemAtIndexToClipboardAndClose(QModelIndex const &index); ///< Copy the text for the item at the given index into the clipboard and close the window
    void copySelectedItemToClipboard(); ///< Close the window and copy the selected item to the clipboard.
    void onResultListRightClicked(QPoint const &p); ///< Callback for the right click

private: // data member
    Ui::PickerWindow ui_ = {}; ///< The GUI for the window.
    PickerModel model_; ///< The model for the list view.
    PickerSortFilterProxyModel proxyModel_; ///< The proxy model for sorting/filtering the list view
    PickerWindowMoverResizer moverResizer_; ///< The mover/resizer for the window.
};


#endif // #ifndef BEEFTEXT_COMBO_PICKER_WINDOW_H
