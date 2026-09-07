param([Parameter(Mandatory = $true)][string]$VulkanVersion)
$ErrorActionPreference = 'Stop'
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

function Install-Executable([string]$Url, [string]$Name, [string[]]$Options) {
    Write-Host "Installing $Name from $Url"
    $path = Join-Path $env:TEMP $Name
    Invoke-WebRequest -UseBasicParsing -Uri $Url -OutFile $path
    $process = Start-Process -FilePath $path -ArgumentList $Options -WindowStyle Hidden -Wait -PassThru
    # Installers can report success with a reboot request; containers cannot reboot.
    if ($process.ExitCode -notin @(0, 3010)) { throw "$Name failed: $($process.ExitCode)" }
    Remove-Item $path -Force
}

# Recommended workload components track the Windows SDK supported by the stable toolchain.
Install-Executable 'https://aka.ms/vs/18/stable/vs_buildtools.exe' 'vs-buildtools.exe' @(
    '--quiet', '--wait', '--norestart', '--nocache', '--installPath', 'C:\BuildTools',
    '--add', 'Microsoft.VisualStudio.Workload.VCTools',
    '--add', 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64',
    '--includeRecommended'
)
Install-Executable 'https://github.com/git-for-windows/git/releases/download/v2.55.0.windows.5/Git-2.55.0.5-64-bit.exe' 'git.exe' @(
    '/VERYSILENT', '/NORESTART', '/NOCANCEL', '/SP-', '/DIR=C:\Git'
)
Install-Executable 'https://www.python.org/ftp/python/3.14.7/python-3.14.7-amd64.exe' 'python.exe' @(
    '/quiet', 'InstallAllUsers=1', 'TargetDir=C:\Python', 'PrependPath=0', 'Include_test=0'
)
Invoke-WebRequest -UseBasicParsing -Uri 'https://github.com/Kitware/CMake/releases/download/v4.4.3/cmake-4.4.3-windows-x86_64.zip' -OutFile C:\image\cmake.zip
Expand-Archive C:\image\cmake.zip C:\Tools
# Use the official SDK so Windows image updates do not depend on a repackaged release.
Install-Executable "https://sdk.lunarg.com/sdk/download/$VulkanVersion/windows/vulkan_sdk.exe" 'vulkan-sdk.exe' @(
    '--root', 'C:\VulkanSDK\current', '--accept-licenses', '--default-answer', '--confirm-command', 'install'
)

$env:Path = 'C:\Python;C:\Python\Scripts;C:\Git\cmd;C:\Tools\cmake-4.4.3-windows-x86_64\bin;C:\VulkanSDK\current\Bin;' + $env:Path
[Environment]::SetEnvironmentVariable('Path', $env:Path, 'Machine')
& python.exe -m venv C:\garnet-venv
if ($LASTEXITCODE -ne 0) { throw 'Python venv creation failed' }
& C:\garnet-venv\Scripts\python.exe -m pip install --upgrade pip
if ($LASTEXITCODE -ne 0) { throw 'pip upgrade failed' }
# New Python releases may need native packages built from source (notably OpenEXR).
$env:CMAKE_GENERATOR = 'Visual Studio 18 2026'
$env:CMAKE_GENERATOR_PLATFORM = 'x64'
& cmd.exe /d /s /c '"C:\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64 && C:\garnet-venv\Scripts\python.exe -m pip install --upgrade -r C:\image\requirements.txt'
if ($LASTEXITCODE -ne 0) { throw 'Python dependency installation failed' }
& C:\garnet-venv\Scripts\python.exe -m pip check
if ($LASTEXITCODE -ne 0) { throw 'Python dependency consistency check failed' }
& C:\garnet-venv\Scripts\python.exe -m pip freeze | Set-Content C:\image\python-packages.txt
if ($LASTEXITCODE -ne 0) { throw 'Python package manifest failed' }
& C:\garnet-venv\Scripts\python.exe -m black --version
if ($LASTEXITCODE -ne 0) { throw 'Black is missing' }
& C:\garnet-venv\Scripts\python.exe -c "import ctypes; ctypes.WinDLL('opengl32.dll')"
if ($LASTEXITCODE -ne 0) { throw 'OpenGL runtime is missing; use the full Windows Server base image' }
& python.exe --version
if ($LASTEXITCODE -ne 0) { throw 'Python is missing' }
& git.exe --version
if ($LASTEXITCODE -ne 0) { throw 'Git is missing' }
& git.exe lfs version
if ($LASTEXITCODE -ne 0) { throw 'Git LFS is missing' }
& cmake.exe --version
if ($LASTEXITCODE -ne 0) { throw 'CMake is missing' }
& C:\VulkanSDK\current\Bin\glslc.exe --version
if ($LASTEXITCODE -ne 0) { throw 'Vulkan shader compiler is missing' }
if (-not (Test-Path C:\BuildTools\Common7\Tools\VsDevCmd.bat)) { throw 'MSVC installation is missing' }
Remove-Item C:\image\*.zip -Force
