/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CArrayValue.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CArrayValue.h"

#include <Utility/StringConverting.h>

#include "SystemAPI.h"
#include "ReturnCodes.h"


// ----- ActOfRose::Value::CArrayValue class -----

// Constructor that takes another array and makes a copy
ActOfRose::Value::CArrayValue::CArrayValue(const ActOfRose::Value::CArrayValue* array) :
	CValue(ActOfRose::Value::EValueType::EVT_Array)
{
	for (unsigned int valueIt = 0; valueIt < (unsigned int)(array->_mValueArray.size()); valueIt++)
	{
		ActOfRose::Value::CValue* newValue = AORSystemCopyValue(array->_mValueArray[valueIt]);
		_mValueArray.push_back(newValue);
	}
}

// Destructor
ActOfRose::Value::CArrayValue::~CArrayValue()
{
	for (unsigned int valueIt = 0; valueIt < (unsigned int)(_mValueArray.size()); valueIt++)
	{
		delete _mValueArray[valueIt];
	}
}

// Converts the array to a byte string
std::string ActOfRose::Value::CArrayValue::ConvertValueToByteString() const
{
	std::string resultValueStr = "{ ";

	for (unsigned int valueIt = 0; valueIt < (unsigned int)(_mValueArray.size()); valueIt++)
	{
		if (_mValueArray[valueIt]->GetValueType() == ActOfRose::Value::EValueType::EVT_String)
		{
			resultValueStr += '\"';
			resultValueStr += _mValueArray[valueIt]->ConvertValueToByteString();
			resultValueStr += '\"';
		}
		else
		{
			resultValueStr += _mValueArray[valueIt]->ConvertValueToByteString();
		}

		if (valueIt != ((unsigned int)(_mValueArray.size()) - 1))
		{
			resultValueStr += ", ";
		}
	}

	resultValueStr += " }";

	return resultValueStr;
}

// Converts the array to a wide string
std::wstring ActOfRose::Value::CArrayValue::ConvertValueToWideString() const
{
	std::wstring resultValueStr = L"{ ";

	for (unsigned int valueIt = 0; valueIt < (unsigned int)(_mValueArray.size()); valueIt++)
	{
		if (_mValueArray[valueIt]->GetValueType() == ActOfRose::Value::EValueType::EVT_String)
		{
			resultValueStr += L'\"';
			resultValueStr += _mValueArray[valueIt]->ConvertValueToWideString();
			resultValueStr += L'\"';
		}
		else
		{
			resultValueStr += _mValueArray[valueIt]->ConvertValueToWideString();
		}

		if (valueIt != ((unsigned int)(_mValueArray.size()) - 1))
		{
			resultValueStr += L", ";
		}
	}

	resultValueStr += L" }";

	return resultValueStr;
}

// Performs an operation of a specified type with a current value and a given right value
int ActOfRose::Value::CArrayValue::ExecuteOperation(ActOfRose::Value::SValueReference* retValueRefHolder,
	ActOfRose::Operation::EOperationTypes opType, ActOfRose::Value::SValueReference* rightValRef)
{
	return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
}
