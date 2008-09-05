@echo off
goto :start

REM ============
REM sub routines
REM ============

:warn
echo.
echo ========
echo = WARN = %*
echo ========
echo.
goto :EOF

:usage
echo.
echo Usage: setupEnv.cmd [/h^|/?] [vc80^|icl^|mingw] [x86^|x64^|xenon] [debug^|profile^|retail^|stdbg^|stret^|stprof]
echo.
goto :EOF

:prepend_env
if not "" == "%~4" (
	echo %3=%~4
	set "%1=%~4;%~2"
) else (
	echo INFO: %3 not found.
)
goto :EOF

REM ====================
REM begin of main script
REM ====================
:start
pushd %~dp0..
set GARNET_ROOT=%cd%
popd

REM ============================
REM setup garnet build parameers
REM ============================

echo.
echo ==========================
echo Setup Garnet build variant
echo ==========================

:parse_cmdline
if not "" == "%1" (
		if "/?" == "%1" ( goto :usage
		) else if /I "/h" == "%1" ( goto :usage
		) else if /I "vc80" == "%1" ( set GN_BUILD_COMPILER=vc80
		) else if /I "icl" == "%1" ( set GN_BUILD_COMPILER=icl
		) else if /I "mingw" == "%1" ( set GN_BUILD_COMPILER=mingw
		) else if /I "x86" == "%1" ( set GN_BUILD_TARGET_CPU=x86
		) else if /I "x64" == "%1" ( set GN_BUILD_TARGET_CPU=x64
		) else if /I "debug" == "%1" ( set GN_BUILD_VARIANT=debug
		) else if /I "profile" == "%1" ( set GN_BUILD_VARIANT=profile
		) else if /I "retail" == "%1" ( set GN_BUILD_VARIANT=retail
		) else if /I "stdbg" == "%1" ( set GN_BUILD_VARIANT=stdbg
		) else if /I "stprof" == "%1" ( set GN_BUILD_VARIANT=stprof
		) else if /I "stret" == "%1" ( set GN_BUILD_VARIANT=stret
		) else if /I "xenon" == "%1" (
				set GN_BUILD_COMPILER=xenon
				set GN_BUILD_TARGET_OS=xenon
				set GN_BUILD_TARGET_CPU=ppc
				set GN_BUILD_VARIANT=stdbg
		) else (
				call :warn Unknown parameter "%1".
		)
		shift
		goto parse_cmdline
)

REM =======================
REM detect current CPU type
REM =======================
	   if /I "amd64" == "%PROCESSOR_ARCHITECTURE%" ( set GN_CURRENT_CPU=x64
) else if /I "amd64" == "%PROCESSOR_ARCHITEWOW64%" ( set GN_CURRENT_CPU=x64
) else if /I "ia64" == "%PROCESSOR_ARCHITECTURE%" ( set GN_CURRENT_CPU=ia64
) else if /I "x86" == "%PROCESSOR_ARCHITECTURE%" ( set GN_CURRENT_CPU=x86
) else (
	call :error Unknown CPU type!
	goto :EOF
)

REM ============================
REM setup garnet build parameers
REM ============================

if "" == "%GN_BUILD_COMPILER%" set GN_BUILD_COMPILER=vc80
if "" == "%GN_BUILD_TARGET_OS%" set GN_BUILD_TARGET_OS=mswin
if "AMD64" == "%PROCESSOR_ARCHITECTURE%" (
	if "" == "%GN_BUILD_TARGET_CPU%" set GN_BUILD_TARGET_CPU=x64
) else (
	if "" == "%GN_BUILD_TARGET_CPU%" set GN_BUILD_TARGET_CPU=x86
)
if "" == "%GN_BUILD_VARIANT%" set GN_BUILD_VARIANT=debug

echo GN_BUILD_COMPILER = %GN_BUILD_COMPILER%
echo GN_BUILD_TARGET_OS = %GN_BUILD_TARGET_OS%
echo GN_BUILD_TARGET_CPU = %GN_BUILD_TARGET_CPU%
echo GN_BUILD_VARIANT = %GN_BUILD_VARIANT%

REM =====================
REM setup VS environment
REM =====================
if /I "vc80" == "%GN_BUILD_COMPILER%" (
	echo.
	echo =====================================
	echo Setup Visual Studio build environment
	echo =====================================
	if not "" == "%VS90COMNTOOLS%" (
		set "VS_ROOT=%VS90COMNTOOLS%..\.."
		set "VS_SETENV=%VS90COMNTOOLS%..\..\VC\vcvarsall.bat"
	) else if not "" == "%VS80COMNTOOLS%" (
		set "VS_ROOT=%VS80COMNTOOLS%..\.."
		set "VS_SETENV=%VS80COMNTOOLS%..\..\VC\vcvarsall.bat"
	) else if not "" == "%VSINSTALLDIR%" (
		set "VS_ROOT=%VSINSTALLDIR%"
		set "VS_SETENV=%VSINSTALLDIR%\VC\vcvarsall.bat"
	) else (
		call :warn None of VSINSTALLDIR, VS80COMNTOOLS, VS90COMNTOOLS is found. Please install Visual Studio 2005+.
	)
)

if not "" == "%VS_SETENV%" (
	if /I "x86" == "%GN_BUILD_TARGET_CPU%" (
		call "%VS_SETENV%" x86
	) else if /I "x64" == "%GN_BUILD_TARGET_CPU%" (
		if /I "x64" == "%GN_CURRENT_CPU%" (
			call "%VS_SETENV%" amd64
		) else (
			call "%VS_SETENV%" x86_amd64
		)
	) else if /I "ia64" == "%GN_BUILD_TARGET_CPU%" (
		if /I "ia64" == "%GN_CURRENT_CPU%" (
			call "%VS_SETENV%" ia64
		) else (
			call "%VS_SETENV%" x86_ia64
		)
	) else (
		call :error Unsupport target CPU type: %GN_BUILD_TARGET_CPU%.
	)

	set VS8_ROOT=
	set VS_SETENV=
)

REM =========================
REM setup directx environment
REM =========================

echo.
echo =============================
echo Setup DirectX SDK environment
echo =============================
if not "" == "%DXSDK_DIR%" (
	echo Using DirectX SDK at %DXSDK_DIR%
	set "DXSDK_SETENV=%DXSDK_DIR%Utilities\Bin\dx_setenv.cmd"
) else (
	call :warn Environment variable DXSDK_DIR not found. Please install DirectX SDK.
)

if not "" == "%DXSDK_SETENV%" (
	if /I "x86" == "%GN_BUILD_TARGET_CPU%" (
		call "%DXSDK_SETENV%" x86
	) else if /I "x64" == "%GN_BUILD_TARGET_CPU%" (
		call "%DXSDK_SETENV%" amd64
	)
	set DXSDK_SETENV=
)

REM =========================
REM setup GREEN environment
REM =========================

if not "" == "%GREEN_PATH%" (
	echo.
	echo =================
	echo Setup GREEN tools
	echo =================
	if /I "x86" == "%GN_BUILD_TARGET_CPU%" (
		call :prepend_env INCLUDE "%INCLUDE%" GREEN_INC_X86 "%GREEN_INC_X86%"
		call :prepend_env LIB "%LIB%" GREEN_LIB_X86 "%GREEN_LIB_X86%"
	) else if /I "x64" == "%GN_BUILD_TARGET_CPU%" (
		call :prepend_env INCLUDE "%INCLUDE%" GREEN_INC_X64 "%GREEN_INC_X64%"
		call :prepend_env LIB "%LIB%" GREEN_LIB_X64 "%GREEN_LIB_X64%"
	)
)

REM =======================
REM setup xenon environment
REM =======================
if "xenon" == "%GN_BUILD_COMPILER%" (
	echo.
	echo =============================
	echo Setup Xenon build environment
	echo =============================
	if "" == "%XEDK%" (
		call :warn Environment variable XEDK not found.
	) else (
		echo Using XDK at "%XEDK%"
		pushd .
		call "%XEDK%\bin\win32\xdkvars.bat"
		popd
	)
)

REM =================
REM setup custom path
REM =================
if "AMD64" == "%PROCESSOR_ARCHITECTURE%" (
	set "mypath=%GARNET_ROOT%\env\bin\mswin\x64;%GARNET_ROOT%\env\bin\mswin\x86"
) else (
	set "mypath=%GARNET_ROOT%\env\bin\mswin\x86"
)
set "PATH=%mypath%;%PATH%"
set mypath=

REM ===========
REM setup scons
REM ===========
echo.
echo =============================
echo Setup SCons build environment
echo =============================
echo SCons Directory : %GARNET_ROOT%\env\scons
set PATH=%GARNET_ROOT%\env\scons\Scripts;%PATH%
set SCONS_LIB_DIR=%GARNET_ROOT%\env\scons\Lib

REM ===========
REM setup alias
REM ===========

if exist "%GARNET_ROOT%\env\alias.txt" (
	for /F "tokens=1*" %%i in (%GARNET_ROOT%\env\alias.txt) do alias %%i %%j
) else (
	call :warning "%GARNET_ROOT%\env\alias.txt" is missing.
)

REM ====================
REM Update console title
REM ====================
title garnet3d ( %GARNET_ROOT% %GN_BUILD_COMPILER%-%GN_BUILD_TARGET_OS%-%GN_BUILD_TARGET_CPU% %GN_BUILD_VARIANT% )

REM ====================
REM MISC.
REM ====================
set SCONSFLAGS=-U
title garnet3d ( %GARNET_ROOT% %GN_BUILD_COMPILER%-%GN_BUILD_TARGET_OS%-%GN_BUILD_TARGET_CPU% %GN_BUILD_VARIANT% )


REM ===================
REM End of common setup
REM ===================

echo Garnet root : %GARNET_ROOT%
cd /d %GARNET_ROOT%


REM =========================
REM Call user specific script
REM =========================

echo User name: %USERNAME%
if exist env\user\%USERNAME%.cmd call env\user\%USERNAME%.cmd
