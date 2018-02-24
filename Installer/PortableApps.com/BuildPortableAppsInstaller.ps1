$scriptPath = $PSScriptRoot;
$solutionDir = [System.IO.Path]::GetFullPath((Join-Path $scriptPath "..\.."))
$dstParentFolder = [System.IO.Path]::GetFullPath((Join-Path $solutionDir "_build\PortableApps.com"))
$dstPath = [System.IO.Path]::GetFullPath((Join-Path $dstParentFolder "BeeftextPortable"))
$dstBinPath = [System.IO.Path]::GetFullPath((Join-Path $dstPath "App\Beeftext"))
$dstDataDir = [System.IO.Path]::GetFullPath((Join-Path $dstPath "Data"))
$solutionFile = [System.IO.Path]::GetFullPath((Join-Path $solutionDir "Beeftext.sln"))
$templatePath = [System.IO.Path]::GetFullPath((Join-Path $scriptPath "Template\BeeftextPortable"))
$sslDir = [System.IO.Path]::GetFullPath((Join-Path $solutionDir "Vendor\OpenSSL"))
$vcDllDir = [System.IO.Path]::GetFullPath((Join-Path $solutionDir "Vendor\VCRuntime\Dlls"))
$exePath = [System.IO.Path]::GetFullPath((Join-Path $solutionDir "_build\Win32\Release\Beeftext.exe"))
$vsPath = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.com"
$installerGeneratorPath = "C:\Program Files (x86)\PortableApps.comInstaller\PortableApps.comInstaller.exe"
$beaconFileName = "PortableApps.bin"

"Compiling Beeftext"
Invoke-Expression '& "$vsPath" "$solutionFile" /rebuild "Release|x86"' | Out-Null

"Cleaning destination folder"
if (Test-Path -PathType Container $dstParentFolder) { Remove-Item -Path $dstParentFolder -Recurse -Force }
New-Item -ItemType Directory -Force -Path $dstParentFolder | Out-Null

"Copying template to the destination folder"
Copy-Item -Path $templatePath -Destination $dstParentFolder -Recurse -Force

"Deleting Data folder"
if (Test-Path -PathType Container $dstDataDir) { Remove-Item -Path $dstDataDir -Recurse -Force }

"Copying program files to the destination folder"
Copy-Item -Path $exePath -Destination $dstBinPath

"Copying Qt DLL files"
$qtDlls = "Qt5Core.dll", "Qt5Gui.dll", "Qt5Multimedia.dll", "Qt5Network.dll", "Qt5Widgets.dll"
foreach ($dll in $qtDlls) { Copy-Item -Path ([System.IO.Path]::GetFullPath((Join-Path $env:QTDIR "bin\$dll"))) -Destination $dstBinPath }
$folders = "audio", "imageformats", "platforms", "styles"
foreach ($folder in $folders) { New-Item -ItemType Directory -Force -Path ([System.IO.Path]::GetFullPath((Join-Path $dstBinPath $folder))) | Out-Null}
$qtPlugins = "audio\qtaudio_windows.dll", "imageformats\qico.dll", "platforms\qwindows.dll", "styles\qwindowsvistastyle.dll"
foreach ($dll in $qtPlugins) { Copy-Item -Path ([System.IO.Path]::GetFullPath((Join-Path $env:QTDIR "plugins\$dll"))) -Destination ([System.IO.Path]::GetFullPath((Join-Path $dstBinPath $dll))) }

"Copying README.md file"
Copy-Item -Path ([System.IO.Path]::GetFullPath((Join-Path $solutionDir "README.md"))) -Destination $dstBinPath

"Copy SSL DLL files"
$sslDlls = "libeay32.dll", "ssleay32.dll"
foreach ($dll in $sslDlls) { Copy-Item -Path ([System.IO.Path]::GetFullPath((Join-Path $sslDir $dll))) -Destination $dstBinPath }

"Copy Visual C++ DLL files"
$vcDlls = "msvcp140.dll", "vcruntime140.dll"
foreach ($dll in $vcDlls) { Copy-Item -Path ([System.IO.Path]::GetFullPath((Join-Path $vcDllDir $dll))) -Destination $dstBinPath }

"Creating PortablesApps.com beacon file"
"Do not delete this file" | Set-Content -Path ([System.IO.Path]::GetFullPath((Join-Path $dstBinPath $beaconFileName)))

"Generating installer executable"
Invoke-Expression '& "$installerGeneratorPath" "$dstPath"' | Out-Null

