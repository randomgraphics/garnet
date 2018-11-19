@echo off
if exist %GARNET_ROOT%\%GN_BUILD_DIR% (
    cd %GARNET_ROOT%\%GN_BUILD_DIR%
    cmake --build . %*
) else (
    echo ERROR: %GARNET_ROOT%\%GN_BUILD_DIR% not found. Run c.cmd first to create build folder.
)
