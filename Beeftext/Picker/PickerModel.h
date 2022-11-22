/// \file
/// \author 
///
/// \brief Declaration of picker model
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEEXT_PICKER_MODEL_H
#define BEEFTEEXT_PICKER_MODEL_H


//****************************************************************************************************************************************************
/// \brief Picker model for the picker window list view
//****************************************************************************************************************************************************
class PickerModel : public QAbstractListModel {
Q_OBJECT
public: // member functions
    explicit PickerModel(QObject *parent = nullptr); ///< Default constructor
    PickerModel(PickerModel const &) = delete; ///< Disabled copy-constructor
    PickerModel(PickerModel &&) = delete; ///< Disabled assignment copy-constructor
    ~PickerModel() override = default; ///< Destructor
    PickerModel &operator=(PickerModel const &) = delete; ///< Disabled assignment operator
    PickerModel &operator=(PickerModel &&) = delete; ///< Disabled move assignment operator
    int rowCount(const QModelIndex &parent) const override; ///< return the number of rows in the model
    QVariant data(const QModelIndex &index, int role) const override; ///< Return the data for a given role at a given index

public slots: // member functions
    void resetModel(); ///< Make the model as reset
};


#endif // #ifndef BEEFTEEXT_PICKER_MODEL_H
