/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo table frame
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboTableFrame.h"
#include "ComboManager.h"
#include "ComboDialog.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


namespace {
   qint32 const kMaxNameColumnDefaultWidth = 300; ///< The maximum default width in pixels of the name column
}

//**********************************************************************************************************************
/// brief A class overriding the default style only to remove the focus rectangle around items in a table view
//**********************************************************************************************************************
class ComboTableProxyStyle : public QProxyStyle
{
public:
   void drawPrimitive(PrimitiveElement element, const QStyleOption * option,
      QPainter * painter, const QWidget * widget = nullptr) const override
   {
      if (PE_FrameFocusRect != element)
         QProxyStyle::drawPrimitive(element, option, painter, widget);
   }
};


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the frame
//**********************************************************************************************************************
ComboTableFrame::ComboTableFrame(QWidget* parent)
   : QFrame(parent)
   , proxyStyle_(std::make_unique<ComboTableProxyStyle>())
   , contextMenu_(nullptr)
{
   ui_.setupUi(this);
   this->setupTable();
   this->setupContextMenu();
   this->setupImportExportMenu();
   this->updateGui();
   connect(new QShortcut(QKeySequence("Ctrl+F"), this), &QShortcut::activated,
      this, [&]() { this->ui_.editSearch->setFocus(); this->ui_.editSearch->selectAll(); });
   connect(new QShortcut(QKeySequence("Escape"), this), &QShortcut::activated,
      this, [&]() { this->ui_.editSearch->setFocus(); this->ui_.editSearch->clear(); });
   connect(new QShortcut(QKeySequence("Ctrl+N"), this), &QShortcut::activated,
      this, &ComboTableFrame::onActionAddCombo);
   connect(new QShortcut(QKeySequence("Delete"), this), &QShortcut::activated, 
      this, &ComboTableFrame::onActionDeleteCombo);
   connect(new QShortcut(QKeySequence("Ctrl+Shift+N"), this), &QShortcut::activated,
      this, &ComboTableFrame::onActionDuplicateCombo);
   connect(new QShortcut(QKeySequence(Qt::Key_Return), this), &QShortcut::activated,
      this, &ComboTableFrame::onActionEditCombo);
   connect(new QShortcut(QKeySequence("Ctrl+E"), this), &QShortcut::activated,
      this, &ComboTableFrame::onActionEnableDisableCombo);
   connect(new QShortcut(QKeySequence("Ctrl+A"), this), &QShortcut::activated,
      this, &ComboTableFrame::onActionSelectAll);
   connect(new QShortcut(QKeySequence("Ctrl+D"), this), &QShortcut::activated,
      this, &ComboTableFrame::onActionDeselectAll);
}


//**********************************************************************************************************************
/// This event filter override the default mouse double click behavior of the table view to respond also 
/// when the user double click on an empty area
//**********************************************************************************************************************
bool ComboTableFrame::eventFilter(QObject *object, QEvent *event)
{
   if (event->type() != QEvent::MouseButtonDblClick)
      return QObject::eventFilter(object, event);
   this->onDoubleClick();
   return true;
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::setupTable()
{
   proxyModel_.setSourceModel(&ComboManager::instance().getComboListRef());
   ui_.tableComboList->setModel(&proxyModel_);
   proxyModel_.sort(0, Qt::AscendingOrder);
   QHeaderView* horizontalHeader = ui_.tableComboList->horizontalHeader();
   horizontalHeader->setFixedHeight(horizontalHeader->fontMetrics().height() + 10);
   horizontalHeader->setSortIndicator(0, Qt::AscendingOrder);  //< required, otherwise the indicator is first displayed in the wrong direction
   horizontalHeader->setDefaultAlignment(Qt::AlignLeft);
   connect(ui_.tableComboList->selectionModel(), &QItemSelectionModel::selectionChanged, this,
      &ComboTableFrame::updateGui);
   QHeaderView *verticalHeader = ui_.tableComboList->verticalHeader();
   verticalHeader->setDefaultSectionSize(verticalHeader->fontMetrics().height() + 10);
   for (qint32 i = 0; i < 2; ++i)
      ui_.tableComboList->resizeColumnToContents(i);
   if (ui_.tableComboList->horizontalHeader()->sectionSize(0) > kMaxNameColumnDefaultWidth)
      ui_.tableComboList->horizontalHeader()->resizeSection(0, kMaxNameColumnDefaultWidth);
   ui_.tableComboList->setStyle(proxyStyle_.get());
   ui_.tableComboList->viewport()->installEventFilter(this); // we install an event filter that override the default double-click behavior
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::setupContextMenu()
{
   contextMenu_.clear();
   contextMenu_.addAction(ui_.actionAddCombo);
   contextMenu_.addAction(ui_.actionDuplicateCombo);
   contextMenu_.addAction(ui_.actionDeleteCombo);
   contextMenu_.addAction(ui_.actionEditCombo);
   contextMenu_.addAction(ui_.actionEnableDisableCombo);
   contextMenu_.addSeparator();
   contextMenu_.addAction(ui_.actionSelectAll);
   contextMenu_.addAction(ui_.actionDeselectAll);
   ui_.tableComboList->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui_.tableComboList, &QTableView::customContextMenuRequested, this, &ComboTableFrame::onContextMenuRequested);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::setupImportExportMenu()
{
   QMenu* menu = new QMenu(this);
   menu->addAction(ui_.actionImportCombos);
   menu->addSeparator();
   menu->addAction(ui_.actionExportCombo);
   menu->addAction(ui_.actionExportAllCombos);
   ui_.buttonImportExport->setMenu(menu);
}


//**********************************************************************************************************************
/// \return The number of selected combos
//**********************************************************************************************************************
qint32 ComboTableFrame::getSelectedComboCount() const
{
   return ui_.tableComboList->selectionModel()->selectedRows().size();
}


//**********************************************************************************************************************
/// The returned indexes are based on the internal order of the list, not the display order in the table (that can be
/// modified by sorting by columns).
///
/// \return The list of indexes of the selected combos 
//**********************************************************************************************************************
QList<qint32> ComboTableFrame::getSelectedComboIndexes() const
{
   QList<qint32> result;
   ComboList& comboList = ComboManager::instance().getComboListRef();
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
void ComboTableFrame::changeEvent(QEvent *event)
{
   if (QEvent::LanguageChange == event->type())
      ui_.retranslateUi(this);
   QFrame::changeEvent(event);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::updateGui() const
{
   qint32 const selectedCount = this->getSelectedComboCount();
   bool const listIsEmpty = (ComboManager::instance().getComboListRef().size() == 0);
   bool const hasOneSelected = (1 == selectedCount);
   bool const hasOneOrMoreSelected = (selectedCount > 0);
   ui_.buttonDuplicateCombo->setEnabled(hasOneSelected);
   ui_.actionDuplicateCombo->setEnabled(hasOneSelected);
   ui_.buttonDeleteCombo->setEnabled(hasOneOrMoreSelected);
   ui_.actionDeleteCombo->setEnabled(hasOneOrMoreSelected);
   ui_.buttonEditCombo->setEnabled(hasOneSelected);
   ui_.actionEditCombo->setEnabled(hasOneSelected);
   ui_.actionEnableDisableCombo->setEnabled(hasOneSelected);
   ui_.buttonEnableDisableCombo->setEnabled(hasOneSelected);
   ui_.actionExportCombo->setEnabled(hasOneOrMoreSelected);
   ui_.actionExportAllCombos->setEnabled(!listIsEmpty);

   QString enableDisableText = tr("Ena&ble");
   QString enableDisableToolTip = tr("Enable combo");
   if ((hasOneSelected) 
      && (ComboManager::instance().getComboListRef()[this->getSelectedComboIndexes().front()]->isEnabled()))
   {
      enableDisableText = "Disa&ble";
      enableDisableToolTip = "Disable combo";
   }
   ui_.actionEnableDisableCombo->setText(enableDisableText);
   ui_.actionEnableDisableCombo->setToolTip(enableDisableToolTip);
   ui_.actionEnableDisableCombo->setIconText(enableDisableToolTip);
   ui_.buttonEnableDisableCombo->setText(enableDisableText);
   ui_.buttonEnableDisableCombo->setToolTip(enableDisableToolTip);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionAddCombo()
{
   try
   {
      SPCombo const combo = Combo::create();
      if (!ComboDialog::run(combo, tr("Add Combo")))
         return;
      ComboManager& comboManager = ComboManager::instance();
      ComboList& comboList = ComboManager::instance().getComboListRef();
      if (!comboList.append(combo))
         throw xmilib::Exception(tr("The combo could not be added to the list."));
      QString errorMessage;
      if (!comboManager.saveComboListToFile(&errorMessage))
         throw xmilib::Exception(errorMessage);
   }
   catch (xmilib::Exception const& e)
   {
      QMessageBox::critical(this, tr("Error"), e.qwhat());
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionDuplicateCombo()
{
   try
   {
      QList<qint32> const selectedIndex = this->getSelectedComboIndexes();
      if (1 != selectedIndex.size())
         return;
      ComboManager& comboManager = ComboManager::instance();
      ComboList& comboList = comboManager.getComboListRef();
      qint32 const index = selectedIndex[0];
      Q_ASSERT((index >= 0) && (index < comboList.size()));
      if ((index < 0) || (index >= comboList.size()))
         throw xmilib::Exception(tr("The combo could not be duplicated: invalid index."));
      SPCombo const combo = Combo::duplicate(*comboList[index]);
      if (!ComboDialog::run(combo, tr("Duplicate Combo")))
         return;
      if (!comboList.append(combo))
         throw xmilib::Exception(tr("The duplicated combo could not added to the list."));
   }
   catch (xmilib::Exception const& e)
   {
      QMessageBox::critical(this, tr("Error"), e.qwhat());
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionDeleteCombo()
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
      comboManager.getComboListRef().erase(index);
   QString errorMessage;
   if (!comboManager.saveComboListToFile(&errorMessage))
      QMessageBox::critical(this, tr("Error"), errorMessage);
   this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionEditCombo()
{
   QList<qint32> const selectedIndex = this->getSelectedComboIndexes();
   if (1 != selectedIndex.size())
      return;

   ComboManager& comboManager = ComboManager::instance();
   ComboList& comboList = comboManager.getComboListRef();
   qint32 const index = selectedIndex[0];
   Q_ASSERT((index >= 0) && (index < comboList.size()));
   SPCombo const combo = comboList[index];
   if (!ComboDialog::run(combo, tr("Edit Combo")))
      return;
   comboList.markComboAsEdited(index);
   QString errorMessage;
   if (!comboManager.saveComboListToFile(&errorMessage))
      QMessageBox::critical(this, tr("Error"), errorMessage);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionSelectAll() const
{
   ui_.tableComboList->selectAll();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionDeselectAll() const
{
   ui_.tableComboList->clearSelection();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionEnableDisableCombo()
{
   QList<qint32> const selectedIndex = this->getSelectedComboIndexes();
   if (1 != selectedIndex.size())
      return;

   ComboManager& comboManager = ComboManager::instance();
   ComboList& comboList = comboManager.getComboListRef();
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
void ComboTableFrame::onActionExportCombo()
{
   QList<qint32> const indexes = this->getSelectedComboIndexes();
   if (indexes.size() < 1)
      return;

   QString const path = QFileDialog::getSaveFileName(this, tr("Export Combos"), QString(), 
      constants::kJsonFileDialogFilter);
   if (path.isEmpty())
      return;

   ComboList const& comboList = ComboManager::instance().getComboListRef();
   ComboList exportList;
   for (qint32 const index : indexes)
   {
      Q_ASSERT((index >= 0) && (index < comboList.size()));
      exportList.append(comboList[index]);
   }

   QString errorMsg;
   if (!exportList.save(path, &errorMsg))
   {
      globals::debugLog().addError(errorMsg);
      QMessageBox::critical(this, tr("Error"), tr("Could not save the combo list file."));
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionExportAllCombos()
{
   QString const path = QFileDialog::getSaveFileName(this, tr("Export All Combos"), QString(),
      constants::kJsonFileDialogFilter);
   if (path.isEmpty())
      return;
   QString errorMessage;
   if (!ComboManager::instance().getComboListRef().save(path, &errorMessage))
   {
      globals::debugLog().addError(errorMessage);
      QMessageBox::critical(this, tr("Error"), tr("Could not save the combo list file."));
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionImportCombos()
{
   QString const path = QFileDialog::getOpenFileName(this, tr("Import Combos"), QString(),
      constants::kJsonFileDialogFilter);
   if (path.isEmpty())
      return;
   ComboList importList;
   QString errorMessage;
   if (!importList.load(path, &errorMessage))
   {
      globals::debugLog().addError(errorMessage);
      QMessageBox::critical(this, tr("Error"), tr("Could not load the combo list from file."));
      return;
   }
   qint32 successCount = 0,failureCount = 0;
   ComboList& comboList = ComboManager::instance().getComboListRef();
   for (SPCombo const& combo : importList)
      if (comboList.append(combo))
         ++successCount;
      else
         ++failureCount;

   if (failureCount)
      QMessageBox::warning(this, tr("Import Combos"), tr("Combos successfully imported: %1\n\nCombos skipped to avoid "
         "duplicates: %2").arg(successCount).arg(failureCount));
   else
      QMessageBox::information(this, tr("Import Combos"), tr("Combos successfully imported: %1").arg(successCount));
   if ((successCount > 0) && (!ComboManager::instance().saveComboListToFile(&errorMessage)))
      QMessageBox::critical(this, tr("Error"), errorMessage);
   this->updateGui();
}


//**********************************************************************************************************************
/// \param[in] text The text to search
//**********************************************************************************************************************
void ComboTableFrame::onSearchFilterChanged(QString const& text)
{
   QString const searchStr = text.trimmed();
   proxyModel_.setFilterFixedString(searchStr);

}

//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onContextMenuRequested()
{
   contextMenu_.exec(QCursor::pos());
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onDoubleClick()
{
   switch (this->getSelectedComboCount())
   {
   case 0:
      this->onActionAddCombo();
      break;
   case 1:
      this->onActionEditCombo();
   default:
      break;
   }
}
