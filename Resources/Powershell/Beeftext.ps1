# This script contain useful PowerShell functions for Beeftext
#
# Author: Xavier Michelon
#
# Copyright (c) Xavier Michelon. All rights reserved.  
# Licensed under the MIT License. See LICENSE file in the project root for full license information.  


$vsPath = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.com"

#***********************************************************************************************************************
# Returns the absolute path obtain by merging path1 and path2
#***********************************************************************************************************************
function absolutePath([String]$path)
{
   [System.IO.Path]::GetFullPath($path)
}


#***********************************************************************************************************************
# Returns the absolute path obtain by merging path1 and path2
#***********************************************************************************************************************
function absolutePath([String]$path1, [String]$path2)
{
   [System.IO.Path]::GetFullPath([System.IO.Path]::Combine($path1, $path2))
}


#***********************************************************************************************************************
# useful variable declarations
#***********************************************************************************************************************
$solutionDir = absolutePath $PSScriptRoot "..\.."
$sslDir =  absolutePath $solutionDir "Vendor\OpenSSL"
$vcDllDir = absolutePath $solutionDir "Vendor\VCRuntime\Dlls"
$nsisInstallerPath = absolutePath $solutionDir "Installer\installer.nsi"
$portableAppsConfigPath = absolutePath $solutionDir "Installer\PortableApps.com\Template\BeeftextPortable\App\AppInfo\appinfo.ini"
$rcFilePath = absolutePath $solutionDir "Beeftext.rc"

#***********************************************************************************************************************
# Compiles a solution using Visual Studio
#***********************************************************************************************************************
function compileVisualStudioSolution([String]$solutionPath, [String]$configuration = "Release|x86")
{
   Invoke-Expression '& "$vsPath" "$solutionPath" /rebuild "$configuration"' | Out-Null
   if (0 -ne $LASTEXITCODE)
   {
       Write-Error "Compilation of the application failed."
   }
}


#***********************************************************************************************************************
# Copy the Qt DLLS required by Beeftext to the destination folder
#***********************************************************************************************************************
function copyQtDlls([String]$dstPath)
{
   $qtDlls = "Qt5Core.dll", "Qt5Gui.dll", "Qt5Multimedia.dll", "Qt5Network.dll", "Qt5Widgets.dll"
   foreach ($dll in $qtDlls) { Copy-Item -Path (absolutePath $env:QTDIR  "bin\$dll") -Destination $dstPath }
   $folders = "audio", "imageformats", "platforms", "styles"
   foreach ($folder in $folders) { New-Item -ItemType Directory -Force -Path (absolutePath $dstPath $folder) | Out-Null }
   $qtPlugins = "audio\qtaudio_windows.dll", "imageformats\qico.dll", "platforms\qwindows.dll", "styles\qwindowsvistastyle.dll"
   foreach ($dll in $qtPlugins) { Copy-Item -Path (absolutePath $env:QTDIR "plugins\$dll") -Destination (absolutePath $dstPath $dll) }    
}


#***********************************************************************************************************************
# Copy The SSL DLL files required by Beeftext ot the destination folder
#***********************************************************************************************************************
function copySslDlls([String]$dstPath)
{
   $sslDlls = "libeay32.dll", "ssleay32.dll"
   foreach ($dll in $sslDlls) { Copy-Item -Path (absolutePath $sslDir $dll) -Destination $dstPath }
}


#***********************************************************************************************************************
# Copy The Visual C++ runtime DLL files required by Beeftext ot the destination folder
#***********************************************************************************************************************
function copyVcppDlls([String]$dstPath)
{
   $vcDlls = "msvcp140.dll", "vcruntime140.dll"
   foreach ($dll in $vcDlls) { Copy-Item -Path (absolutePath $vcDllDir $dll) -Destination $dstPath }
}


#***********************************************************************************************************************
# Retrieve the major and minor version number of Beeftext from its source socde
#***********************************************************************************************************************
function getBeeftextVersion
{
   $srcFile = absolutePath $solutionDir "BeeftextConstants.cpp"
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
# Update the version number in the installer file
#***********************************************************************************************************************
function updateVersionNumberInPortableAppsConfiguration
{
    $major, $minor = getBeeftextVersion
    (Get-Content $portableAppsConfigPath) `
        -replace '(PackageVersion)=\d+.\d+.\d+.\d+', ('$1=' + "$major.$minor.0.0") `
        -replace '(DisplayVersion)=\d+.\d+', ('$1=' + "$major.$minor") |
        Out-File $portableAppsConfigPath -Encoding "UTF8"
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
