@echo off

if defined DevEnvDir (
    goto EOF
)

set BUILD_ARCH=x64

call "%~dp0\DetectVS2019.cmd"
if %ERRORLEVEL% EQU 0 goto SUCCESS

call "%~dp0\DetectVS2022.cmd"
if %ERRORLEVEL% EQU 0 goto SUCCESS

:FAIL
echo Visual Studio Environment detection failed
exit /b 1

:SUCCESS
echo Visual Studio Environment has been detected, detected VS %BUILD_VS_YEAR%

:EOF
exit /b 0
