/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			SystemAPI.cpp
	@Created:		25.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "SystemAPI.h"

#include <string>
#include <map>
#include <stack>
#include <cmath>

#include "ReturnCodes.h"
#include "Global.h"
#include "Log.h"
#include "Token.h"
#include "Script.h"
#include "Utility/StringMisc.h"

#include "Value/CIntegerValue.h"
#include "Value/CFloatValue.h"
#include "Value/CBooleanValue.h"
#include "Value/CStringValue.h"
#include "Value/CCharValue.h"
#include "Value/CArrayValue.h"


/*[
	Functions for working with script contexts
 ]*/

// Adds a new script context related to the script
void ActOfRose::AORSystemPushScriptContext(ActOfRose::Script::CScript* script)
{
	gScriptContexts.push(script);
}

// Removes current script context
void ActOfRose::AORSystemPopScriptContext()
{
	gScriptContexts.pop();
}


/*[
	Functions for working with scopes in script contexts
 ]*/

// Adds/pushes a scope in current script context
void ActOfRose::AORSystemAddScope(ActOfRose::EScopeVisibilityTypes visibilityType)
{
	gScriptContexts.top().AddScope(visibilityType);
}

// Removes a top scope from current script context
void ActOfRose::AORSystemRemoveScope()
{
	gScriptContexts.top().RemoveScope();
}


/*[
	Functions for working with elements
 ]*/

// Registers an identifier and builds an association with an element (variable, constant or function)
ActOfRose::SElement* ActOfRose::AORSystemRegisterGlobalIdentifierAndElement(const char* identifier, ActOfRose::EElementType type, void* addr)
{
	std::pair<std::map<const std::string, ActOfRose::SElement>::iterator, bool> result = gGlobalIdentifierMap.insert({ identifier, { type, addr } });
	
	if (result.second == true)
	{
		std::pair<const std::string, ActOfRose::SElement>* newElPair = &(*(result.first));

		return &(newElPair->second);
	}

	return nullptr;
}

// Checks if the identifier is already in use
bool ActOfRose::AORSystemIsIdentifierUsedGlobally(const char* identifier)
{
	std::map<const std::string, ActOfRose::SElement>::iterator identifierIt = gGlobalIdentifierMap.find(identifier);
	if (identifierIt == gGlobalIdentifierMap.end())
	{
		return false;
	}

	return true;
}

// Returns a pointer to a block of information about registered element by the given identifier if it exists
ActOfRose::SElement* ActOfRose::AORSystemGetGlobalElementByIdentifier(const char* identifier)
{
	std::map<const std::string, ActOfRose::SElement>::iterator identifierIt = gGlobalIdentifierMap.find(identifier);
	if (identifierIt == gGlobalIdentifierMap.end())
	{
		return nullptr;
	}

	std::pair<const std::string, ActOfRose::SElement>* elementPair = &(*identifierIt);
	
	return &(elementPair->second);
}


// Registers an identifier and builds an association with an element (variable or constant) in current scope
ActOfRose::SElement* ActOfRose::AORSystemRegisterLocalIdentifierAndElement(const char* identifier, ActOfRose::EElementType type, void* addr)
{
	return gScriptContexts.top().GetScopes()->back().RegisterIdentifierAndElement(identifier, type, addr);
}

// Checks if the identifier is already in use in current or lower accessible scopes
bool ActOfRose::AORSystemIsIdentifierUsedLocally(const char* identifier)
{
	std::vector<ActOfRose::CScope>* scopes = gScriptContexts.top().GetScopes();
	for (unsigned int scopeIt = (unsigned int)(scopes->size()) - 1; scopeIt >= 0; scopeIt--)
	{
		if ((*scopes)[scopeIt].IsIdentifierUsed(identifier) == true)
		{
			return true;
		}

		if ((*scopes)[scopeIt].GetElementVisibilityType() == ActOfRose::EScopeVisibilityTypes::ESIT_LocalScope)
		{
			break;
		}
	}

	return false;
}

// Checks if the identifier is already in use in current scope
bool ActOfRose::AORSystemIsIdentifierUsedInScope(const char* identifier)
{
	return gScriptContexts.top().GetScopes()->back().IsIdentifierUsed(identifier);
}

// Returns a pointer to a block of information about registered element by the given identifier if it exists in current or lower accessible scopes
ActOfRose::SElement* ActOfRose::AORSystemGetLocalElementByIdentifier(const char* identifier)
{
	std::vector<ActOfRose::CScope>* scopes = gScriptContexts.top().GetScopes();
	for (unsigned int scopeIt = (unsigned int)(scopes->size()) - 1; scopeIt >= 0; scopeIt--)
	{
		ActOfRose::SElement* el = (*scopes)[scopeIt].GetElementByIdentifier(identifier);
		if (el != nullptr)
		{
			return el;
		}

		if ((*scopes)[scopeIt].GetElementVisibilityType() == ActOfRose::EScopeVisibilityTypes::ESIT_LocalScope)
		{
			break;
		}
	}

	return nullptr;
}


/*
	Returns a pointer to a block of information about registered element by the given identifier if it exists
	globally or locally in current or lower accessible scopes
 **/
ActOfRose::SElement* ActOfRose::AORSystemGetElementByIdentifier(const char* identifier)
{
	ActOfRose::SElement* el = nullptr;

	el = AORSystemGetLocalElementByIdentifier(identifier);
	if (el != nullptr)
	{
		return el;
	}

	el = AORSystemGetGlobalElementByIdentifier(identifier);
	if (el != nullptr)
	{
		return el;
	}

	return nullptr;
}


/*[
	Functions for working with values
 ]*/
	
// Creates a value from token and saves it to a value holder pointed to by valueHolder
int ActOfRose::AORSystemCreateValueFromToken(ActOfRose::Value::CValue** valueHolder, ActOfRose::Token::SToken* token)
{
	ActOfRose::Value::CValue* newValue;

	switch (token->type)
	{
		case ActOfRose::Token::ETokenType::ETTNumber:
		{
			float numValue = std::stof(token->value);
			if (std::floor(numValue) == numValue)
			{
				newValue = new ActOfRose::Value::CIntegerValue(std::stoi(token->value));
			}
			else
			{
				newValue = new ActOfRose::Value::CFloatValue(numValue);
			}

			break;
		}

		case ActOfRose::Token::ETokenType::ETTString:
		{
			if ((token->value.compare("true") == 0) || (token->value.compare("false") == 0))
			{
				newValue = new ActOfRose::Value::CBooleanValue(token->value.compare("true") == 0);
			}
			else
			{
				newValue = new ActOfRose::Value::CStringValue(token->value.c_str());
			}

			break;
		}

		default:
		{
			{
				std::string errorMsg = "Could not create a value from a token with value \"";
				errorMsg += token->value;
				errorMsg += "\"";

				ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);
			}

			return AOR_ERROR_INTERNAL_ERROR;
		}
	}

	if (valueHolder != nullptr)
	{
		*valueHolder = newValue;
	}

	return AOR_SUCCESS;
}

// Creates a copy of a value pointed to by originalValue and returns a pointer to the copy
ActOfRose::Value::CValue* ActOfRose::AORSystemCopyValue(ActOfRose::Value::CValue* originalValue)
{
	ActOfRose::Value::CValue* newValue = nullptr;

	switch (originalValue->GetValueType())
	{
		case ActOfRose::Value::EValueType::EVT_Boolean:
		{
			ActOfRose::Value::CBooleanValue* newBooleanValue = new ActOfRose::Value::CBooleanValue(
				((ActOfRose::Value::CBooleanValue*)originalValue)->GetRawValue());

		#ifdef _DEBUG
			std::string logMsg = "Copy of boolean value has been created (" + newBooleanValue->ConvertValueToByteString() + ")";
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		#endif

			newValue = (ActOfRose::Value::CValue*)newBooleanValue;

			break;
		}

		case ActOfRose::Value::EValueType::EVT_Integer:
		{
			ActOfRose::Value::CIntegerValue* newIntValue = new ActOfRose::Value::CIntegerValue(
				((ActOfRose::Value::CIntegerValue*)originalValue)->GetRawValue());

		#ifdef _DEBUG
			std::string logMsg = "Copy of integer value has been created (" + newIntValue->ConvertValueToByteString() + ")";
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		#endif

			newValue = (ActOfRose::Value::CValue*)newIntValue;

			break;
		}

		case ActOfRose::Value::EValueType::EVT_FloatingPoint:
		{
			ActOfRose::Value::CFloatValue* newFloatValue = new ActOfRose::Value::CFloatValue(
				((ActOfRose::Value::CFloatValue*)originalValue)->GetRawValue());

		#ifdef _DEBUG
			std::string logMsg = "Copy of floating-point value has been created (" + newFloatValue->ConvertValueToByteString() + ")";
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		#endif

			newValue = (ActOfRose::Value::CValue*)newFloatValue;

			break;
		}

		case ActOfRose::Value::EValueType::EVT_Character:
		{
			ActOfRose::Value::CCharValue* newCharValue = new ActOfRose::Value::CCharValue(
				((ActOfRose::Value::CCharValue*)originalValue)->GetRawCharSet());

		#ifdef _DEBUG
			std::string logMsg = "Copy of character value has been created (" + newCharValue->ConvertValueToByteString() + ")";
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		#endif

			newValue = (ActOfRose::Value::CValue*)newCharValue;

			break;
		}

		case ActOfRose::Value::EValueType::EVT_String:
		{
			ActOfRose::Value::CStringValue* newString = new ActOfRose::Value::CStringValue(
				(ActOfRose::Value::CStringValue*)originalValue);

		#ifdef _DEBUG
			std::string logMsg = "Copy of string has been created (\"" + newString->ConvertValueToByteString() + "\")";
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		#endif

			newValue = (ActOfRose::Value::CValue*)newString;

			break;
		}

		case ActOfRose::Value::EValueType::EVT_Array:
		{
			ActOfRose::Value::CArrayValue* newArray = new ActOfRose::Value::CArrayValue(
				(ActOfRose::Value::CArrayValue*)originalValue);

		#ifdef _DEBUG
			std::string logMsg = "Copy of array has been created (" + newArray->ConvertValueToByteString() + ")";
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		#endif

			newValue = (ActOfRose::Value::CValue*)newArray;

			break;
		}

		default:
		{
			std::string errorMsg = "Copying of values of type \"";
				errorMsg += originalValue->GetTypeByteString();
				errorMsg += "\" is not supported";
	
			ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);

			break;
		}
	}

	return newValue;
}
