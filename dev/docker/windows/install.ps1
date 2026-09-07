param([Parameter(Mandatory = $true)][string]$VulkanVersion)
$ErrorActionPreference = 'Stop'
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

function Install-Executable([string]$Url, [string]$Name, [string[]]$Options) {
    $path = Join-Path $env:TEMP $Name
    Invoke-WebRequest -UseBasicParsing -Uri $Url -OutFile $path
    $process = Start-Process -FilePath $path -ArgumentList $Options -Wait -PassThru
    # Installers can report success with a reboot request; containers cannot reboot.
    if ($process.ExitCode -notin @(0, 3010)) { throw "$Name failed: $($process.ExitCode)" }
    Remove-Item $path -Force
}

Install-Executable 'https://aka.ms/vs/17/release/vs_buildtools.exe' 'vs-buildtools.exe' @(
    '--quiet', '--wait', '--norestart', '--nocache', '--installPath', 'C:\BuildTools',
    '--add', 'Microsoft.VisualStudio.Workload.VCTools',
    '--add', 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64',
    '--add', 'Microsoft.VisualStudio.Component.Windows11SDK.22621'
)
Install-Executable 'https://github.com/git-for-windows/git/releases/download/v2.49.0.windows.1/Git-2.49.0-64-bit.exe' 'git.exe' @(
    '/VERYSILENT', '/NORESTART', '/NOCANCEL', '/SP-', '/DIR=C:\Git'
)
Install-Executable 'https://www.python.org/ftp/python/3.12.10/python-3.12.10-amd64.exe' 'python.exe' @(
    '/quiet', 'InstallAllUsers=1', 'TargetDir=C:\Python', 'PrependPath=0', 'Include_test=0'
)
Invoke-WebRequest -UseBasicParsing -Uri 'https://github.com/Kitware/CMake/releases/download/v3.31.6/cmake-3.31.6-windows-x86_64.zip' -OutFile C:\image\cmake.zip
Expand-Archive C:\image\cmake.zip C:\Tools
Invoke-WebRequest -UseBasicParsing -Uri "https://github.com/randomgraphics/vulkan-sdk-for-windows/releases/download/v$VulkanVersion/vulkan-sdk-for-windows-$VulkanVersion.zip" -OutFile C:\image\vulkan.zip
Expand-Archive C:\image\vulkan.zip C:\VulkanSDK

$env:Path = 'C:\Python;C:\Python\Scripts;C:\Git\cmd;C:\Tools\cmake-3.31.6-windows-x86_64\bin;C:\VulkanSDK\vulkan-sdk-for-windows\Bin;' + $env:Path
[Environment]::SetEnvironmentVariable('Path', $env:Path, 'Machine')
& python.exe -m venv C:\garnet-venv
if ($LASTEXITCODE -ne 0) { throw 'Python venv creation failed' }
& C:\garnet-venv\Scripts\python.exe -m pip install -r C:\image\requirements.txt
if ($LASTEXITCODE -ne 0) { throw 'Python dependency installation failed' }
& git.exe lfs version
if ($LASTEXITCODE -ne 0) { throw 'Git LFS is missing' }
& cmake.exe --version
if ($LASTEXITCODE -ne 0) { throw 'CMake is missing' }
& C:\VulkanSDK\vulkan-sdk-for-windows\Bin\glslc.exe --version
if ($LASTEXITCODE -ne 0) { throw 'Vulkan shader compiler is missing' }
if (-not (Test-Path C:\BuildTools\Common7\Tools\VsDevCmd.bat)) { throw 'MSVC installation is missing' }
Remove-Item C:\image\*.zip -Force
