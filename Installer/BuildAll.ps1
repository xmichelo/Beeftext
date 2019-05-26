# This script builds both the installer-based and Portable Edition of Beeftext
#
# Author: Xavier Michelon
#
# Copyright (c) Xavier Michelon. All rights reserved.  
# Licensed under the MIT License. See LICENSE file in the project root for full license information.  

$ErrorActionPreference = "Stop"


$nsisPath = "C:\Program Files (x86)\NSIS\makensis.exe"


#***********************************************************************************************************************
# Compiles the NSIS installer
#***********************************************************************************************************************
function compileNsisInstaller
{
    "Generating Beeftext installer"
    $buildLog = Invoke-Expression "& `"$nsisPath`" `"Installer\installer.nsi`"" | Out-Null
    if (0 -ne $LASTEXITCODE)
    {
       $buildLog # dump the build log
       Write-Error "Generation of the installer failed"
    }
}

compileNsisInstaller
. (Join-Path $PSScriptRoot "PortableEdition\BuildPortableEdition.ps1")
