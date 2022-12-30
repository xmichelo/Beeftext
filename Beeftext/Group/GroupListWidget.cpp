/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group list widget
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "GroupDialog.h"
#include "GroupListWidget.h"
#include "MimeDataUtils.h"
#include "Combo/ComboManager.h"
#include <XMiLib/Exception.h>


namespace {


struct ActionMapping {
    QAction **action { nullptr };
    QString text;
    QString tooltip;
    QString shortcut;
    void (GroupListWidget::*slot)() { nullptr };
}; ///< Structure for action mapping.


}


//****************************************************************************************************************************************************
/// \param[in] parent The parent of the widget
//****************************************************************************************************************************************************
GroupListWidget::GroupListWidget(QWidget *parent)
    : QWidget(parent), ui_(), contextMenu_(nullptr) {
    ui_.setupUi(this);
    GroupList &groups = ComboManager::instance().comboListRef().groupListRef();
    ui_.listGroup->setModel(&groups);
    ui_.listGroup->viewport()->installEventFilter(this);
    this->setupActions();
    this->setupContextMenu();
    this->setupGroupsMenu();
    QItemSelectionModel const *selectionModel = ui_.listGroup->selectionModel();
    if (!selectionModel)
        throw xmilib::Exception("The group list selection model is null");
    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &GroupListWidget::onSelectionChanged);
    connect(&groups, &GroupList::groupMoved, this, &GroupListWidget::onGroupMoved);
    connect(&ComboManager::instance(), &ComboManager::backupWasRestored, this, &GroupListWidget::onBackupRestored);
    connect(new QShortcut(QKeySequence("Ctrl+Shift+A"), this), &QShortcut::activated, this, &GroupListWidget::selectAllCombosEntry);
    connect(ui_.listGroup, &QListView::doubleClicked, this, &GroupListWidget::onActionEditGroup);

    this->updateGui();
    QTimer::singleShot(0, [&]() { this->selectAllCombosEntry(); }); // delayed to be sure the signal/slot mechanism work
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void GroupListWidget::setupActions() {
    QList<ActionMapping> const mappings = {
        { &actionNewGroup_,           tr("&New"),    tr("New Group"),    tr("Ctrl+Shift+N"), &GroupListWidget::onActionNewGroup },
        { &actionDeleteGroup_,        tr("&Delete"), tr("Delete Group"), tr("Shift+Del"),    &GroupListWidget::onActionDeleteGroup },
        { &actionEditGroup_,          tr("&Edit"),   tr("Edit Group"),   tr("Shift+Return"), &GroupListWidget::onActionEditGroup },
        { &actionEnableDisableGroup_, QString(),     QString(),          tr("Ctrl+Shift+E"), &GroupListWidget::onActionEnableDisableGroup },
    };

    for (ActionMapping const &mapping: mappings) {
        QAction *action = new QAction(mapping.text, this);
        action->setToolTip(mapping.tooltip);
        action->setShortcut(mapping.shortcut);
        *mapping.action = action;
        connect(action, &QAction::triggered, this, mapping.slot);
    }
}


//****************************************************************************************************************************************************
/// \return the selected group
/// \return a null pointer if nor group is selected
//****************************************************************************************************************************************************
SpGroup GroupListWidget::selectedGroup() const {
    GroupList const &list = ComboManager::instance().groupListRef();
    qint32 const index = this->selectedGroupIndex();
    return ((index >= 0) && (index < list.size())) ? list[index] : SpGroup();
}


//****************************************************************************************************************************************************
/// \return The selected group or the first group if no group is selected
//****************************************************************************************************************************************************
SpGroup GroupListWidget::selectedOrFirstGroup() const {
    SpGroup group = selectedGroup();
    if (!group) {
        GroupList &groupList = ComboManager::instance().comboListRef().groupListRef();
        if (!groupList.isEmpty())
            group = groupList[0];
    }
    return group;
}


//****************************************************************************************************************************************************
/// \param[in] group The group
//****************************************************************************************************************************************************
void GroupListWidget::selectGroup(SpGroup const &group) const {
    if (!group)
        return;
    GroupList &groupList = ComboManager::instance().comboListRef().groupListRef();
    for (qint32 i = 0; i < groupList.size(); ++i) {
        SpGroup const g = groupList[i];
        if (g && (g->uuid() == group->uuid())) {
            ui_.listGroup->setCurrentIndex(groupList.index(i + 1)); /// + 1 because entry 0 is \<All combos\>
            return;
        }
    }
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the menu
//****************************************************************************************************************************************************
QMenu *GroupListWidget::menu(QWidget *parent) const {
    QMenu *menu = new QMenu(menuTitle(), parent);
    menu->addAction(actionNewGroup_);
    menu->addAction(actionEditGroup_);
    menu->addSeparator();
    menu->addAction(actionDeleteGroup_);
    menu->addSeparator();
    menu->addAction(actionEnableDisableGroup_);
    connect(menu, &QMenu::aboutToShow, this, &GroupListWidget::onMenuAboutToShow);
    return menu;
}


//****************************************************************************************************************************************************
/// \return The localized title of the menu.
//****************************************************************************************************************************************************
QString GroupListWidget::menuTitle() {
    return tr("&Groups");
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupListWidget::updateGui() const {
    qint32 const groupCount = ComboManager::instance().groupListRef().size();
    qint32 const index = this->selectedGroupIndex();
    bool const validIndex = (index >= 0) && (index < groupCount);
    actionDeleteGroup_->setEnabled(validIndex && (groupCount > 1));
    actionEditGroup_->setEnabled(validIndex);
    actionEnableDisableGroup_->setEnabled(validIndex);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupListWidget::setupGroupsMenu() {
    ui_.buttonGroups->setMenu(this->menu(this));
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupListWidget::setupContextMenu() {
    if (contextMenu_)
        contextMenu_->deleteLater();
    contextMenu_ = this->menu(this);
    ui_.listGroup->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui_.listGroup, &QListView::customContextMenuRequested, this, &GroupListWidget::onContextMenuRequested);
}


//****************************************************************************************************************************************************
/// \param[in] event The event
//****************************************************************************************************************************************************
void GroupListWidget::changeEvent(QEvent *event) {
    if (QEvent::LanguageChange == event->type())
        ui_.retranslateUi(this);
    QWidget::changeEvent(event);
}


//****************************************************************************************************************************************************
/// \return The index of the selected group
/// \return -1 if no group is selected
//****************************************************************************************************************************************************
qint32 GroupListWidget::selectedGroupIndex() const {
    QItemSelectionModel const *model = ui_.listGroup->selectionModel();
    if (!model)
        return -1;
    QModelIndexList const selectedIndexes = model->selectedIndexes();
    if (selectedIndexes.isEmpty() || (0 == selectedIndexes.front().row()))
        return -1;
    return selectedIndexes.front().row() - 1; // -1 because entry at index 0 is '<All combos>'
}


//****************************************************************************************************************************************************
/// This event filter implement a non default behavior for the Qt QListView single selection system. When clicking
/// on a selected item or an empty area, the 'New Group' action is triggered.
/// 
/// \param[in] object The object
/// \param[in] event The event
//****************************************************************************************************************************************************
bool GroupListWidget::eventFilter(QObject *object, QEvent *event) {
    if ((!event) || (!object))
        return false;
    if (event->type() == QEvent::DragEnter) {
        QDropEvent const *dropEvent = dynamic_cast<QDropEvent *>(event);
        if (dropEvent) {
            QMimeData const *mimeData = dropEvent->mimeData();
            ComboManager::instance().groupListRef().setDropType(mimeData && mimeData->hasFormat(kUuuidListMimeType) ?
                GroupList::ComboDrop : GroupList::GroupDrop);
        }
    }

    if (event->type() != QEvent::MouseButtonDblClick)
        return QObject::eventFilter(object, event);
    QMouseEvent const *mouseEvent = dynamic_cast<QMouseEvent *>(event);
    if (!mouseEvent)
        throw xmilib::Exception(QString("Internal error: %1(): could not retrieve mouse event.").arg(__FUNCTION__));
    QItemSelectionModel const *selectionModel = ui_.listGroup->selectionModel();
    if (!selectionModel)
        throw xmilib::Exception(QString("Internal error: %1(): could not retrieve selection model.").arg(__FUNCTION__));
    QPoint const mousePos = mouseEvent->pos();
    QModelIndex const item = ui_.listGroup->indexAt(mousePos);
    if (!item.isValid()) {
        QTimer::singleShot(0, this, &GroupListWidget::onActionNewGroup);
        return true;
    }
    return QObject::eventFilter(object, event);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void GroupListWidget::updateActionsAvailabilityAndNames() const {
    SpGroup const group = this->selectedGroup();
    bool const enabled = (group && group->enabled());
    actionEnableDisableGroup_->setText(enabled ? tr("&Disable") : tr("&Enable"));
    actionEnableDisableGroup_->setIconText(enabled ? tr("Enable") : tr("Disable"));
    actionEnableDisableGroup_->setToolTip(enabled ? tr("Enable the group") : tr("Disable the group"));
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupListWidget::onActionNewGroup() {
    try {
        SpGroup const group = Group::create(QString());
        if (!GroupDialog::run(group, tr("New Group")))
            return;
        ComboManager &comboManager = ComboManager::instance();
        GroupList &groups = comboManager.groupListRef();
        if (!groups.append(group))
            throw xmilib::Exception(tr("The group could not be added to the list."));
        QString errorMessage;
        if (!comboManager.saveComboListToFile(&errorMessage))
            throw xmilib::Exception(errorMessage);
        this->selectGroup(group);
    }
    catch (xmilib::Exception const &e) {
        QMessageBox::critical(this, tr("Error"), e.qwhat());
    }
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupListWidget::selectAllCombosEntry() const {
    ui_.listGroup->setCurrentIndex(ComboManager::instance().comboListRef().groupListRef().index(0));
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupListWidget::onActionEditGroup() {
    try {
        ComboManager &comboManager = ComboManager::instance();
        GroupList &groups = comboManager.groupListRef();
        qint32 const index = this->selectedGroupIndex();
        if ((index < 0) || (index >= groups.size()))
            return;
        SpGroup const group = groups[index];
        if (!GroupDialog::run(group, tr("Edit Group")))
            return;
        QString errorMessage;
        if (!comboManager.saveComboListToFile(&errorMessage))
            throw xmilib::Exception(errorMessage);
        this->updateGui();
    }
    catch (xmilib::Exception const &e) {
        QMessageBox::critical(this, tr("Error"), e.qwhat());
    }
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupListWidget::onActionDeleteGroup() {
    try {
        if (QMessageBox::Yes != QMessageBox::question(this, tr("Delete Group?"), tr("Are you sure you want to delete "
                                                                                    "the selected group and all its combos?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
            return;
        ComboManager &comboManager = ComboManager::instance();
        GroupList &groups = comboManager.groupListRef();
        qint32 const index = this->selectedGroupIndex();
        if ((groups.size() <= 1) || (index < 0) || (index >= groups.size()))
            return;
        comboManager.comboListRef().eraseCombosOfGroup(groups[index]);
        groups.erase(index);
        if (!comboManager.saveComboListToFile())
            throw xmilib::Exception("Could not save combo list.");
        // we force the emission of a selectedGroupChange event, because the system will not do it in that case
        this->onSelectionChanged(QItemSelection(), QItemSelection());
        this->updateGui();
    }
    catch (xmilib::Exception const &e) {
        QMessageBox::critical(this, tr("Error"), e.qwhat());
    }
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void GroupListWidget::onActionEnableDisableGroup() {
    try {
        SpGroup const group = this->selectedGroup();
        if (!group)
            return;
        group->setEnabled(!group->enabled());
        this->updateGui();
        QString errorMessage;
        if (!ComboManager::instance().saveComboListToFile(&errorMessage))
            throw xmilib::Exception(errorMessage);

        qint32 const index = this->selectedGroupIndex();
        GroupList &groups = ComboManager::instance().groupListRef();
        emit selectedGroupChanged(((index < 0) || (index >= groups.size())) ? nullptr : groups[index]);
    }
    catch (xmilib::Exception const &e) {
        QMessageBox::critical(this, tr("Error"), e.qwhat());
    }
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void GroupListWidget::onSelectionChanged(QItemSelection const &, QItemSelection const &) {
    this->updateGui();
    GroupList &groups = ComboManager::instance().groupListRef();
    qint32 const index = this->selectedGroupIndex();
    if (index < 0)
        this->selectAllCombosEntry();
    emit selectedGroupChanged(((index < 0) || (index >= groups.size())) ? nullptr : groups[index]);
}


//****************************************************************************************************************************************************
/// \param[in] group The group
/// \param[in] newIndex The new index of the group in the list
//****************************************************************************************************************************************************
void GroupListWidget::onGroupMoved(SpGroup const &group, qint32 newIndex) const {
    if (!group)
        return;
    ComboManager &comboManager = ComboManager::instance();
    ui_.listGroup->setCurrentIndex(comboManager.groupListRef().index(newIndex + 1));
    //+1 because entry at index 0 is '<All combos>'
    if (!comboManager.saveComboListToFile())
        throw xmilib::Exception("Could not save combo list.");
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupListWidget::onContextMenuRequested() const {
    if (contextMenu_)
        contextMenu_->exec(QCursor::pos());
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void GroupListWidget::onMenuAboutToShow() const {
    QMenu const *menu = dynamic_cast<QMenu *>(this->sender());
    if (!menu)
        throw xmilib::Exception(QString("Internal error: %1(): could not retrieve context menu.").arg(__FUNCTION__));
    this->updateActionsAvailabilityAndNames();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void GroupListWidget::onBackupRestored() const {
    this->selectAllCombosEntry();
}


