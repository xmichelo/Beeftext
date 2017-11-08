/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of dialog for creating/editing a combo


#include "stdafx.h"
#include "ComboDialog.h"
#include "BeeftextConstants.h"


//**********************************************************************************************************************
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ComboDialog::ComboDialog(QWidget* parent)
   : QDialog(parent, constants::kDefaultDialogFlags)
{
   ui_.setupUi(this);
}

