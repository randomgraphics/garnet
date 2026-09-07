param([ValidateSet('debug', 'profile', 'release')][string]$Variant = 'debug')
$ErrorActionPreference = 'Stop'
Set-Location C:\garnet
& git.exe config --global --add safe.directory C:/garnet
if ($LASTEXITCODE -ne 0) { throw 'Git safe.directory setup failed' }
# The preinstalled venv and PIP_NO_INDEX keep environment setup offline.
. .\env\garnet.ps1
& python.exe -m pip check
if ($LASTEXITCODE -ne 0) { throw 'Python dependency check failed' }
& python.exe env\bin\build.py -b C:\garnet-build $Variant
if ($LASTEXITCODE -ne 0) { throw "$Variant build failed" }
