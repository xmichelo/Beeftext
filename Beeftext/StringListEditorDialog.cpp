/// \file
/// \author 
///
/// \brief Implementation of string list editor dialog class
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "StringListEditorDialog.h"


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
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void StringListEditorDialog::onActionOk()
{
   this->accept();
}
