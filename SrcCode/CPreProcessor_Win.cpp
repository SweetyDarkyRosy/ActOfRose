/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CPreProcessor_Win.cpp
	@Created:		15.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Windows OS-specific implementations. */

#include "CPreProcessor.h"

#include "ReturnCodes.h"
#include "Log.h"
#include "Token.h"
#include "SystemAPI.h"
#include "Utility/StringMisc.h"
#include "Utility/StringConverting.h"
#include "Value/Value.h"


// Processes parameters/arguments in the default execution mode for later execution of scripts
int ActOfRose::CPreProcessor::ProcessParametersInExecMode(int argCount, std::wstring* args, std::vector<ActOfRose::SPreProcessorOperation*>* opDeclArr)
{
	// ----- Argument/parameter processing -----

	unsigned int currArgIndex = 1;

	while (currArgIndex < (unsigned int)(argCount))
	{
		if (args[currArgIndex].compare(L"-P") == 0)
		{
			// ----- If a parameter with a custom path to a root script found -----

			if ((argCount - (int)currArgIndex) < 2)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			std::filesystem::path pathDetected = args[currArgIndex + 1];
			if (std::filesystem::exists(pathDetected) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Custom path to a script file is invalid"),
					(sizeof(PREF_STRING("Custom path to a script file is invalid")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}
			else if (std::filesystem::is_regular_file(pathDetected) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Specified path does not refer to a file"),
					(sizeof(PREF_STRING("Specified path does not refer to a file")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}

			_mRootScriptCustomPath = pathDetected;

			currArgIndex += 2;
		}
		else if (args[currArgIndex].compare(L"-D") == 0)
		{
			// ----- If a parameter with a custom path to a root script found -----

			if ((argCount - (int)currArgIndex) < 3)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			{
				ActOfRose::SPreProcessorSetValueOperation* setValueOp = new ActOfRose::SPreProcessorSetValueOperation();

				if (ConvertStringUTF16BEToUTF8(&(setValueOp->identifier), &(args[currArgIndex + 1])) != AOR_SUCCESS)
				{
					ActOfRose::WriteLog(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8"),
						(sizeof(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_INTERNAL_ERROR;
				}

				if (ConvertStringUTF16BEToUTF8(&(setValueOp->value), &(args[currArgIndex + 2])) != AOR_SUCCESS)
				{
					ActOfRose::WriteLog(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8"),
						(sizeof(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_INTERNAL_ERROR;
				}

				opDeclArr->push_back(setValueOp);
			}

			currArgIndex += 3;
		}
		else
		{
			ActOfRose::WriteLog(PREF_STRING("Invalid parameter"), (sizeof(PREF_STRING("Invalid parameter")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_INVALID_PARAMETER;
		}
	}

	return AOR_SUCCESS;
}

// Processes parameters/arguments in the cache editing mode
int ActOfRose::CPreProcessor::ProcessParametersInCacheEditMode(int argCount, std::wstring* args, std::vector<ActOfRose::SPreProcessorOperation*>* opDeclArr)
{
	return AOR_PREPROCESSOR_EXIT;
}
