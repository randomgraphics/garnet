# escape=`
FROM mcr.microsoft.com/dotnet/framework/runtime:4.8-windowsservercore-ltsc2022
SHELL ["powershell.exe", "-NoLogo", "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command"]

ARG VULKAN_VERSION
COPY install.ps1 requirements.txt C:/image/
RUN $ErrorActionPreference = 'Stop'; C:\image\install.ps1 -VulkanVersion $env:VULKAN_VERSION

ENV VULKAN_SDK="C:\VulkanSDK\vulkan-sdk-for-windows" `
    GARNET_PYVENV="C:\garnet-venv" `
    CMAKE_GENERATOR="Visual Studio 17 2022" `
    CMAKE_GENERATOR_PLATFORM="x64" `
    PIP_NO_INDEX="1" `
    PIP_DISABLE_PIP_VERSION_CHECK="1"
COPY entrypoint.cmd C:/image/entrypoint.cmd
WORKDIR C:/garnet
ENTRYPOINT ["C:\\image\\entrypoint.cmd"]
CMD ["powershell.exe", "-NoLogo", "-ExecutionPolicy", "Bypass"]
