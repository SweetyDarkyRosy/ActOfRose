/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CFunction.cpp
	@Created:		19.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CFunction.h"

#include "ReturnCodes.h"
#include "Log.h"
#include "CExecutor.h"
#include "Utility/StringMisc.h"
#include "Value/Value.h"


// ----- ActOfRose::CFunction class -----

// Executes a function with a specified set of parameters
int ActOfRose::CFunction::Execute(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::SValueReference>* params)
{
	// ----- Checking -----

	if (_mParamNames.size() < params->size())
	{
		ActOfRose::WriteLog(PREF_STRING("Too many arguments have been passed"),
			(sizeof(PREF_STRING("Too many arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
	}
	else if (_mParamNames.size() > params->size())
	{
		ActOfRose::WriteLog(PREF_STRING("Too few arguments have been passed"),
			(sizeof(PREF_STRING("Too few arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
	}


	if (returnValueHolder != nullptr)
	{
		returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_None;
	}


	// ----- Execution -----

	ActOfRose::CExecutor executor;				// Local instance of executor
	{
		int execResult = executor.Execute(returnValueHolder, &_mTokens);
		if (execResult != AOR_SUCCESS)
		{
			return execResult;
		}
	}

	return AOR_SUCCESS;
}
