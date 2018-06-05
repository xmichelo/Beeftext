# This script builds update the version number in the installer configuration files based
# on the version number value found in the source code
#
# Author: Xavier Michelon
#
# Copyright (c) Xavier Michelon. All rights reserved.  
# Licensed under the MIT License. See LICENSE file in the project root for full license information.  

param([String]$dstDir)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "Beeftext.ps1")

"Performing requirement checks"
checkQtInstallation
if ([String]::IsNullOrEmpty($dstDir)) { throw "Missing parameter -dstDir <outputDirectory>" }

"Creating destination folder"
New-Item -ItemType Directory -Force -Path $dstDir | Out-Null

"Compiling XMiLib French translation file"
compileTranslationFiles (absolutePath $solutionDir "Submodules\XMiLib\XMiLib\Translations\xmilib_fr.ts") $dstDir

"Compiling Beeftext French translation file"
compileTranslationFiles (absolutePath $solutionDir "Beeftext\Translations\beeftext_fr.ts") $dstDir

"Copying Qt French translation"
Copy-Item -Path (absolutePath $qtDir "translations\qtbase_fr.qm") -Destination $dstDir
