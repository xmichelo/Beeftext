/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of application entry point
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "MainWindow.h"
#include "BeeftextConstants.h"


//**********************************************************************************************************************
/// \brief Application entry point
///
/// \param[in] argc The number of command line arguments
/// \param[in] argv The list of command line arguments
//**********************************************************************************************************************
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(constants::kOrganizationName);
    app.setApplicationName(constants::kApplicationName);
    app.setApplicationDisplayName(constants::kApplicationName);
    MainWindow w;
    w.show();
    return app.exec();
}
