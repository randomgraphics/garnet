@echo off

pushd %~dp0..
set GARNET_ROOT=%cd%
popd

REM ==========
REM setup path
REM ==========
if "AMD64" == "%PROCESSOR_ARCHITECTURE%" (
    set mypath=%GARNET_ROOT%\env\bin\x64;%GARNET_ROOT%\env\bin\win32;%GARNET_ROOT%\env\bin\common
) else (
    set mypath=%GARNET_ROOT%\env\bin\win32;%GARNET_ROOT%\env\bin\common
)
set PATH=%mypath%;%PATH%
set mypath=


REM ===========
REM setup scons
REM ===========
echo SCons Directory : %GARNET_ROOT%\env\scons
set PATH=%GARNET_ROOT%\env\scons\Scripts;%PATH%
set SCONS_LIB_DIR=%GARNET_ROOT%\env\scons\Lib

REM ===========
REM setup alias
REM ===========

alias root "cd /d %GARNET_ROOT%\$*"
alias src "cd /d %GARNET_ROOT%\src\$*"
alias priv "cd /d %GARNET_ROOT%\src\priv\$*"
alias base "cd /d %GARNET_ROOT%\src\priv\base\$*"
alias core "cd /d %GARNET_ROOT%\src\priv\core\$*"
alias gfx "cd /d %GARNET_ROOT%\src\priv\gfx\$*"
alias test "cd /d %GARNET_ROOT%\src\priv\test\$*"
alias sample "cd /d %GARNET_ROOT%\src\priv\sample\$*"
alias doc "cd /d %GARNET_ROOT%\src\priv\doc$*"
alias man "cd /d %GARNET_ROOT%\src\priv\manual$*"
alias misc "cd /d %GARNET_ROOT%\src\priv\misc$*"
alias inc "cd /d %GARNET_ROOT%\src\priv\inc\garnet\$*"
alias extern "cd /d %GARNET_ROOT%\src\extern\$*"
alias media "cd /d %GARNET_ROOT%\src\media\$*"
alias bin "cd /d %GARNET_ROOT%\bin"
alias msvc "cd /d %GARNET_ROOT%\msvc"
alias env "cd /d %GARNET_ROOT%\env\$*"

REM =====
REM Misc.
REM =====

set INCLUDE=
set LIBS=

REM ============
REM End of setup
REM ============

echo Garnet root : %GARNET_ROOT%
cd /d %GARNET_ROOT%

REM =========================
REM Call user specific script
REM =========================

echo User name: %USERNAME%
if exist env\user\%USERNAME%.cmd call env\user\%USERNAME%.cmd
