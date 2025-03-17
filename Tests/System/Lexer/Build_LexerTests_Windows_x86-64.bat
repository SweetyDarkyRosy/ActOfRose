@echo off

call "%~dp0\..\..\..\Build\Windows\SetVSDevEnv_x86-64.cmd"
if %ERRORLEVEL% EQU 0 goto COMPILE_AND_BUILD

:FAIL
echo Visual Studio Environment detection failed.
exit /b 1

:COMPILE_AND_BUILD

cl.exe /c "..\..\..\SrcCode\Utility\StringConverting.cpp" /Fo"StringConverting.obj" /I"..\..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG
cl.exe /c "..\..\..\SrcCode\CLexer.cpp" /Fo"CLexer.obj" /I"..\..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG
cl.exe /c "..\..\..\SrcCode\Log.cpp" /Fo"Log.obj" /I"..\..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG
cl.exe /c "..\..\..\SrcCode\Keywords.cpp" /Fo"Keywords.obj" /I"..\..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG
cl.exe /c LexerTests.cpp /Fo"LexerTests.obj" /I"..\..\..\SrcCode" /W3 /Od /std:c++17 /MTd /DDEBUG /D_DEBUG

link.exe /DEBUG:FULL /MACHINE:X64 "*.obj" /OUT:"LexerTestsExec.exe"

del *.obj
