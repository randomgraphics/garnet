@echo off
del GN*.vcproj 2>nul
del GN*.sln 2>nul
scons . variant=all
del GN*.sln 2>nul
