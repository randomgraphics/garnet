@echo off
call C:\BuildTools\Common7\Tools\VsDevCmd.bat -arch=amd64 -host_arch=amd64
if errorlevel 1 exit /b %errorlevel%
set "PATH=C:\garnet-venv\Scripts;%PATH%"
%*
exit /b %errorlevel%
