/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of utility functions for the Beeftext application


#ifndef BEEFTEXT__UTILS__H
#define BEEFTEXT__UTILS__H


bool registerApplicationForAutoStart(); ///< Register the application to be automatically started at login
void unregisterApplicationFromAutoStart(); ///< Unregister the application to be automatically started at login
void openLogFile(); ///< Open the log file with the default text editor


#endif // #ifndef BEEFTEXT__UTILS__H
