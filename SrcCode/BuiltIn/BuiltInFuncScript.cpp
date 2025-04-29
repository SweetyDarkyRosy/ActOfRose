/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			BuiltInFuncScript.cpp
	@Created:		29.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "BuiltInFuncScript.h"

#include <ReturnCodes.h>
#include <Global.h>
#include <Log.h>
#include <Script.h>
#include <Utility/StringMisc.h>

#include <Value/CStringValue.h>


// Prints a desired value to console
int ActOfRose::BuiltIn::Act(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params)
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

	returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_None;

	int result = AOR_SUCCESS;

	if ((*params)[0] != nullptr)
	{
		if ((*params)[0]->GetValueType() != ActOfRose::Value::EValueType::EVT_String)
		{
			ActOfRose::WriteLog(PREF_STRING("Invalid parameter. String with path should be passed"),
				(sizeof(PREF_STRING("Invalid parameter. String with path should be passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			result = AOR_ERROR_EXEC_INVALID_PARAMETER;
		}
		else
		{
			ActOfRose::Value::CStringValue* pathStr = (ActOfRose::Value::CStringValue*)((*params)[0]);

			ActOfRose::Script::CScript rootScript(pathStr->GetRawString());
			if (rootScript.IsLoaded() == false)
			{
			#if defined (WIN32) || defined (_WIN32)
				std::wstring errMsg = L"Could not open the \"";
				errMsg += rootScript.GetPath()->wstring();
				errMsg += L"\" script file";
			#elif defined (__linux__)
				std::string errMsg = "Could not open the \"";
				errMsg += rootScript.GetPath()->string();
				errMsg += "\" script file";
			#endif

				ActOfRose::WriteLog(errMsg.c_str(), errMsg.length(), ActOfRose::ELogLevel::ELL_Error);
				
				result = AOR_ERROR_SCRIPT_FILE_NOT_LOADED;
			}
			else
			{
				// ----- Execution -----

				result = rootScript.Execute();
			}
		}
	}
	else
	{
		ActOfRose::WriteLog(PREF_STRING("Invalid parameter. String with path should be passed"),
			(sizeof(PREF_STRING("Invalid parameter. String with path should be passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

		result = AOR_ERROR_EXEC_INVALID_PARAMETER;
	}


	// ----- Cleanup -----

	delete (*params)[0];


	return result;
}
