@echo off
if exist %GARNET_ROOT%\%GN_BUILD_DIR% (
    pushd %GARNET_ROOT%\%GN_BUILD_DIR%
    cmake --build . %*
    popd
) else (
    echo ERROR: %GARNET_ROOT%\%GN_BUILD_DIR% not found. Run c.cmd first to create build folder.
)
