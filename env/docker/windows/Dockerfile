# escape=`
# Full Windows Server supplies OpenGL runtime DLLs and supports DirectX GPU device sharing.
FROM mcr.microsoft.com/windows/server:ltsc2022
SHELL ["powershell.exe", "-NoLogo", "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command"]

ARG VULKAN_VERSION=1.4.357.0
COPY install.ps1 requirements.txt C:/image/
RUN $ErrorActionPreference = 'Stop'; C:\image\install.ps1 -VulkanVersion $env:VULKAN_VERSION

# Git for Windows bundles an older LFS release; replace that executable explicitly.
RUN $ErrorActionPreference = 'Stop'; `
    Invoke-WebRequest -UseBasicParsing -Uri 'https://github.com/git-lfs/git-lfs/releases/download/v3.8.0/git-lfs-windows-amd64-v3.8.0.zip' -OutFile C:\image\git-lfs.zip; `
    Expand-Archive C:\image\git-lfs.zip C:\image\git-lfs; `
    $lfs = @(Get-ChildItem C:\image\git-lfs -Recurse -Filter git-lfs.exe); `
    if ($lfs.Count -ne 1) { throw 'Unexpected Git LFS archive layout' }; `
    Copy-Item $lfs[0].FullName C:\Git\mingw64\bin\git-lfs.exe -Force; `
    git lfs version; if ($LASTEXITCODE -ne 0) { throw 'Git LFS check failed' }; `
    Remove-Item -LiteralPath C:\image\git-lfs -Recurse -Force; `
    Remove-Item -LiteralPath C:\image\git-lfs.zip -Force

ENV VULKAN_SDK="C:\VulkanSDK\current" `
    GARNET_PYVENV="C:\garnet-venv" `
    CMAKE_GENERATOR="Visual Studio 18 2026" `
    CMAKE_GENERATOR_PLATFORM="x64" `
    PIP_NO_INDEX="1" `
    PIP_DISABLE_PIP_VERSION_CHECK="1"
COPY entrypoint.cmd C:/image/entrypoint.cmd
WORKDIR C:/garnet
ENTRYPOINT ["C:\\image\\entrypoint.cmd"]
CMD ["powershell.exe", "-NoLogo", "-ExecutionPolicy", "Bypass"]
