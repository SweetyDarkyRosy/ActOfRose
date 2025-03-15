@echo off

call "%~dp0\Build\Windows\SetVSDevEnv_x86-64.cmd"
if %ERRORLEVEL% EQU 0 goto COMPILE_AND_BUILD

:FAIL
echo Visual Studio Environment detection failed.
exit /b 1

:COMPILE_AND_BUILD
if exist "Bin" (
    rd /s /q "Bin"
)

mkdir "Bin\Intermediates\Win_x86-64\Debug"

for /R "SrcCode" %%f in ("*.cpp") do (
    cl.exe /c /I"SrcCode" "%%f" /Fo"Bin\Intermediates\Win_x86-64\Debug\%%~nf.obj" /Fd"Bin\Intermediates\Win_x86-64\Debug\%%~nf.pdb" /W3 /Od /std:c++17 /Zi /MTd /DDEBUG /D_DEBUG
)

link.exe /DEBUG:FULL /MACHINE:X64 "Bin\Intermediates\Win_x86-64\Debug\*.obj" /OUT:"Bin\ActOfRose.exe"