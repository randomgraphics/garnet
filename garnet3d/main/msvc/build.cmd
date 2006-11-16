@echo off
del /s GN*.vcproj 2>nul

for %%v in (debug profile retail stdbg stprof stret) do (

    REM ================================
    REM Generate VS projects for VS 2005 
    REM ================================
    for %%d in (x86 x64) do (
        echo call scons -U . variant=%%v compiler=vc80 os=mswin cpu=%%d -j1 --no-cache %*
        call scons -U . variant=%%v compiler=vc80 os=mswin cpu=%%d -j1 --no-cache %*
    )

    REM =================================
    REM Generate VS projects for XBox 360
    REM =================================
    echo call scons -U . variant=%%v compiler=xenon os=xenon cpu=ppc -j1 --no-cache %*
    call scons -U . variant=%%v compiler=xenon os=xenon cpu=ppc -j1 --no-cache %*
)
