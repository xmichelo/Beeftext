/// \file
/// \author 
///
/// \brief Implementation of theme related functions.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "Theme.h"
#include "BeeftextGlobals.h"
#include <XMiLib/Exception.h>


QString loadStylesheetFile(QString const &path); ///< Load a stylesheet from file. The function throws an exception if an error occur.


//****************************************************************************************************************************************************
/// \param[in] theme The theme.
/// \return The display name of the theme.
//****************************************************************************************************************************************************
QString themeName(ETheme theme) {
    switch (theme) {
    case ETheme::Light:
        return QObject::tr("light");
    case ETheme::Dark:
        return QObject::tr("dark");
    case ETheme::Count:
    default:
        return QObject::tr("unknown");
    }
}


//****************************************************************************************************************************************************
/// \param[in] combo The combo box.
//****************************************************************************************************************************************************
void fillThemeComboBox(QComboBox &combo) {
    QSignalBlocker blocker(&combo);
    combo.clear();
    for (qint32 i = 0; i < static_cast<qint32>(ETheme::Count); ++i) {
        QString name = themeName(static_cast<ETheme>(i));
        if (name.length() > 0)
            name[0] = name[0].toUpper();
        combo.addItem(name, i);
    }
}


//****************************************************************************************************************************************************
/// \param[in] theme The theme.
/// \param[in] combo The combo box.
//****************************************************************************************************************************************************
void selectThemeInCombo(ETheme theme, QComboBox &combo) {
    for (qint32 i = 0; i < combo.count(); ++i) {
        bool ok = false;
        ETheme const itemTheme = static_cast<ETheme>(combo.itemData(i).toInt(&ok));
        if (!ok)
            continue;
        if (itemTheme == theme)
            combo.setCurrentIndex(i);
    }
}


//****************************************************************************************************************************************************
/// \param[in] combo The combo box.
/// \return The currently selected item in a combo box.
//****************************************************************************************************************************************************
ETheme selectedThemeInCombo(QComboBox const &combo) {
    qint32 index = qMax<qint32>(0, combo.currentIndex());
    if (index >= static_cast<qint32>(ETheme::Count))
        index = 0;
    return static_cast<ETheme>(index);
}


//****************************************************************************************************************************************************
/// \param[in] useCustomTheme Does the application use a custom theme
/// \param[in] theme The theme.
//****************************************************************************************************************************************************
void applyThemePreferences(bool useCustomTheme, ETheme theme) {
    try {
        if (!useCustomTheme) {
            qApp->setStyleSheet(loadStylesheetFile(":/MainWindow/Resources/StyleNoCustom.qss"));
            return;
        }

        qApp->setStyleSheet(loadStylesheetFile(":/MainWindow/Resources/StyleCommon.qss") + "\n" +
                            loadStylesheetFile(QString(":/MainWindow/Resources/Style%1.qss").arg((ETheme::Dark == theme)
                                                                                                 ? "Dark" : "Light")));
    }
    catch (xmilib::Exception const &e) {
        QString const &msg = e.qwhat();
        if (!msg.isEmpty())
            globals::debugLog().addWarning(msg);
        qApp->setStyleSheet(QString());
    }

}


//****************************************************************************************************************************************************
/// \param[in] path The path of the style sheet to load.
/// \return The loaded style sheet.
//****************************************************************************************************************************************************
QString loadStylesheetFile(QString const &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw xmilib::Exception(QString("Could not load stylesheet %1").arg(path));
    return QString::fromUtf8(file.readAll());
}