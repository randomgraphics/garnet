@echo off
del GN*.vcproj 2>nul
del GN*.sln 2>nul
scons . variant=all --no-cache
del GN*.sln 2>nul
