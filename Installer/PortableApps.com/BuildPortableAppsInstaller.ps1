# This script builds the PortableApps.com version of Beeftext
#
# Author: Xavier Michelon
#
# Copyright (c) Xavier Michelon. All rights reserved.  
# Licensed under the MIT License. See LICENSE file in the project root for full license information.  

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "..\..\Scripts\PowerShell\Beeftext.ps1")

#***********************************************************************************************************************
# Variable definitions
#***********************************************************************************************************************
$dstParentDir = absolutePath $solutionDir "Installer\_build\PortableApps.com"
$dstDir = absolutePath $dstParentDir "BeeftextPortable"
$dstBinDir = absolutePath $dstDir "App\Beeftext"
$dstDataDir = absolutePath $dstDir "Data"
$solutionPath = absolutePath $solutionDir "Beeftext.sln"
$templateDir = absolutePath $PSScriptRoot "Template\BeeftextPortable"
$srcBinDir = absolutePath $solutionDir "_build\Win32\Release"
$exePath = absolutePath $srcBinDir "Beeftext.exe"
$srcTransDir = absolutePath $srcBinDir "translations"
$dstTransDir = absolutePath $DstBinDir "translations"
$installerGeneratorPath = "C:\Program Files (x86)\PortableApps.comInstaller\PortableApps.comInstaller.exe"
$beaconFileName = "PortableApps.bin"
$emojiDir = absolutePath $dstBinDir "emojis" 

#***********************************************************************************************************************
# The actual script
#***********************************************************************************************************************
"Cleaning destination folder"
if (Test-Path -PathType Container $dstParentDir) { Remove-Item -Path $dstParentDir -Recurse -Force }

"Compiling Beeftext"
compileVisualStudioSolution $solutionPath

"Creating destination folder"
New-Item -ItemType Directory -Force -Path $dstParentDir | Out-Null

"Copying template to the destination folder"
Copy-Item -Path $templateDir -Destination $dstParentDir -Recurse -Force

"Creating binary folder"
if (Test-Path -PathType Container $dstBinDir) { Remove-Item -Path $dstBinDir
 -Recurse -Force }
New-Item -ItemType Directory -Force -Path $dstBinDir | Out-Null

"Deleting Data folder"
if (Test-Path -PathType Container $dstDataDir) { Remove-Item -Path $dstDataDir -Recurse -Force }

"Copying program files to the destination folder"
Copy-Item -Path $exePath -Destination $dstBinDir

"Copying Qt DLL files"
copyQtDlls $dstBinDir

"Copying README.md file"
Copy-Item -Path (absolutePath $solutionDir "README.md") -Destination $dstBinDir

"Copying SSL DLL files"
copySslDlls $dstBinDir

"Copying Visual C++ DLL files"
copyVcppDlls $dstBinDir

"Copying translation files"
Copy-Item $srcTransDir -Destination $dstTransDir -Recurse

"Creating emojis folder"
New-Item -ItemType Directory -Force -Path $emojiDir | Out-Null

"Copying emojis file"
Copy-Item -Path (absolutePath $solutionDir "Submodules\emojilib\emojis.json") -Destination $emojiDir

"Creating PortablesApps.com beacon file"
"Do not delete this file" | Set-Content -Path (absolutePath $dstBinDir $beaconFileName)

"Generating installer executable"
Invoke-Expression '& "$installerGeneratorPath" "$dstDir"' | Out-Null
