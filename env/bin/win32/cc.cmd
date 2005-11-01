@echo off

setlocal

REM ************************
REM code coverage automation
REM ************************

IF ""=="%1" (
	echo Code coverage automation.
	echo Usage: cc program [parameters]
	exit /b
)

REM
REM determin sql server
REM
if ""=="%MAGELLAN_SQL_SERVER%" (
	echo Environment "MAGELLAN_SQL_SERVER" not found, use default SQL server on msra-xe2m-amda
	set MAGELLAN_SQL_SERVER=msra-xe2m-amda
) else (
	echo Use SQL server on %MAGELLAN_SQL_SERVER%
)

echo.
echo Code injection...
echo.
echo bbcover /i %1 /db Server=%MAGELLAN_SQL_SERVER%;Database=garnet3d
bbcover /i %1 /db Server=%MAGELLAN_SQL_SERVER%;Database=garnet3d

echo.
echo Run the application...
echo.
echo %1.instr
%1.instr

echo.
echo Collect coverage data...
echo.
echo covercmd /save /db Server=%MAGELLAN_SQL_SERVER%;Database=garnet3d
covercmd /save /db Server=%MAGELLAN_SQL_SERVER%;Database=garnet3d
