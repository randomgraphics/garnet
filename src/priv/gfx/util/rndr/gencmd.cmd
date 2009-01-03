@echo off

set TARGET=mtrndrCmd.h

if not exist mtrndrCommandMeta.txt (
    echo ERROR: mtrndrCommandMeta.txt not found!
    goto :EOF
)


REM generate file header
echo.>>%TARGET%
echo //>>%TARGET%
echo // DO NOT EDIT. Generated by "gencmd.cmd">%TARGET%
echo //>>%TARGET%
echo.>>%TARGET%
echo namespace GN { namespace gfx>>%TARGET%
echo {>>%TARGET%
echo.>>%TARGET%

REM generate enum table
echo ///>>%TARGET%
echo /// Renderer command enumerations.>>%TARGET%
echo ///>>%TARGET%
echo enum RndrCommand {>>%TARGET%
for /F "tokens=1*" %%a in ( mtrndrCommandMeta.txt ) do echo CMD_%%a,%%b>>%TARGET%
echo };>>%TARGET%

REM generate end of file
echo }} // end of file>>%TARGET%
echo.>>%TARGET%
echo DONE!