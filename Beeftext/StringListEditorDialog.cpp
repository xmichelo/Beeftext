/// \file
/// \author 
///
/// \brief Implementation of string list editor dialog class
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "StringListEditorDialog.h"
#include <XMiLib/Exception.h>
#include "BeeftextUtils.h"


namespace {
   Qt::WindowFlags const kDefaultDialogFlags(Qt::WindowCloseButtonHint | Qt::WindowTitleHint); ///< The default dialog flags
}


//**********************************************************************************************************************
/// \param[in] stringList The string list
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
StringListEditorDialog::StringListEditorDialog(QStringList const& stringList, QWidget* parent)
   : QDialog(parent, kDefaultDialogFlags)
   , model_(stringList)
{
   ui_.setupUi(this);
   lastActionButton_ = ui_.buttonRemove;
   ui_.stringListView->setModel(&model_);
   QItemSelectionModel const * const selModel = ui_.stringListView->selectionModel();
   if (selModel)
      connect(selModel, &QItemSelectionModel::selectionChanged, this, &StringListEditorDialog::onSelectionChanged);
   this->setHeaderText(QString());
   this->updateGui();
}


//**********************************************************************************************************************
/// \return The string list
//**********************************************************************************************************************
QStringList StringListEditorDialog::stringList() const
{
   return model_.stringList();
}


//**********************************************************************************************************************
/// \param[in] stringList The string list
//**********************************************************************************************************************
void StringListEditorDialog::setStringList(QStringList const& stringList)
{
   model_.setStringList(stringList);
}


//**********************************************************************************************************************
/// \param[in] text The text
//**********************************************************************************************************************
void StringListEditorDialog::setHeaderText(QString const& text) const
{
   ui_.labelHeader->setText(text);
   ui_.labelHeader->setVisible(!text.isEmpty());
}


//**********************************************************************************************************************
/// \param[in] button The button to add
//**********************************************************************************************************************
void StringListEditorDialog::addCustomButton(QPushButton* button)
{
   ui_.buttonLayout->addWidget(button);
   QWidget::setTabOrder(lastActionButton_, button);
   QWidget::setTabOrder(button, ui_.buttonOk);
   lastActionButton_ = button;
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void StringListEditorDialog::onActionAddString()
{
   try
   {
      qint32 const row = model_.rowCount();
      model_.insertRows(row, 1);
      QModelIndex const index = model_.index(row);
      ui_.stringListView->setCurrentIndex(index);
      ui_.stringListView->edit(index);
      this->updateGui();
   }
   catch (xmilib::Exception const& e)
   {
      reportInternalError(QString("%1(): %2").arg(__FUNCTION__).arg(e.qwhat()), this);
   }
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void StringListEditorDialog::onActionRemoveString()
{
   try
   {
      QItemSelectionModel const * const selModel = ui_.stringListView->selectionModel();
      if (!selModel)
         throw xmilib::Exception(tr("Could not retrieve selection model."));
      QModelIndexList selection = selModel->selectedRows(); // we sort the select by descending column so that removal does not shift indexes
      std::sort(selection.begin(), selection.end(), [](QModelIndex const& a, QModelIndex const& b) -> bool 
         {return a.row() > b.row(); });
      for (QModelIndex const& index: selection)
         model_.removeRows(index.row(), 1);
      this->updateGui();
   }
   catch (xmilib::Exception const& e)
   {
      reportInternalError(QString("%1(): %2").arg(__FUNCTION__).arg(e.qwhat()), this);
   }
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void StringListEditorDialog::onSelectionChanged()
{
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void StringListEditorDialog::updateGui()
{
   try
   {
      QItemSelectionModel const * const selModel = ui_.stringListView->selectionModel();
      if (!selModel)
         throw xmilib::Exception(tr("Could not retrieve selection model."));
      qint32 const selectedRowCount = selModel->selectedRows().size();

      ui_.buttonRemove->setEnabled(selectedRowCount > 0);
   }
   catch (xmilib::Exception const& e)
   {
      reportInternalError(QString("%1(): %2").arg(__FUNCTION__).arg(e.qwhat()), this);
   }
}
