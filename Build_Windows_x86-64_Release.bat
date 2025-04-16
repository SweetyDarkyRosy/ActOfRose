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

mkdir "Bin\Intermediates\Win_x86-64\Release"

for /R "SrcCode" %%f in ("*.cpp") do (
    echo %%~nxf | findstr /i "_Linux" >nul
    if errorlevel 1 (
        cl.exe /c "%%f" /I"SrcCode" /Fo"Bin\Intermediates\Win_x86-64\Release\%%~nf.obj" /Fd"Bin\Intermediates\Win_x86-64\Release\%%~nf.pdb" /W3 /O2 /std:c++17 /Zi /MT /DNDEBUG /D_NDEBUG
    )
)

link.exe /DEBUG:FULL /MACHINE:X64 "Bin\Intermediates\Win_x86-64\Release\*.obj" /OUT:"Bin\ActOfRose.exe"
