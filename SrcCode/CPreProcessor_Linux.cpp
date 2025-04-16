/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CPreProcessor_Linux.cpp
	@Created:		16.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Linux OS-specific implementations. */

#include "CPreProcessor.h"

#include <cstring>
#include <string>

#include "ReturnCodes.h"
#include "Log.h"
#include "Token.h"
#include "SystemAPI.h"
#include "Utility/StringMisc.h"
#include "Utility/StringConverting.h"
#include "Value/Value.h"


// Processes parameters/arguments in the default execution mode for later execution of scripts
int ActOfRose::CPreProcessor::ProcessParametersInExecMode(int argCount, char** args, std::vector<ActOfRose::SPreProcessorOperation*>* opDeclArr)
{
	// ----- Argument/parameter processing -----

	unsigned int currArgIndex = 1;

	while (currArgIndex < (unsigned int)(argCount))
	{
		if (std::strcmp(args[currArgIndex], "-P") == 0)
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
		else if (std::strcmp(args[currArgIndex], "-D") == 0)
		{
			// ----- If parameter for setting a predefined value found -----

			if ((argCount - (int)currArgIndex) < 3)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			ActOfRose::SPreProcessorSetValueOperation* setValueOp = new ActOfRose::SPreProcessorSetValueOperation();
			setValueOp->identifier = args[currArgIndex + 1];
			setValueOp->value = args[currArgIndex + 2];

			opDeclArr->push_back(setValueOp);

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
int ActOfRose::CPreProcessor::ProcessParametersInCacheEditMode(int argCount, char** args, std::vector<ActOfRose::SPreProcessorOperation*>* opDeclArr)
{
	// ----- Argument/parameter processing -----

	unsigned int currArgIndex = 2;

	while (currArgIndex < (unsigned int)(argCount))
	{
		if (std::strcmp(args[currArgIndex], "-P") == 0)
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
		else if (std::strcmp(args[currArgIndex], "-D") == 0)
		{
			// ----- If parameter for setting a predefined value found -----

			if ((argCount - (int)currArgIndex) < 3)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			ActOfRose::SPreProcessorSetValueOperation* setValueOp = new ActOfRose::SPreProcessorSetValueOperation();
			setValueOp->identifier = args[currArgIndex + 1];
			setValueOp->value = args[currArgIndex + 2];

			opDeclArr->push_back(setValueOp);

			currArgIndex += 3;
		}
		else if (std::strcmp(args[currArgIndex], "-R") == 0)
		{
			// ----- If parameter for removing a predefined value found -----

			if ((argCount - (int)currArgIndex) < 2)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			ActOfRose::SPreProcessorRemoveValueOperation* removeValueOp = new ActOfRose::SPreProcessorRemoveValueOperation();
			removeValueOp->identifier = args[currArgIndex + 1];

			opDeclArr->push_back(removeValueOp);

			currArgIndex += 2;
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
