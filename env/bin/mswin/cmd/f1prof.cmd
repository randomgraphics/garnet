@echo off

if "" == "%1" (
	echo F1 [app_basename]
	exit /b
)

mkdir c:\garnet_perf\%1 2>nul

vsperfcmd -start:sample -output:c:\garnet_perf\%1\%1
vsperfcmd -attach:%1.exe -timer:1000000
pause
vsperfcmd -detach
vsperfcmd -shutdown
cd /d c:\garnet_perf\%1
vsperfreport /summary:all %1.vsp
