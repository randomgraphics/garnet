@echo off

if "" == "%1" (
	echo F1 [app_basename]
	exit /b
)

mkdir c:\garnet_perf\%1 2>nul

vsperfcmd -start:sample -output:c:\garnet_perf\%1\%1
vsperfcmd -attach:%1.exe -timer:1000000
pause
vsperfcmd -shutdown
cd /d c:\garnet_perf\%1
if "" == "%_NT_SYMBOL_PATH%" (
	vsperfreport /summary:all %1.vsp
) else (
	vsperfreport /summary:all /SYMBOLPATH:"%_NT_SYMBOL_PATH%" %1.vsp
)
