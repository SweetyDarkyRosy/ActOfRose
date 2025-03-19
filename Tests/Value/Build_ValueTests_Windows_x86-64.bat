@echo off

call "%~dp0\..\..\Build\Windows\SetVSDevEnv_x86-64.cmd"
if %ERRORLEVEL% EQU 0 goto COMPILE_AND_BUILD

:FAIL
echo Visual Studio Environment detection failed.
exit /b 1

:COMPILE_AND_BUILD

cl.exe /c "..\..\SrcCode\Utility\StringConverting.cpp" /Fo"StringConverting.obj" /I"..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG
cl.exe /c "..\..\SrcCode\Value\CIntegerValue.cpp" /Fo"CIntegerValue.obj" /I"..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG
cl.exe /c "..\..\SrcCode\Value\CFloatValue.cpp" /Fo"CFloatValue.obj" /I"..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG
cl.exe /c "..\..\SrcCode\Value\CBooleanValue.cpp" /Fo"CBooleanValue.obj" /I"..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG
cl.exe /c "..\..\SrcCode\Value\CCharValue.cpp" /Fo"CCharValue.obj" /I"..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG
cl.exe /c ValueTests.cpp /Fo"ValueTests.obj" /I"..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG

link.exe /DEBUG:FULL /MACHINE:X64 "*.obj" /OUT:"ValueTestsExec.exe"

del *.obj
