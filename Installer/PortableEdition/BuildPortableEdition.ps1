# This script builds the portable edition of Beeftext
#
# Author: Xavier Michelon
#
# Copyright (c) Xavier Michelon. All rights reserved.  
# Licensed under the MIT License. See LICENSE file in the project root for full license information.  

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "..\..\Resources\Powershell\Beeftext.ps1")

#***********************************************************************************************************************
# Variable definitions
#***********************************************************************************************************************
$dstDir = absolutePath $solutionDir "_build\BeeftextPortableEdition"
$solutionPath = absolutePath $solutionDir "Beeftext.sln"
$exePath = absolutePath $solutionDir "_build\Win32\Release\Beeftext.exe"
$beaconFileName = "Portable.bin"

#***********************************************************************************************************************
# The actual script
#***********************************************************************************************************************
"Cleaning destination folder"
if (Test-Path -PathType Container $dstDir) { Remove-Item -Path $dstDir -Recurse -Force }

"Compiling Beeftext"
compileVisualStudioSolution $solutionPath

"Creating destination folder"
New-Item -ItemType Directory -Force -Path $dstDir | Out-Null

"Copying program files to the destination folder"
Copy-Item -Path $exePath -Destination $dstDir

"Copying Qt DLL files"
copyQtDlls $dstDir

"Copying README.md file"
Copy-Item -Path (absolutePath $solutionDir "README.md") -Destination $dstDir

"Copy SSL DLL files"
copySslDlls $dstDir

"Copy Visual C++ DLL files"
copyVcppDlls $dstDir

"Creating Portable Edition beacon file"
"Do not delete this file" | Set-Content -Path (absolutePath $dstDir $beaconFileName)

