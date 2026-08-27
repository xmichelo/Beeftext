# This script builds an MSIX package for Abbrevio, suitable for local sideload testing and,
# once AppxManifest.xml's Identity Name/Publisher are replaced with the values assigned by
# Partner Center (https://partner.microsoft.com/dashboard) for the reserved app name, for
# submission to the Microsoft Store.
#
# Copyright (c) . All rights reserved.
# Licensed under the MIT License. See LICENSE file in the project root for full license information.

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "..\..\Scripts\PowerShell\Abbrevio.ps1")

#***********************************************************************************************************************
# Variable definitions
#***********************************************************************************************************************
$major, $minor = getAbbrevioVersion
$msixVersion = "$major.$minor.0.0" # MSIX package versions must have exactly 4 numeric parts
$buildFolder = absolutePath $solutionDir "Installer\_build"
$layoutDir = absolutePath $buildFolder "AbbrevioMsixLayout"
$msixPath = absolutePath $buildFolder "Abbrevio-$major.$minor.msix"
$manifestSrcPath = absolutePath $PSScriptRoot "AppxManifest.xml"
$assetsSrcDir = absolutePath $PSScriptRoot "Assets"
$solutionPath = absolutePath $solutionDir "Abbrevio.sln"
$exeDir = absolutePath $solutionDir "_build\x64\Release"
$exePath = absolutePath $exeDir "Abbrevio.exe"


#***********************************************************************************************************************
# Locate a Windows SDK tool (makeappx.exe, signtool.exe, ...) by searching installed SDK versions, newest first.
#***********************************************************************************************************************
function findWindowsSdkTool([String]$toolName) {
    $sdkBinRoot = "${env:ProgramFiles(x86)}\Windows Kits\10\bin"
    if (-not (Test-Path $sdkBinRoot)) {
        Write-Error "Windows SDK not found at '$sdkBinRoot'. Install the Windows 10/11 SDK to build an MSIX package."
    }
    $versionDirs = Get-ChildItem -Path $sdkBinRoot -Directory | Where-Object { $_.Name -match '^\d+\.\d+\.\d+\.\d+$' } |
        Sort-Object { [Version]$_.Name } -Descending
    foreach ($versionDir in $versionDirs) {
        $candidate = Join-Path $versionDir.FullName "x64\$toolName"
        if (Test-Path $candidate) {
            return $candidate
        }
    }
    Write-Error "Could not find '$toolName' in any installed Windows SDK version under '$sdkBinRoot'."
}


#***********************************************************************************************************************
# Stage the application layout folder that makeappx.exe will pack into the MSIX package.
#***********************************************************************************************************************
function stageMsixLayout {
    "Cleaning old layout folder"
    if (Test-Path -PathType Container $layoutDir) { Remove-Item -Path $layoutDir -Recurse -Force }
    New-Item -ItemType Directory -Force -Path $layoutDir | Out-Null

    "Copying program files to the layout folder"
    Copy-Item -Path $exePath -Destination $layoutDir

    "Copying Qt DLL files"
    copyQtDlls $layoutDir

    "Copying SSL DLL files"
    copySslDlls $layoutDir

    "Copying Visual C++ DLL files"
    copyVcppDlls $layoutDir

    "Copying manifest and visual assets"
    Copy-Item -Path $manifestSrcPath -Destination (absolutePath $layoutDir "AppxManifest.xml")
    Copy-Item -Path $assetsSrcDir -Destination (absolutePath $layoutDir "Assets") -Recurse

    "Setting package version to $msixVersion in the staged manifest"
    $manifestPath = absolutePath $layoutDir "AppxManifest.xml"
    # Matches only a standalone Version="..." attribute (the Identity element's), not the <?xml version="1.0"?>
    # declaration's 2-part version (excluded by requiring 4 dotted numeric parts) and not MinVersion=/MaxVersionTested=
    # on TargetDeviceFamily, which also end in the literal substring "Version=" (excluded by the \b word-boundary
    # anchor, since e.g. "Min" runs directly into "Version" with no non-word separator between them). PowerShell's
    # -replace is case-insensitive by default, which is why this needs to be this precise.
    $manifestContent = (Get-Content $manifestPath -Raw) -replace '\bVersion="\d+\.\d+\.\d+\.\d+"', "Version=`"$msixVersion`""
    # Out-File -Encoding UTF8 writes a byte-order mark, which makeappx.exe's strict manifest validator rejects
    # ("Incorrect xml declaration syntax") since it precedes the required <?xml ...?> declaration. Write without a BOM.
    [System.IO.File]::WriteAllText($manifestPath, $manifestContent, (New-Object System.Text.UTF8Encoding $false))
}


#***********************************************************************************************************************
# The actual script
#***********************************************************************************************************************
function buildAbbrevioMsix {
    "Compiling Abbrevio"
    compileVisualStudioSolution $solutionPath

    stageMsixLayout

    "Locating makeappx.exe"
    $makeAppxPath = findWindowsSdkTool "makeappx.exe"

    "Packing MSIX package"
    if (Test-Path -PathType Leaf $msixPath) { Remove-Item -Path $msixPath -Force }
    & $makeAppxPath pack /d $layoutDir /p $msixPath /overwrite
    if (0 -ne $LASTEXITCODE) {
        Write-Error "makeappx.exe failed to pack the MSIX package."
    }

    "Removing layout folder"
    Remove-Item -Path $layoutDir -Recurse -Force

    "MSIX package created at $msixPath"
    "Note: this package is signed with neither a trusted certificate nor Partner Center's. To sideload it for local"
    "testing, sign it with a certificate whose subject matches AppxManifest.xml's Identity/Publisher value, e.g.:"
    "  New-SelfSignedCertificate -Type Custom -Subject '<Publisher value from the manifest>' -KeyUsage DigitalSignature ``"
    "      -FriendlyName 'Abbrevio dev cert' -CertStoreLocation Cert:\CurrentUser\My ``"
    "      -TextExtension @('2.5.29.37={text}1.3.6.1.5.5.7.3.3', '2.5.29.19={text}')"
    "  signtool sign /fd SHA256 /a /f <exported .pfx> /p <password> $msixPath"
    "Then install it via 'Add-AppxPackage -Path $msixPath' (after trusting the cert) or by double-clicking it."
}

buildAbbrevioMsix
