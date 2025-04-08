/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CBooleanValue.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CBooleanValue.h"

#include <Utility/StringConverting.h>

#include "ReturnCodes.h"


// ----- ActOfRose::Value::CBooleanValue class -----

// Converts a value to a byte string
std::string ActOfRose::Value::CBooleanValue::ConvertValueToByteString() const
{
	return std::string((_mValue == false) ? "False" : "True");
}

// Converts a value to a wide string
std::wstring ActOfRose::Value::CBooleanValue::ConvertValueToWideString() const
{
	return std::wstring((_mValue == false) ? L"False" : L"True");
}

// Performs an operation of a specified type with a current value and a given right value
int ActOfRose::Value::CBooleanValue::ExecuteOperation(ActOfRose::Value::SValueReference* retValueRefHolder,
	ActOfRose::Operation::EOperationTypes opType, ActOfRose::Value::SValueReference* rightValRef)
{
	return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
}
