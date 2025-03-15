/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of utility set source code.
	
	@Name:			Log.cpp
	@Created:		12.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Log.h"

#if defined (WIN32) || defined (_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include "ErrorCodes.h"
#include "Utility/StringConverting.h"


// Writes a given null-terminated UTF-8-encoded message as a log with the specified log level
void ActOfRose::WriteLog(const char* utf8Msg, std::size_t msgLength, ActOfRose::ELogLevel level)
{
#if defined (WIN32) || defined (_WIN32)
	std::wstring utf16BEStr;

	if (ConvertStringUTF8ToUTF16BE(&utf16BEStr, utf8Msg, msgLength) == AOR_ERROR_SUCCESS)
	{
		WriteLog(utf16BEStr.c_str(), utf16BEStr.length(), level);
	}
#endif
}

// Writes a given null-terminated UTF-16BE-encoded message as a log with the specified log level
void ActOfRose::WriteLog(const wchar_t* utf16BEMsg, std::size_t msgLength, ActOfRose::ELogLevel level)
{
#if defined (WIN32) || defined (_WIN32)
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (level)
	{
		case ActOfRose::ELogLevel::ELL_Debug:
		{
			WriteConsoleW(consoleHandle, L"- DEBUG: ", 9, NULL, NULL);
			break;
		}

		case ActOfRose::ELogLevel::ELL_Warning:
		{
			WriteConsoleW(consoleHandle, L"- WARNING: ", 11, NULL, NULL);
			break;
		}

		case ActOfRose::ELogLevel::ELL_Error:
		{
			WriteConsoleW(consoleHandle, L"- ERROR: ", 9, NULL, NULL);
			break;
		}

		default:
		{
			WriteConsoleW(consoleHandle, L"- ", 2, NULL, NULL);
			break;
		}
	}

	WriteConsoleW(consoleHandle, utf16BEMsg, (DWORD)msgLength, NULL, NULL);
	WriteConsoleW(consoleHandle, L"\n", 1, NULL, NULL);
#endif
}
