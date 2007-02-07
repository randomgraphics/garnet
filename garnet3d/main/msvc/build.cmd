@echo off
setlocal

del /s GN*.vcproj 2>nul

set xe=1
set vc80=1

for %%v in (debug profile retail stdbg stprof stret) do (

	REM ================================
	REM Generate VS projects for VS 2005
	REM ================================
	if "1" == "%vc80%" (
		for %%d in (x86 x64) do (
			echo call scons -U . variant=%%v compiler=vc80 os=mswin cpu=%%d -j1 --no-cache %*
			call scons -U . variant=%%v compiler=vc80 os=mswin cpu=%%d -j1 --no-cache %*
		)
	)

	REM =================================
	REM Generate VS projects for XBox 360
	REM =================================
	if "1" == "%xe%" (
		echo call scons -U . variant=%%v compiler=xenon os=xenon cpu=ppc -j1 --no-cache %*
		call scons -U . variant=%%v compiler=xenon os=xenon cpu=ppc -j1 --no-cache %*
	)
)
