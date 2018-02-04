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
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ComboImportDialog::ComboImportDialog(QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
{
   ui_.setupUi(this);
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
   event->accept();
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
   ui_.buttonImport->setEnabled(comboList_.size() > 0);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboImportDialog::onActionImport()
{
   if (comboList_.isEmpty())
      return;
   qint32 successCount = 0,failureCount = 0;
   ComboList& comboList = ComboManager::instance().getComboListRef();
   for (SPCombo const& combo : comboList_)
      if (comboList.append(combo))
         ++successCount;
      else
         ++failureCount;

   if (failureCount)
      QMessageBox::warning(this, tr("Import Combos"), tr("Combos successfully imported: %1\n\nCombos skipped to avoid "
         "duplicates: %2").arg(successCount).arg(failureCount));
   else
      QMessageBox::information(this, tr("Import Combos"), tr("Combos successfully imported: %1").arg(successCount));

   QString errorMsg;
   if ((successCount > 0) && (!ComboManager::instance().saveComboListToFile(&errorMsg)))
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
   comboList_.clear();
   comboList_.load(text);
   this->updateGui();
}



