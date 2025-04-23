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
#include "SystemAPI.h"
#include "CVariable.h"
#include "CExecutor.h"
#include "Utility/StringMisc.h"
#include "Value/Value.h"


// ----- ActOfRose::CPredefinedFunction class -----

// Executes a function with a specified set of parameters
int ActOfRose::CPredefinedFunction::Execute(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params)
{
	return _pFunc(returnValueHolder, params);
}


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


	// ----- Preparation of arguments as local variables -----

	// Creates a function's local scope
	ActOfRose::AORSystemAddScope(ActOfRose::EScopeVisibilityTypes::ESIT_LocalScope);

	for (unsigned int paramIt = 0; paramIt < (unsigned int)(params->size()); paramIt++)
	{
		// New variable
		ActOfRose::CVariable* newLocalVariable = new ActOfRose::CVariable((*params)[paramIt]);

		if (AORSystemRegisterLocalIdentifierAndElement(_mParamNames[paramIt].c_str(), ActOfRose::EElementType::EET_Variable, (void*)newLocalVariable) == nullptr)
		{
			delete newLocalVariable;

			ActOfRose::AORSystemRemoveScope();

			std::string errorMsg = "Runtime error. Failed to declare the ";
			errorMsg += _mParamNames[paramIt];
			errorMsg += " variable";

			ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_INTERNAL_ERROR;
		}
	}


	// ----- Execution -----

	ActOfRose::CExecutor executor;				// Local instance of executor
	int execResult = executor.Execute(returnValueHolder, &_mTokens);

	// Removes a function's local scope
	ActOfRose::AORSystemRemoveScope();

	if (execResult == AOR_LEAVE_EXECUTION)
	{
		execResult = AOR_SUCCESS;
	}

	return execResult;
}
