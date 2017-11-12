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


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the frame
//**********************************************************************************************************************
ComboTableFrame::ComboTableFrame(QWidget* parent)
   : QFrame(parent)
{
   ui_.setupUi(this);
   this->setupTable();
   this->updateGui();
   connect(new QShortcut(QKeySequence("Ctrl+F"), this), &QShortcut::activated,
      this, [&]() { this->ui_.editSearch->setFocus(); this->ui_.editSearch->selectAll(); });
   connect(new QShortcut(QKeySequence("Escape"), this), &QShortcut::activated,
      this, [&]() { this->ui_.editSearch->setFocus(); this->ui_.editSearch->clear(); });
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::setupTable()
{
   proxyModel_.setSourceModel(&ComboManager::instance().getComboListRef());
   ui_.tableComboList->setModel(&proxyModel_);
   proxyModel_.sort(0, Qt::AscendingOrder);
   connect(ui_.tableComboList->selectionModel(), &QItemSelectionModel::selectionChanged, this, 
      &ComboTableFrame::updateGui);
   connect(ui_.tableComboList, &QTableView::doubleClicked, this, &ComboTableFrame::onActionEditCombo);

   QHeaderView *verticalHeader = ui_.tableComboList->verticalHeader();
   verticalHeader->setDefaultSectionSize(verticalHeader->fontMetrics().height() + 10);
   ui_.tableComboList->setStyleSheet("QHeaderView:section { background: rgb(240, 240, 240); border: none; "
      "padding: 5px;}");
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
// 
//**********************************************************************************************************************
void ComboTableFrame::updateGui()
{
   qint32 const selectedCount = this->getSelectedComboCount();
   ui_.buttonEditCombo->setEnabled(1 == selectedCount);
   ui_.buttonDeleteCombo->setEnabled(selectedCount > 0);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionAddCombo()
{
   SPCombo combo = std::make_shared<Combo>();
   ComboDialog dlg(combo);
   if (QDialog::Accepted != dlg.exec())
      return;
   ComboManager& comboManager = ComboManager::instance();
   ComboList& comboList = ComboManager::instance().getComboListRef();
   comboList.append(combo);
   QString errorMessage;
   if (!comboManager.saveComboListToFile(&errorMessage))
      QMessageBox::critical(this, tr("Error"), errorMessage);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboTableFrame::onActionDeleteCombo()
{
   qint32 const count = this->getSelectedComboCount();
   if (count < 1)
      return;
   QString question = count > 1 ? tr("Are you sure you want to delete the selected combos?")
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
   qint32 index = selectedIndex[0];
   Q_ASSERT((index >= 0) && (index < comboList.size()));
   SPCombo combo = comboList[index];
   ComboDialog dlg(combo);
   if (QDialog::Accepted != dlg.exec())
      return;
   comboList.markComboAsEdited(index);
   QString errorMessage;
   if (!comboManager.saveComboListToFile(&errorMessage))
      QMessageBox::critical(this, tr("Error"), errorMessage);
}


//**********************************************************************************************************************
/// \param[in] text The text to search
//**********************************************************************************************************************
void ComboTableFrame::onSearchFilterChanged(QString const& text)
{
   qDebug() << QString("%1(%2)").arg(__FUNCTION__).arg(text);
   QString const searchStr = text.trimmed();
   proxyModel_.setFilterFixedString(searchStr);

}


