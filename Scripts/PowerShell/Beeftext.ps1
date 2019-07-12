# This script contain useful PowerShell functions for Beeftext
#
# Author: Xavier Michelon
#
# Copyright (c) Xavier Michelon. All rights reserved.  
# Licensed under the MIT License. See LICENSE file in the project root for full license information.  


. (Join-Path $PSScriptRoot "..\..\Submodules\XMiLib\Scripts\PowerShell\Utils.ps1")

#***********************************************************************************************************************
# useful variable declarations
#***********************************************************************************************************************
$openSslBinDir = "C:\Qt\Tools\OpenSSL\Win_x86\bin"
$solutionDir = absolutePath $PSScriptRoot "..\.."
$vcDllDir = absolutePath $solutionDir "Vendor\VCRuntime\"
$nsisInstallerPath = absolutePath $solutionDir "Installer\installer.nsi"
$rcFilePath = absolutePath $solutionDir "Beeftext\Beeftext.rc"


#***********************************************************************************************************************
# Copy The SSL DLL files required by Beeftext ot the destination folder
#***********************************************************************************************************************
function copySslDlls([String]$dstPath)
{
    foreach ($dll in "libcrypto-1_1.dll", "libssl-1_1.dll") {Copy-Item -Path (absolutePath $openSslBinDir $dll) -Destination $dstPath }
}


#***********************************************************************************************************************
# Copy The Visual C++ runtime DLL files required by Beeftext ot the destination folder
#***********************************************************************************************************************
function copyVcppDlls([String]$dstPath)
{
   $vcDlls = "msvcp140.dll", "vcruntime140.dll", "msvcr100.dll"
   foreach ($dll in $vcDlls) { Copy-Item -Path (absolutePath $vcDllDir $dll) -Destination $dstPath }
}


#***********************************************************************************************************************
# Retrieve the major and minor version number of Beeftext from its source socde
#***********************************************************************************************************************
function getBeeftextVersion
{
   $srcFile = absolutePath $solutionDir "Beeftext\BeeftextConstants.cpp"
   $regexMinor = 'kVersionMinor\s*=\s*(\d+)\s*;' 
   $regexMajor = 'kVersionMajor\s*=\s*(\d+)\s*;' 
   $major = Select-String -Path $srcFile -Pattern $regexMajor | ForEach-Object {$_.Matches} | 
      ForEach-Object { $_.Groups[1].Value }
   $minor = Select-String -Path $srcFile -Pattern $regexMinor | ForEach-Object {$_.Matches} |
       ForEach-Object { $_.Groups[1].Value }
   if ([string]::IsNullOrEmpty($major) -Or [string]::IsNullOrEmpty($minor)) 
   {  
      Write-Error "Could not parse version number"
   }
   @($major, $minor)
}


#***********************************************************************************************************************
# Update the version number in the installer file
#***********************************************************************************************************************
function updateVersionNumberInInstaller
{
    $major, $minor = getBeeftextVersion
    (Get-Content $nsisInstallerPath) `
        -replace '(!define\s*VERSION_MAJOR)\s*\d+', ('$1 ' + $major) `
        -replace '(!define\s*VERSION_MINOR)\s*\d+', ('$1 ' + $minor) |
        Out-File $nsisInstallerPath -Encoding "UTF8"
}


#***********************************************************************************************************************
# Update the version number in the RC file
#***********************************************************************************************************************
function updateVersionNumberInRcFile
{
    $major, $minor = getBeeftextVersion
    (Get-Content $rcFilePath) `
        -replace '^(#define\s+VERSION_NUMBER)\s+(\d+)\s*,\s*(\d+)(\s*,\s*0\s*,\s*0\s*$)', ('$1 ' + $major + ',' + $minor + ',0,0') `
        -replace '^(#define\s+VERSION_STRING)\s+"(\d+)\.(\d+)\\0"\s*$', ('$1 "' + $major + '.' + $minor + '\0"') |
        Out-File $rcFilePath -Encoding "ASCII" 
        # Note we use ASCII for output  as UTF8 would cause the insertion of a 3-bytes BOM at the beginning of the file
        # that will screw the RC file for VS.
}
