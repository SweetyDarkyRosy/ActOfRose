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
#include <SystemAPI.h>
#include <Log.h>
#include <Script.h>
#include <Utility/StringMisc.h>
#include <Utility/StringConverting.h>

#include <Value/CBooleanValue.h>
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

				result = rootScript.Execute(returnValueHolder);
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

// Checks if an element with a specified name has been defined
int ActOfRose::BuiltIn::IsDefined(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params)
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

	if (((*params)[0] == nullptr) || ((*params)[0]->GetValueType() != ActOfRose::Value::EValueType::EVT_String))
	{
		ActOfRose::WriteLog(PREF_STRING("Invalid parameter. String with path should be passed"),
			(sizeof(PREF_STRING("Invalid parameter. String with path should be passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

		result = AOR_ERROR_EXEC_INVALID_PARAMETER;
	}
	else
	{
		ActOfRose::Value::CStringValue* elName = (ActOfRose::Value::CStringValue*)((*params)[0]);

		if ((AORSystemIsIdentifierUsedGlobally(elName->GetRawString()) == true) ||
			(AORSystemIsIdentifierUsedLocally(elName->GetRawString()) == true))
		{
			returnValueHolder->value.value = new ActOfRose::Value::CBooleanValue(true);
		}
		else
		{
			returnValueHolder->value.value = new ActOfRose::Value::CBooleanValue(false);
		}

		returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;
	}


	// ----- Cleanup -----

	delete (*params)[0];


	return result;
}

// Returns the path of current script
int ActOfRose::BuiltIn::GetCurrentScriptPath(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params)
{
	if (params->size() != 0)
	{
		ActOfRose::WriteLog(PREF_STRING("Too many arguments have been passed"),
			(sizeof(PREF_STRING("Too many arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
	}

	std::string utf8AbsolutePathStr;
#if defined (WIN32) || defined (_WIN32)
	{
		std::filesystem::path* currScriptPath = gScriptContexts.top().GetScript()->GetPath();
		std::wstring utf16BEAbsolutePathStr = std::filesystem::absolute(*currScriptPath).wstring();

		if (ConvertStringUTF16BEToUTF8(&utf8AbsolutePathStr, &utf16BEAbsolutePathStr) != AOR_SUCCESS)
		{
			ActOfRose::WriteLog(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8"),
				(sizeof(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_INTERNAL_ERROR;
		}
	}
#elif defined (__linux__)
	{
		std::filesystem::path* currScriptPath = gScriptContexts.top().GetScript()->GetPath();
		utf8AbsolutePathStr = std::filesystem::absolute(*currScriptPath).string();
	}
#endif

	returnValueHolder->value.value = new ActOfRose::Value::CStringValue(utf8AbsolutePathStr.c_str());
	returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

	return AOR_SUCCESS;
}

// Returns the path of root script
int ActOfRose::BuiltIn::GetRootScriptPath(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params)
{
	if (params->size() != 0)
	{
		ActOfRose::WriteLog(PREF_STRING("Too many arguments have been passed"),
			(sizeof(PREF_STRING("Too many arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
	}

#if defined (WIN32) || defined (_WIN32)
	std::string utf8AbsolutePathStr;
	{
		std::wstring utf16BEAbsolutePathStr = std::filesystem::absolute(gRootScriptPath).wstring();

		if (ConvertStringUTF16BEToUTF8(&utf8AbsolutePathStr, &utf16BEAbsolutePathStr) != AOR_SUCCESS)
		{
			ActOfRose::WriteLog(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8"),
				(sizeof(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_INTERNAL_ERROR;
		}
	}
	returnValueHolder->value.value = new ActOfRose::Value::CStringValue(utf8AbsolutePathStr.c_str());
#elif defined (__linux__)
	returnValueHolder->value.value = new ActOfRose::Value::CStringValue(gRootScriptPath.string().c_str());
#endif

	returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

	return AOR_SUCCESS;
}
