/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of combo import dialog
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboImportDialog.h"
#include "ComboManager.h"
#include "BeeftextConstants.h"


//**********************************************************************************************************************
/// \param[in] filePath The path of the file to import. Can be null
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ComboImportDialog::ComboImportDialog(QString const& filePath, QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
{
   ui_.setupUi(this);
   if (!filePath.isEmpty())
      ui_.editPath->setText(QDir::toNativeSeparators(filePath));
   else
      this->updateGui();
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void ComboImportDialog::dragEnterEvent(QDragEnterEvent* event)
{
   if (event->mimeData()->hasUrls()) // we only accept files
      event->acceptProposedAction();
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void ComboImportDialog::dragMoveEvent(QDragMoveEvent* event)
{
   if (event->mimeData()->hasUrls()) // we only accept files
      event->acceptProposedAction();
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void ComboImportDialog::dragLeaveEvent(QDragLeaveEvent* event)
{
   event->accept();
}


//**********************************************************************************************************************
/// \param[in] event The event
//**********************************************************************************************************************
void ComboImportDialog::dropEvent(QDropEvent* event)
{
   QMimeData const* mimeData = event->mimeData();
   if (!mimeData->hasUrls())
      return;
   QList<QUrl> urls = mimeData->urls();
   if (urls.size() > 0) // should always be the case
      ui_.editPath->setText(QDir::toNativeSeparators(urls[0].toLocalFile()));
   event->acceptProposedAction();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboImportDialog::updateGui()
{
   ui_.buttonImport->setEnabled(importableCombos_.size() > 0);
   qint32 const importableCount = importableCombos_.size();
   if ((!importableCount) && (!skippedComboCount_))
   {
      if (ui_.editPath->text().isEmpty())
         ui_.labelStatus->setText("\n");
      else
         ui_.labelStatus->setText(tr("This file is not a valid combo list file."));
      return;
   }
   if (!importableCount)
   {
      if (skippedComboCount_ > 1)
         ui_.labelStatus->setText(tr("None of the %1 combos in this file can be imported (duplicates or conflicts).")
            .arg(skippedComboCount_));
      else
         ui_.labelStatus->setText(tr("The combo in this file cannot be imported (duplicate or conflict)."));
      return;
   }
   QString label = (importableCount > 1) ? tr("%1 combos will be imported.\n").arg(importableCount) : 
      tr("1 combo will be imported.\n");
   if (skippedComboCount_)
      label += (skippedComboCount_ > 1) ? tr("%1 combos will be skipped (duplicates or conflicts).")
         .arg(skippedComboCount_)  : tr("1 combo will be skipped (duplicate or conflict).")
         ;
   ui_.labelStatus->setText(label);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboImportDialog::onActionImport()
{
   if (importableCombos_.isEmpty())
      return;
   qint32 successCount = 0,failureCount = 0;
   ComboList& comboList = ComboManager::instance().getComboListRef();
   for (SPCombo const& combo : importableCombos_)
      if (!comboList.append(combo))
         ++failureCount;
   if (failureCount > 0)
      QMessageBox::critical(this, tr("error"), tr("%1 combo(s) could not be imported."));
   QString errorMsg;
   if ((!ComboManager::instance().saveComboListToFile(&errorMsg)))
      QMessageBox::critical(this, tr("Error"), errorMsg);
   this->accept();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboImportDialog::onActionCancel()
{
   this->reject();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboImportDialog::onActionBrowse()
{
   QString const path = QFileDialog::getOpenFileName(this, tr("Select Combo File"), QString(),
      constants::kJsonFileDialogFilter);
   if (path.isEmpty())
      return;
   ui_.editPath->setText(QDir::toNativeSeparators(path));
}


//**********************************************************************************************************************
/// \param[in] text The new text of the edit control
//**********************************************************************************************************************
void ComboImportDialog::onEditPathTextChanged(QString const& text)
{
   importableCombos_.clear();
   skippedComboCount_ = 0;
   ComboList candidateList;
   ComboList& comboList = ComboManager::instance().getComboListRef();
   if (!candidateList.load(text))
   {
      this->updateGui();
      return;
   }
   for (SPCombo const& combo : candidateList)
   {
      if ((!comboList.canComboBeAdded(combo)) || (!importableCombos_.append(combo)))
         ++skippedComboCount_;
   }
   this->updateGui();
}



