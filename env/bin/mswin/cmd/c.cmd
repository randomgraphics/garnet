@echo off
setlocal
mkdir %GARNET_ROOT%\%GN_BUILD_DIR%
cd %GARNET_ROOT%\%GN_BUILD_DIR%
if "%GN_BUILD_CMAKE_GENERATOR%" == "" (
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_GENERATOR_PLATFORM=%GN_BUILD_TARGET_CPU% %* ..\..
) else (
    cmake -G %GN_BUILD_CMAKE_GENERATOR% -DCMAKE_BUILD_TYPE=Debug %* ..\..
)
