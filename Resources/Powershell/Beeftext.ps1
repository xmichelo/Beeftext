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


#***********************************************************************************************************************
# Compiles a solution using Visual Studio
#***********************************************************************************************************************
function compileVisualStudioSolution([String]$solutionPath, [String]$configuration = "Release|x86")
{
   Invoke-Expression '& "$vsPath" "$solutionPath" /rebuild "$configuration"' | Out-Null
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

