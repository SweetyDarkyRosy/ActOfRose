/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CIntegerValue.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CIntegerValue.h"

#include <Utility/StringConverting.h>

#include "ReturnCodes.h"


// ----- ActOfRose::Value::CIntegerValue class -----

// Converts a value to a byte string
std::string ActOfRose::Value::CIntegerValue::ConvertValueToByteString() const
{
	return std::to_string(_mValue);
}

// Converts a value to a wide string
std::wstring ActOfRose::Value::CIntegerValue::ConvertValueToWideString() const
{
	std::string utf8Str = std::to_string(_mValue);
	std::wstring utf16BEStr;

	ConvertStringUTF8ToUTF16BE(&utf16BEStr, &utf8Str);

	return utf16BEStr;
}

// Performs an operation of a specified type with a current value and a given right value
int ActOfRose::Value::CIntegerValue::ExecuteOperation(ActOfRose::Value::SValueReference* retValueRefHolder,
	ActOfRose::Operation::EOperationTypes opType, ActOfRose::Value::SValueReference* rightValRef)
{
	ActOfRose::Value::CValue* rightValue;
	if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_LValue)
	{
		rightValue = *(rightValRef->value.valueHolder);
	}
	else
	{
		rightValue = rightValRef->value.value;
	}

	switch (rightValue->GetValueType())
	{
		case ActOfRose::Value::EValueType::EVT_Integer:
		{
			ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
			int intResult;

			switch (opType)
			{
				case ActOfRose::Operation::EOperationTypes::EO_Summation:
				{
					intResult = _mValue + rightIntValue->GetRawValue();
					break;
				}

				default:
				{
					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}
			}

			retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

			break;
		}

		default:
		{
			return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
		}
	}

	retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

	return AOR_SUCCESS;
}
