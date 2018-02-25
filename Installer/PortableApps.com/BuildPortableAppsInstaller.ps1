# This script builds the PortableApps.com version of Beeftext
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
$dstParentDir = absolutePath $solutionDir "_build\PortableApps.com"
$dstDir = absolutePath $dstParentDir "BeeftextPortable"
$dstBinPath = absolutePath $dstDir "App\Beeftext"
$dstDataDir = absolutePath $dstDir "Data"
$solutionPath = absolutePath $solutionDir "Beeftext.sln"
$templateDir = absolutePath $PSScriptRoot "Template\BeeftextPortable"
$exePath = absolutePath $solutionDir "_build\Win32\Release\Beeftext.exe"

$installerGeneratorPath = "C:\Program Files (x86)\PortableApps.comInstaller\PortableApps.comInstaller.exe"

$beaconFileName = "PortableApps.bin"

#***********************************************************************************************************************
# The actual script
#***********************************************************************************************************************
"Compiling Beeftext"
compileVisualStudioSolution $solutionPath

"Cleaning destination folder"
if (Test-Path -PathType Container $dstParentDir) { Remove-Item -Path $dstParentDir -Recurse -Force }
New-Item -ItemType Directory -Force -Path $dstParentDir | Out-Null

"Copying template to the destination folder"
Copy-Item -Path $templateDir -Destination $dstParentDir -Recurse -Force

"Deleting Data folder"
if (Test-Path -PathType Container $dstDataDir) { Remove-Item -Path $dstDataDir -Recurse -Force }

"Copying program files to the destination folder"
Copy-Item -Path $exePath -Destination $dstBinPath

"Copying Qt DLL files"
copyQtDlls $dstBinPath

"Copying README.md file"
Copy-Item -Path (absolutePath $solutionDir "README.md") -Destination $dstBinPath

"Copy SSL DLL files"
copySslDlls $dstBinPath

"Copy Visual C++ DLL files"
copyVcppDlls $dstBinPath

"Creating PortablesApps.com beacon file"
"Do not delete this file" | Set-Content -Path (absolutePath $dstBinPath $beaconFileName)

"Generating installer executable"
Invoke-Expression '& "$installerGeneratorPath" "$dstDir"' | Out-Null
