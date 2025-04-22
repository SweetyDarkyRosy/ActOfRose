/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Function.cpp
	@Created:		19.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Function.h"

#include "ReturnCodes.h"
#include "Log.h"
#include "CExecutor.h"
#include "Utility/StringMisc.h"
#include "Value/Value.h"


// ----- ActOfRose::CUserFunction class -----

// Executes a function with a specified set of parameters
int ActOfRose::CUserFunction::Execute(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params)
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


	/**
		Temporary deleting of incoming parameter values (they are not used now)
	 */
	for (unsigned int paramIt = 0; paramIt < (unsigned int)(params->size()); paramIt++)
	{
		delete (*params)[paramIt];
	}

	return AOR_SUCCESS;
}
