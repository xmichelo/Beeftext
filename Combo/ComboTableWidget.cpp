/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo table frame
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboTableWidget.h"
#include "ComboImportDialog.h"
#include "ComboManager.h"
#include "ComboDialog.h"
#include "PreferencesManager.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {
   qint32 const kMinNameColumnDefaultWidth = 200; ///< The maximum default width in pixels of the name column
   qint32 const kMaxNameColumnDefaultWidth = 500; ///< The maximum default width in pixels of the name column
}



//**********************************************************************************************************************
/// \param[in] parent The parent widget of the frame
//**********************************************************************************************************************
ComboTableWidget::ComboTableWidget(QWidget* parent)
   : QWidget(parent)
   , contextMenu_(nullptr)
   , groupListWidget_(nullptr)
{
   ui_.setupUi(this);
   this->setupTable();
   this->setupKeyboadShortcuts();
   this->setupCombosMenu();
   this->setupContextMenu();
   this->updateGui();
}


//**********************************************************************************************************************
/// \param[in] groupListWidget The group list widget
//**********************************************************************************************************************
void ComboTableWidget::setGroupListWidget(GroupListWidget* groupListWidget)
{
   groupListWidget_ = groupListWidget;
}


//**********************************************************************************************************************
/// \param[in] filePath The path of the combo file to import. This value can be null
//**********************************************************************************************************************
void ComboTableWidget::runComboImportDialog(QString const& filePath)
{
   if (QDialog::Accepted == ComboImportDialog(filePath, 
         (groupListWidget_ ? groupListWidget_->selectedGroup() : SPGroup()), this).exec())
      this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onSelectedGroupChanged(SPGroup const& group)
{
   proxyModel_.setGroup(group);
   this->resizeColumnsToContents();
   QItemSelectionModel *model = ui_.tableComboList->selectionModel();
   if (model)
      model->clear();
   this->updateGui();
}


//**********************************************************************************************************************
/// This event filter override the default mouse double click behavior of the table view to respond also 
/// when the user double click on an empty area
//**********************************************************************************************************************
bool ComboTableWidget::eventFilter(QObject *object, QEvent *event)
{
   if (event->type() != QEvent::MouseButtonDblClick)
      return QObject::eventFilter(object, event);
   this->onDoubleClick();
   return true;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::setupTable()
{
   proxyModel_.setSourceModel(&ComboManager::instance().comboListRef());
   ui_.tableComboList->setModel(&proxyModel_);
   proxyModel_.sort(0, Qt::AscendingOrder);
   QHeaderView* horizontalHeader = ui_.tableComboList->horizontalHeader();
   if (!horizontalHeader)
      throw xmilib::Exception(QString("Internal error: %1(): could not retrieve horizontal header.")
         .arg(__FUNCTION__));
   horizontalHeader->setFixedHeight(horizontalHeader->fontMetrics().height() + 10);
   horizontalHeader->setSortIndicator(0, Qt::AscendingOrder);  //< required, otherwise the indicator is first displayed in the wrong direction
   horizontalHeader->setDefaultAlignment(Qt::AlignLeft);
   connect(ui_.tableComboList->selectionModel(), &QItemSelectionModel::selectionChanged, this,
      &ComboTableWidget::updateGui);
   connect(&ComboManager::instance().groupListRef(), &GroupList::combosChangedGroup, this,
      &ComboTableWidget::onComboChangedGroup);
   QHeaderView *verticalHeader = ui_.tableComboList->verticalHeader();

   if (!verticalHeader)
      throw xmilib::Exception(QString("Internal error: %1(): could not retrieve vertical header.")
         .arg(__FUNCTION__));
   verticalHeader->setDefaultSectionSize(verticalHeader->fontMetrics().height() + 10);

   this->resizeColumnsToContents();
   ui_.tableComboList->viewport()->installEventFilter(this); // we install an event filter that override the default double-click behavior
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::setupKeyboadShortcuts()
{
   // Most shortcuts are set via the actions in the widget UI form. However, shortcuts work only if the action are
   // used. Although actions are not used in the form itself, they are used in the menu, instead for the ones that are
   // defined below

   connect(new QShortcut(QKeySequence("Ctrl+F"), this), &QShortcut::activated, this, 
      &ComboTableWidget::onActionStartSearch);
   connect(new QShortcut(QKeySequence("Escape"), this), &QShortcut::activated, this,
      &ComboTableWidget::onActionClearSearch);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::setupCombosMenu()
{
   QMenu* menu = new QMenu(this);
   menu->addAction(ui_.actionNewCombo);
   menu->addAction(ui_.actionDuplicateCombo);
   menu->addSeparator();
   menu->addAction(ui_.actionEditCombo);
   menu->addSeparator();
   menu->addAction(ui_.actionDeleteCombo);
   menu->addSeparator();
   menu->addAction(ui_.actionEnableDisableCombo);
   menu->addSeparator();
   menu->addAction(ui_.actionSelectAll);
   menu->addAction(ui_.actionDeselectAll);
   menu->addSeparator();
   menu->addAction(ui_.actionDeleteCombo);
   menu->addSeparator();
   menu->addAction(ui_.actionImportCombos);
   menu->addAction(ui_.actionExportCombo);
   menu->addAction(ui_.actionExportAllCombos);
   ui_.buttonCombos->setMenu(menu);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::setupContextMenu()
{
   contextMenu_.clear();
   contextMenu_.addAction(ui_.actionNewCombo);
   contextMenu_.addAction(ui_.actionDuplicateCombo);
   contextMenu_.addSeparator();
   contextMenu_.addAction(ui_.actionEditCombo);
   contextMenu_.addSeparator();
   contextMenu_.addAction(ui_.actionDeleteCombo);
   contextMenu_.addSeparator();
   contextMenu_.addAction(ui_.actionEnableDisableCombo);
   contextMenu_.addSeparator();
   contextMenu_.addAction(ui_.actionSelectAll);
   contextMenu_.addAction(ui_.actionDeselectAll);
   contextMenu_.addSeparator();
   contextMenu_.addAction(ui_.actionImportCombos);
   contextMenu_.addAction(ui_.actionExportCombo);
   contextMenu_.addAction(ui_.actionExportAllCombos);
   ui_.tableComboList->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui_.tableComboList, &QTableView::customContextMenuRequested, this, &ComboTableWidget::onContextMenuRequested);
}


//**********************************************************************************************************************
/// \return The number of selected combos
//**********************************************************************************************************************
qint32 ComboTableWidget::getSelectedComboCount() const
{
   return ui_.tableComboList->selectionModel()->selectedRows().size();
}


//**********************************************************************************************************************
/// The returned indexes are based on the internal order of the list, not the display order in the table (that can be
/// modified by sorting by columns).
///
/// \return The list of indexes of the selected combos 
//**********************************************************************************************************************
QList<qint32> ComboTableWidget::getSelectedComboIndexes() const
{
   QList<qint32> result;
   ComboList& comboList = ComboManager::instance().comboListRef();
   QModelIndexList selectedRows = ui_.tableComboList->selectionModel()->selectedRows();
   for (QModelIndex const& modelIndex : selectedRows)
   {
      qint32 const index = proxyModel_.mapToSource(modelIndex).row();
      if ((index >= 0) && (index < comboList.size()))
         result.push_back(index);
   }
   return result;
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void ComboTableWidget::changeEvent(QEvent *event)
{
   if (QEvent::LanguageChange == event->type())
      ui_.retranslateUi(this);
   QWidget::changeEvent(event);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::resizeColumnsToContents()
{
   QHeaderView* horizontalHeader = ui_.tableComboList->horizontalHeader();
   if (!horizontalHeader)
      throw xmilib::Exception(QString("Internal error: %1(): could not retrieve horizontal header").arg(__FUNCTION__));

   for (qint32 i = 0; i < 2; ++i)
      ui_.tableComboList->resizeColumnToContents(i);
   horizontalHeader->resizeSection(0, qBound<qint32>(kMinNameColumnDefaultWidth, horizontalHeader->sectionSize(0),
      kMaxNameColumnDefaultWidth));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::updateGui() const
{
   qint32 const selectedCount = this->getSelectedComboCount();
   bool const listIsEmpty = (ComboManager::instance().comboListRef().size() == 0);
   bool const hasOneSelected = (1 == selectedCount);
   bool const hasOneOrMoreSelected = (selectedCount > 0);
   ui_.actionDuplicateCombo->setEnabled(hasOneSelected);
   ui_.actionDeleteCombo->setEnabled(hasOneOrMoreSelected);
   ui_.actionEditCombo->setEnabled(hasOneSelected);
   ui_.actionEnableDisableCombo->setEnabled(hasOneSelected);
   ui_.actionExportCombo->setEnabled(hasOneOrMoreSelected);
   ui_.actionExportAllCombos->setEnabled(!listIsEmpty);

   QString enableDisableText = tr("Ena&ble");
   QString enableDisableToolTip = tr("Enable combo");
   if ((hasOneSelected) 
      && (ComboManager::instance().comboListRef()[this->getSelectedComboIndexes().front()]->isEnabled()))
   {
      enableDisableText = "Disa&ble";
      enableDisableToolTip = "Disable combo";
   }
   ui_.actionEnableDisableCombo->setText(enableDisableText);
   ui_.actionEnableDisableCombo->setToolTip(enableDisableToolTip);
   ui_.actionEnableDisableCombo->setIconText(enableDisableToolTip);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionStartSearch()
{
   this->ui_.editSearch->setFocus(); 
   this->ui_.editSearch->selectAll();
}



//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionClearSearch()
{
   this->ui_.editSearch->setFocus(); 
   this->ui_.editSearch->clear();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionNewCombo()
{
   try
   {
      SPCombo combo = Combo::create();
      combo->setGroup(groupListWidget_->selectedGroup());
      if (!ComboDialog::run(combo, tr("New Combo")))
         return;
      ComboManager& comboManager = ComboManager::instance();
      ComboList& comboList = ComboManager::instance().comboListRef();
      if (!comboList.append(combo))
         throw xmilib::Exception(tr("The combo could not be added to the list."));
      QString errorMessage;
      if (!comboManager.saveComboListToFile(&errorMessage))
         throw xmilib::Exception(errorMessage);
      this->updateGui();
   }
   catch (xmilib::Exception const& e)
   {
      QMessageBox::critical(this, tr("Error"), e.qwhat());
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionDuplicateCombo()
{
   try
   {
      QList<qint32> const selectedIndex = this->getSelectedComboIndexes();
      if (1 != selectedIndex.size())
         return;
      ComboManager& comboManager = ComboManager::instance();
      ComboList& comboList = comboManager.comboListRef();
      qint32 const index = selectedIndex[0];
      Q_ASSERT((index >= 0) && (index < comboList.size()));
      if ((index < 0) || (index >= comboList.size()))
         throw xmilib::Exception(tr("The combo could not be duplicated: invalid index."));
      SPCombo combo = Combo::duplicate(*comboList[index]);
      if (!ComboDialog::run(combo, tr("Duplicate Combo")))
         return;
      if (!comboList.append(combo))
         throw xmilib::Exception(tr("The duplicated combo could not added to the list."));
      this->updateGui();
   }
   catch (xmilib::Exception const& e)
   {
      QMessageBox::critical(this, tr("Error"), e.qwhat());
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionDeleteCombo()
{
   qint32 const count = this->getSelectedComboCount();
   if (count < 1)
      return;
   QString const question = count > 1 ? tr("Are you sure you want to delete the selected combos?")
      : tr("Are you sure you want to delete the selected combo?");
   if (QMessageBox::Yes != QMessageBox::question(nullptr, tr("Delete Combo?"), question,
      QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
      return;

   ComboManager& comboManager = ComboManager::instance();
   QList<qint32> indexes = this->getSelectedComboIndexes();
   std::sort(indexes.begin(), indexes.end(), [](qint32 first, qint32 second) -> bool { return first > second; });
   for (qint32 index : indexes)
      comboManager.comboListRef().erase(index);
   QString errorMessage;
   if (!comboManager.saveComboListToFile(&errorMessage))
      QMessageBox::critical(this, tr("Error"), errorMessage);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionEditCombo()
{
   QList<qint32> const selectedIndex = this->getSelectedComboIndexes();
   if (1 != selectedIndex.size())
      return;

   ComboManager& comboManager = ComboManager::instance();
   ComboList& comboList = comboManager.comboListRef();
   qint32 const index = selectedIndex[0];
   Q_ASSERT((index >= 0) && (index < comboList.size()));
   SPCombo combo = comboList[index];
   if (!ComboDialog::run(combo, tr("Edit Combo")))
      return;
   comboList.markComboAsEdited(index);
   QString errorMessage;
   if (!comboManager.saveComboListToFile(&errorMessage))
      QMessageBox::critical(this, tr("Error"), errorMessage);
   proxyModel_.invalidate();
   this->updateGui();
   this->resizeColumnsToContents();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionSelectAll() const
{
   ui_.tableComboList->selectAll();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionDeselectAll() const
{
   ui_.tableComboList->clearSelection();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionEnableDisableCombo()
{
   QList<qint32> const selectedIndex = this->getSelectedComboIndexes();
   if (1 != selectedIndex.size())
      return;

   ComboManager& comboManager = ComboManager::instance();
   ComboList& comboList = comboManager.comboListRef();
   qint32 const index = selectedIndex[0];
   Q_ASSERT((index >= 0) && (index < comboList.size()));
   SPCombo combo = comboList[index];
   combo->setEnabled(!combo->isEnabled());
   comboList.markComboAsEdited(index);
   QString errorMessage;
   if (!comboManager.saveComboListToFile(&errorMessage))
      QMessageBox::critical(this, tr("Error"), errorMessage);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionExportCombo()
{
   QList<qint32> const indexes = this->getSelectedComboIndexes();
   if (indexes.size() < 1)
      return;

   PreferencesManager& prefs = PreferencesManager::instance();
   QString const path = QFileDialog::getSaveFileName(this, tr("Export Combos"), prefs.lastComboImportExportPath(), 
      constants::kJsonFileDialogFilter);
   if (path.isEmpty())
      return;
   prefs.setLastComboImportExportPath(path);

   ComboList const& comboList = ComboManager::instance().comboListRef();
   ComboList exportList;
   for (qint32 const index : indexes)
   {
      Q_ASSERT((index >= 0) && (index < comboList.size()));
      exportList.append(comboList[index]);
   }
   QString errorMsg;
   if (!exportList.save(path, false, &errorMsg))
   {
      globals::debugLog().addError(errorMsg);
      QMessageBox::critical(this, tr("Error"), tr("Could not save the combo list file."));
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionExportAllCombos()
{
   PreferencesManager& prefs = PreferencesManager::instance();
   QString const path = QFileDialog::getSaveFileName(this, tr("Export All Combos"), prefs.lastComboImportExportPath(),
      constants::kJsonFileDialogFilter);
   if (path.isEmpty())
      return;
   prefs.setLastComboImportExportPath(path);
   QString errorMessage;
   if (!ComboManager::instance().comboListRef().save(path, false, &errorMessage))
   {
      globals::debugLog().addError(errorMessage);
      QMessageBox::critical(this, tr("Error"), tr("Could not save the combo list file."));
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onActionImportCombos()
{
   this->runComboImportDialog();
   this->resizeColumnsToContents();
}


//**********************************************************************************************************************
/// \param[in] text The text to search
//**********************************************************************************************************************
void ComboTableWidget::onSearchFilterChanged(QString const& text)
{
   QString const searchStr = text.trimmed();
   proxyModel_.setFilterFixedString(searchStr);
}

//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onContextMenuRequested()
{
   contextMenu_.exec(QCursor::pos());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onDoubleClick()
{
   switch (this->getSelectedComboCount())
   {
   case 0:
      this->onActionNewCombo();
      break;
   case 1:
      this->onActionEditCombo();
   default:
      break;
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableWidget::onComboChangedGroup()
{
   proxyModel_.invalidate();
   ComboManager::instance().saveComboListToFile();
   this->resizeColumnsToContents();
}

