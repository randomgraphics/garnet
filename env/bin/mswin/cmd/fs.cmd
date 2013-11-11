@echo off
setlocal
if "" == "%2" (
    set FILES=*.*
) else (
    set FILES=%2
)
%~dp0..\x86\myfindstr.exe /s /i /l %1 %FILES%