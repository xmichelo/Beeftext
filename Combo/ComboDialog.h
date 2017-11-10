/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of dialog for creating/editing a combo


#ifndef BEEFTEXT__COMBO__DIALOG__H
#define BEEFTEXT__COMBO__DIALOG__H


#include "ui_ComboDialog.h"
#include "Combo/Combo.h"


//**********************************************************************************************************************
/// \brief A dialog class for creating and editing a combo
//**********************************************************************************************************************
class ComboDialog: public QDialog
{
   Q_OBJECT

public: // member functions
	ComboDialog(SPCombo combo, QWidget* parent = nullptr); ///< Default constructor
	~ComboDialog() = default; ///< Default destructor
	SPCombo combo() const; ///< Get the combo

private: // member functions
	ComboDialog(ComboDialog const&) = delete; ///< Disabled copy constructor
	ComboDialog(ComboDialog&&) = delete; ///< Disabled move constructor
	ComboDialog& operator=(ComboDialog const&) = delete; ///< Disabled assignment operator
	ComboDialog& operator=(ComboDialog&&) = delete; ///< Disabled move assignment operator

private slots:
   void onActionOk(); ///< Slot for the 'OK' action

private: // data members
   Ui::ComboDialog ui_; ///< The GUI for the dialog
   SPCombo combo_; ///< The combo
};


#endif // #ifndef BEEFTEXT__COMBO__DIALOG__H