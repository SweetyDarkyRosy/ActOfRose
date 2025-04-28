/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			BuiltInFuncOS.cpp
	@Created:		22.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "BuiltInFuncOS.h"

#if defined (WIN32) || defined (_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif defined (__linux__)
	#include <unistd.h>
#endif

#include <Utility/StringMisc.h>

#include "ReturnCodes.h"
#include "Log.h"
#include "Utility/StringMisc.h"

#include "Value/CStringValue.h"


// Prints a desired value to console
int ActOfRose::BuiltIn::Print(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params)
{
	if (params->size() != 1)
	{
		if (params->size() > 1)
		{
			ActOfRose::WriteLog(PREF_STRING("Too many arguments have been passed"),
				(sizeof(PREF_STRING("Too many arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
		}
		else
		{
			ActOfRose::WriteLog(PREF_STRING("Too few arguments have been passed"),
				(sizeof(PREF_STRING("Too few arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
		}
	}

	if ((*params)[0] != nullptr)
	{
		returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_None;

	#if defined (WIN32) || defined (_WIN32)
		std::wstring utf16BEValueStr = (*params)[0]->ConvertValueToWideString();

		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		WriteConsoleW(consoleHandle, utf16BEValueStr.c_str(), (DWORD)(utf16BEValueStr.length()), NULL, NULL);
		WriteConsoleW(consoleHandle, L"\n", 1, NULL, NULL);
	#elif defined (__linux__)
		std::string utf8ValueStr = (*params)[0]->ConvertValueToByteString();

		write(1, (const void*)(utf8ValueStr.c_str()), utf8ValueStr.length());
		write(1, "\n", 1);
	#endif

		delete (*params)[0];
	}

	return AOR_SUCCESS;
}
