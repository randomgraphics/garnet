@echo off
del GN*.vcproj 2>nul
del GN*.sln 2>nul
call scons -U . variant=all --no-cache
del GN*.sln 2>nul
