/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of the application's main window
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "MainWindow.h"
#include "Combo/ComboManager.h"
#include "Combo/ComboDialog.h"
#include "BeeftextConstants.h"
#include "BeeftextGlobals.h"


using namespace xmilib;


//**********************************************************************************************************************
//
//**********************************************************************************************************************
MainWindow::MainWindow()
   : QMainWindow()
   , debugLogWindow_(std::make_unique<DebugLogWindow>(&globals::debugLog()))
{
    ui_.setupUi(this);
    ui_.tabWidget->setCurrentIndex(0);
    
    this->setupComboTable(); 
    this->setupActions();
    this->setupSystemTrayIcon();
    this->updateGui();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::setupComboTable()
{
   proxyModel_.setSourceModel(&ComboManager::instance().getComboListRef());
   proxyModel_.setFilterCaseSensitivity(Qt::CaseInsensitive);
   ui_.tableComboList->setModel(&proxyModel_);
   proxyModel_.sort(0, Qt::AscendingOrder);
   connect(ui_.tableComboList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updateGui);
   connect(ui_.tableComboList, &QTableView::doubleClicked, this, &MainWindow::onActionEditCombo);

   QHeaderView *verticalHeader = ui_.tableComboList->verticalHeader();
   verticalHeader->setDefaultSectionSize(verticalHeader->fontMetrics().height() + 10);
   ui_.tableComboList->setStyleSheet("QHeaderView:section { background: rgb(240, 240, 240); border: none; "
      "padding: 5px;}");
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::setupActions()
{
   QString const exitActionText = tr("E&xit %1").arg(constants::kApplicationName);
   ui_.actionExit->setText(exitActionText);
   ui_.actionExit->setToolTip(exitActionText);
   ui_.actionExit->setIconText(exitActionText);

   QString const showActionText = tr("&Open %1").arg(constants::kApplicationName);
   ui_.actionShowMainWindow->setText(showActionText);
   ui_.actionShowMainWindow->setToolTip(showActionText);
   ui_.actionShowMainWindow->setIconText(showActionText);

   connect(&systemTrayIcon_, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayIconActivated);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::setupSystemTrayIcon()
{
   systemTrayIcon_.setIcon(QIcon(":/MainWindow/Resources/BeeftextIcon.ico"));
   systemTrayIcon_.setToolTip(constants::kApplicationName);
   systemTrayIcon_.show();

   QMenu* menu = new QMenu(this);
   menu->addAction(ui_.actionShowMainWindow);
   menu->addAction(ui_.actionShowLog);
   menu->addSeparator();
   menu->addAction(ui_.actionExit);
   menu->setActiveAction(ui_.actionShowMainWindow);

   systemTrayIcon_.setContextMenu(menu);
}


//**********************************************************************************************************************
/// \return The number of selected combos
//**********************************************************************************************************************
qint32 MainWindow::getSelectedComboCount() const
{
   return ui_.tableComboList->selectionModel()->selectedRows().size();
}


//**********************************************************************************************************************
/// The returned indexes are based on the internal order of the list, not the display order in the table (that can be
/// modified by sorting by columns).
///
/// \return The list of indexes of the selected combos 
//**********************************************************************************************************************
QList<qint32> MainWindow::getSelectedComboIndexes() const
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
void MainWindow::updateGui()
{
   qint32 const selectedCount = this->getSelectedComboCount();
   ui_.buttonEditCombo->setEnabled(1 == selectedCount);
   ui_.buttonDeleteCombo->setEnabled(selectedCount > 0);
}


//**********************************************************************************************************************
/// An 'activation' in an action performed on the system tray icon.
/// \param[in] reason The reason for the activation
//**********************************************************************************************************************
void MainWindow::onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
   switch (reason)
   {
   case QSystemTrayIcon::DoubleClick:
      this->onActionShowMainWindow();
      break;
   case QSystemTrayIcon::Trigger: // a.k.a single click
      systemTrayIcon_.contextMenu()->popup(QCursor::pos());
      break;
   }
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionShowMainWindow()
{
   this->show();
   this->raise();
   this->activateWindow();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionExit()
{
   qApp->quit();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionShowDebugLog()
{
   Q_ASSERT(debugLogWindow_);
   debugLogWindow_->show();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionAddCombo()
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
void MainWindow::onActionDeleteCombo()
{
   qDebug() << QString("%1()").arg(__FUNCTION__);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void MainWindow::onActionEditCombo()
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



