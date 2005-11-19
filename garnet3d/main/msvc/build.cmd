@echo off
del /s GN*.vcproj 2>nul
call scons -U . variant=all compiler="vc71 vc80 vc80-x64" -j1 --no-cache %*
