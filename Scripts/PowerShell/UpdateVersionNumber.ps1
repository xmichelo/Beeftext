# This script builds update the version number in the installer configuration files based
# on the version number value found in the source code
#
# Author: Xavier Michelon
#
# Copyright (c) Xavier Michelon. All rights reserved.  
# Licensed under the MIT License. See LICENSE file in the project root for full license information.  

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "Beeftext.ps1")

updateVersionNumberInInstaller
updateVersionNumberInRcFile
