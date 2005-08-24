@echo off

REM ************************
REM code coverage automation
REM ************************

IF ""=="%1" (
	echo Code coverage automation.
	echo Usage: cc program [parameters]
	exit /b
)

REM code injection
bbcover /i %1 /db Server=msra-chenli;Database=garnet3d

REM run application
%1.instr

REM coverage collection
covercmd /save /db Server=msra-chenli;Database=garnet3d
