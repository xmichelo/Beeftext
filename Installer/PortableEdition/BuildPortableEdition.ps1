# This script builds the portable edition of Abbrevio
#
# Author: Xavier Michelon
#
# Copyright (c) Xavier Michelon. All rights reserved.  
# Licensed under the MIT License. See LICENSE file in the project root for full license information.  

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "..\..\Scripts\PowerShell\Abbrevio.ps1")

#***********************************************************************************************************************
# Variable definitions
#***********************************************************************************************************************
$major, $minor = getAbbrevioVersion
$buildFolder = absolutePath $solutionDir "Installer\_build"
$dstDir = absolutePath $buildFolder "AbbrevioPortableEdition"
$dstZipFilePath = (absolutePath $buildFolder "Abbrevio-$major.$minor-PortableEdition.zip")
$solutionPath = absolutePath $solutionDir "Abbrevio.sln"
$exeDir = absolutePath $solutionDir "_build\x64\Release"
$exePath = absolutePath $exeDir "Abbrevio.exe"
$srcTransDir = absolutePath $exeDir "translations"
$dstTransDir = absolutePath $dstDir "translations"
$emojiDir = absolutePath $dstDir "emojis"
$beaconFileName = "Portable.bin"


#***********************************************************************************************************************
# The actual script
#***********************************************************************************************************************
function buildAbbrevioPortableEdition
{
    "Cleaning old build files"
    if (Test-Path -PathType Container $dstDir) { Remove-Item -Path $dstDir -Recurse -Force }
    if (Test-Path -PathType Leaf $dstZipFilePath) { Remove-Item -Path $dstZipFilePath -Force }

    "Compiling Abbrevio"
    compileVisualStudioSolution $solutionPath

    "Creating destination folder"
    New-Item -ItemType Directory -Force -Path $dstDir | Out-Null

    "Copying program files to the destination folder"
    Copy-Item -Path $exePath -Destination $dstDir

    "Copying Qt DLL files"
    copyQtDlls $dstDir

    "Copying README.md file"
    Copy-Item -Path (absolutePath $solutionDir "README.md") -Destination $dstDir

    "Copying SSL DLL files"
    copySslDlls $dstDir

    "Copying Visual C++ DLL files"
    copyVcppDlls $dstDir

    "Creating emojis folder"
    New-Item -ItemType Directory -Force -path $emojiDir | Out-Null

    "Copying emoji file"
    Copy-Item -Path (absolutePath $solutionDir "Submodules\emojilib\emojis.json") -Destination $emojiDir

    "Creating Portable Edition beacon file"
    "Do not delete this file" | Set-Content -Path (absolutePath $dstDir $beaconFileName)

    "Copying translation files"
    Copy-Item $srcTransDir -Destination $dstTransDir -Recurse

    "Zipping destination folder"
    Compress-Archive -Path $dstDir -Force -CompressionLevel Optimal -DestinationPath $dstZipFilePath

    "Removing destination folder"
    Remove-Item -Path $dstDir -Recurse -Force
}

buildAbbrevioPortableEdition