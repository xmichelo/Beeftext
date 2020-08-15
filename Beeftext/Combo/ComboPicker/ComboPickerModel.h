/// \file
/// \author 
///
/// \brief Declaration of combo picker model
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEEXT_COMBO_PICKER_MODEL_H
#define BEEFTEEXT_COMBO_PICKER_MODEL_H


//**********************************************************************************************************************
/// \brief Combo picker model for the combo picker window list view
//**********************************************************************************************************************
class ComboPickerModel: public QAbstractListModel
{
   Q_OBJECT
public: // member functions
   explicit ComboPickerModel(QObject* parent = nullptr); ///< Default constructor
   ComboPickerModel(ComboPickerModel const&) = delete; ///< Disabled copy-constructor
   ComboPickerModel(ComboPickerModel&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboPickerModel() override = default; ///< Destructor
   ComboPickerModel& operator=(ComboPickerModel const&) = delete; ///< Disabled assignment operator
   ComboPickerModel& operator=(ComboPickerModel&&) = delete; ///< Disabled move assignment operator
   int rowCount(const QModelIndex& parent) const override; ///< return the number of rows in the model
   QVariant data(const QModelIndex& index, int role) const override; ///< Return the data for a given role at a given index

public slots: // member functions
   void resetModel(); ///< Make the model as reset
};


#endif // #ifndef BEEFTEEXT_COMBO_PICKER_MODEL_H
