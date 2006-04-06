@echo off
del /s GN*.vcproj 2>nul
for %%c in (vc80 vc80-x64 xenon) do (
    echo call scons -U . variant=all compiler=%%c -j1 --no-cache %*
    call scons -U . variant=all compiler=%%c -j1 --no-cache %*
)
