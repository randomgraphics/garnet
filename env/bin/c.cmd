@echo off
setlocal
mkdir %GARNET_ROOT%\%GN_BUILD_DIR%
if "%GN_BUILD_CMAKE_GENERATOR%" == "" (
    cmake -S "%GARNET_ROOT%" -B "%GARNET_ROOT%\%GN_BUILD_DIR%" -DCMAKE_BUILD_TYPE=Debug %*
) else (
    cmake -S "%GARNET_ROOT%" -B "%GARNET_ROOT%\%GN_BUILD_DIR%" -G %GN_BUILD_CMAKE_GENERATOR% -DCMAKE_BUILD_TYPE=Debug %* ..
)
