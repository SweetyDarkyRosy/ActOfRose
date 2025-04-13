/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CPreProcessor.cpp
	@Created:		12.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CPreProcessor.h"

#if defined (WIN32) || defined (_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	#include <stdlib.h>

	#include <vector>
#endif

#include <string>

#include "ReturnCodes.h"
#include "Log.h"
#include "Utility/StringMisc.h"


#if defined (WIN32) || defined (_WIN32)
// Checks if the character is a non-printable character
static inline bool IsWhitespaceUTF16BE(wchar_t value)
{
	return ((value == '\t') || (value == ' ') || (value == 0xA) || (value == 0xD));
}
#endif


// ----- ActOfRose::CPreProcessor class -----

// Processes a command line stream
int ActOfRose::CPreProcessor::ProcessCommandLine(int argCount, char** args)
{
#if defined (WIN32) || defined (_WIN32)
	// ----- Windows OS-specific preparation of arguments -----

	std::vector<std::wstring> utf16BEArguments;
	{
		const wchar_t* argWalker = GetCommandLineW();

		while (*argWalker != L'\0')
		{
			if (IsWhitespaceUTF16BE(*argWalker) == false)
			{
				std::wstring arg;

				if (*argWalker == L'"')
				{
					argWalker++;

					while ((*argWalker != L'"') && (*argWalker != L'\0'))
					{
						arg += *argWalker;
						argWalker++;
					}

					if (*argWalker == L'"')
					{
						argWalker++;
					}
				}
				else
				{
					while ((IsWhitespaceUTF16BE(*argWalker) == false) && (*argWalker != L'\0'))
					{
						arg += *argWalker;
						argWalker++;
					}
				}

				utf16BEArguments.push_back(arg);
			}
			else
			{
				argWalker++;
			}
		}
	}

	unsigned int currArgIndex = 1;

	while (currArgIndex < (unsigned int)(utf16BEArguments.size()))
	{
		if (utf16BEArguments[currArgIndex].compare(L"-P") == 0)
		{
			// ----- If a parameter with a custom path to a root script found -----

			if (((int)(utf16BEArguments.size()) - (int)currArgIndex) < 2)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			_mRootScriptCustomPath = utf16BEArguments[currArgIndex + 1];

			currArgIndex += 2;
		}
	}
#elif defined (__linux__)
	unsigned int currArgIndex = 1;

	while (currArgIndex < (unsigned int)argCount)
	{
		if (std::strcmp(args[currArgIndex], "-P") == 0)
		{
			// ----- If a parameter with a custom path to a root script found -----

			if (((unsigned int)argCount - currArgIndex) < 2)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			_mRootScriptCustomPath = args[currArgIndex + 1];

			currArgIndex += 2;
		}
	}
#endif

	return AOR_SUCCESS;
}
