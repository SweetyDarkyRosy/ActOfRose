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
#elif defined (__linux__)
	#include <unistd.h>
#endif

#include "ReturnCodes.h"
#include "Utility/StringConverting.h"


// Writes a given null-terminated UTF-8-encoded message as a log with the specified log level
void ActOfRose::WriteLog(const char* utf8Msg, std::size_t msgLength, ActOfRose::ELogLevel level)
{
#if defined (WIN32) || defined (_WIN32)
	std::wstring utf16BEStr;

	if (ConvertStringUTF8ToUTF16BE(&utf16BEStr, utf8Msg, msgLength) == AOR_SUCCESS)
	{
		WriteLog(utf16BEStr.c_str(), utf16BEStr.length(), level);
	}
#elif defined (__linux__)
	switch (level)
	{
		case ActOfRose::ELogLevel::ELL_Debug:
		{
			write(1, (const void*)("- \x1B[34mDEBUG\x1B[0m: "), 18);
			break;
		}

		case ActOfRose::ELogLevel::ELL_Warning:
		{
			write(1, (const void*)("- \x1B[35mWARNING\x1B[0m: "), 20);
			break;
		}

		case ActOfRose::ELogLevel::ELL_Error:
		{
			write(1, (const void*)("- \x1B[31mERROR\x1B[0m: "), 18);
			break;
		}

		default:
		{
			write(1, (const void*)("- "), 2);
			break;
		}
	}

	write(1, (const void*)utf8Msg, msgLength);
	write(1, "\n", 1);
#endif
}

// Writes a given null-terminated UTF-16BE-encoded message as a log with the specified log level
void ActOfRose::WriteLog(const wchar_t* utf16BEMsg, std::size_t msgLength, ActOfRose::ELogLevel level)
{
#if defined (WIN32) || defined (_WIN32)
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	WriteConsoleW(consoleHandle, L"- ", 2, NULL, NULL);

	if (level != ActOfRose::ELogLevel::ELL_Info)
	{
		WORD defaultColourAttrs;
		{
			CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
			GetConsoleScreenBufferInfo(consoleHandle, &consoleScreenBufferInfo);
			defaultColourAttrs = consoleScreenBufferInfo.wAttributes;
		}

		switch (level)
		{
			case ActOfRose::ELogLevel::ELL_Debug:
			{
				SetConsoleTextAttribute(consoleHandle, (FOREGROUND_INTENSITY | FOREGROUND_BLUE));
				WriteConsoleW(consoleHandle, L"DEBUG", 5, NULL, NULL);

				break;
			}

			case ActOfRose::ELogLevel::ELL_Warning:
			{
				SetConsoleTextAttribute(consoleHandle, (FOREGROUND_RED | FOREGROUND_BLUE));
				WriteConsoleW(consoleHandle, L"WARNING", 7, NULL, NULL);
				break;
			}

			case ActOfRose::ELogLevel::ELL_Error:
			{
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
				WriteConsoleW(consoleHandle, L"ERROR", 5, NULL, NULL);

				break;
			}

			default:
			{
				break;
			}
		}

		SetConsoleTextAttribute(consoleHandle, defaultColourAttrs);

		WriteConsoleW(consoleHandle, L": ", 2, NULL, NULL);
	}

	WriteConsoleW(consoleHandle, utf16BEMsg, (DWORD)msgLength, NULL, NULL);
	WriteConsoleW(consoleHandle, L"\n", 1, NULL, NULL);
#elif defined (__linux__)
	std::string utf8Str;

	if (ConvertStringUTF16BEToUTF8(&utf8Str, utf16BEMsg, msgLength) == AOR_SUCCESS)
	{
		WriteLog(utf8Str.c_str(), utf8Str.length(), level);
	}
#endif
}
