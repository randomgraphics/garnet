@echo off
setlocal
if "" == "%2" (
    set FILES=*.*
) else (
    set FILES=%2
)
%~dp0myfindstr.exe /s /i /l %1 %FILES%