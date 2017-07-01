@REM customized SVN command
@echo off

if "" == "%*" (
    svn.exe
) else (
    svn.exe --config-dir="%~dp0..\svncfg" %*
)
