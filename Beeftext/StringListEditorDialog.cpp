/// \file
/// \author 
///
/// \brief Implementation of string list editor dialog class
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "StringListEditorDialog.h"
#include <XMiLib/FileUtils.h>
#include <XMiLib/Exception.h>
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"


namespace {
   Qt::WindowFlags const kDefaultDialogFlags(Qt::WindowCloseButtonHint | Qt::WindowTitleHint); ///< The default dialog flags
}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
void StringListEditorDialog::run(QWidget* parent)
{
   StringListEditorDialog(parent).exec();
}


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
StringListEditorDialog::StringListEditorDialog(QWidget* parent)
   : QDialog(parent, kDefaultDialogFlags)
{
   ui_.setupUi(this);
   ui_.stringListView->setModel(&model_);
   QItemSelectionModel const * const selModel = ui_.stringListView->selectionModel();
   if (selModel)
      connect(selModel, &QItemSelectionModel::selectionChanged, this, &StringListEditorDialog::onSelectionChanged);
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void StringListEditorDialog::onActionOk()
{
   qDebug () << model_.stringList();
   this->accept();
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
