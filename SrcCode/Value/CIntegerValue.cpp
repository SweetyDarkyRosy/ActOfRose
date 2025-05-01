/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CIntegerValue.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CIntegerValue.h"

#include "CFloatValue.h"
#include "CBooleanValue.h"

#include <ReturnCodes.h>
#include <Utility/StringConverting.h>


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
	switch (opType)
	{
		case ActOfRose::Operation::EOperationTypes::EO_Summation:
		{
			if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				break;
			}

			ActOfRose::Value::CValue* rightValue = rightValRef->GetValue();
			switch (rightValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					int intResult = _mValue + rightIntValue->GetRawValue();

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					float floatResult = (float)_mValue + rightFloatValue->GetRawValue();

					retValueRefHolder->value.value = new ActOfRose::Value::CFloatValue(floatResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					int intResult = _mValue + (int)(rightBoolValue->GetRawValue());

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				default:
				{
					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}
			}

			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

			break;
		}

		case ActOfRose::Operation::EOperationTypes::EO_Subtraction:
		{
			if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				break;
			}

			ActOfRose::Value::CValue* rightValue = rightValRef->GetValue();
			switch (rightValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					int intResult = _mValue - rightIntValue->GetRawValue();

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					float floatResult = (float)_mValue - rightFloatValue->GetRawValue();

					retValueRefHolder->value.value = new ActOfRose::Value::CFloatValue(floatResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					int intResult = _mValue - (int)(rightBoolValue->GetRawValue());

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				default:
				{
					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}
			}

			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

			break;
		}

		case ActOfRose::Operation::EOperationTypes::EO_Multiplication:
		{
			if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				break;
			}

			ActOfRose::Value::CValue* rightValue = rightValRef->GetValue();
			switch (rightValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					int intResult = _mValue * rightIntValue->GetRawValue();

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					float floatResult = (float)_mValue * rightFloatValue->GetRawValue();

					retValueRefHolder->value.value = new ActOfRose::Value::CFloatValue(floatResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					int intResult = _mValue * (int)(rightBoolValue->GetRawValue());

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				default:
				{
					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}
			}

			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

			break;
		}

		case ActOfRose::Operation::EOperationTypes::EO_Division:
		{
			if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				break;
			}

			ActOfRose::Value::CValue* rightValue = rightValRef->GetValue();
			switch (rightValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					int intResult = _mValue / rightIntValue->GetRawValue();

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					float floatResult = (float)_mValue / rightFloatValue->GetRawValue();

					retValueRefHolder->value.value = new ActOfRose::Value::CFloatValue(floatResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					int intResult = _mValue / (int)(rightBoolValue->GetRawValue());

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				default:
				{
					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}
			}

			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

			break;
		}

		case ActOfRose::Operation::EOperationTypes::EO_Negation:
		{
			retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(_mValue * (-1));
			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

			break;
		}

		case ActOfRose::Operation::EOperationTypes::EO_CampareEqualTo:
		{
			if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				break;
			}

			ActOfRose::Value::CValue* rightValue = rightValRef->GetValue();
			switch (rightValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue(_mValue == rightIntValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((float)_mValue == rightFloatValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue(_mValue == (int)(rightBoolValue->GetRawValue()));

					break;
				}

				default:
				{
					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}
			}

			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

			break;
		}

		case ActOfRose::Operation::EOperationTypes::EO_CampareNotEqualTo:
		{
			if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				break;
			}

			ActOfRose::Value::CValue* rightValue = rightValRef->GetValue();
			switch (rightValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue(_mValue != rightIntValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((float)_mValue != rightFloatValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue(_mValue != (int)(rightBoolValue->GetRawValue()));

					break;
				}

				default:
				{
					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}
			}

			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

			break;
		}

		case ActOfRose::Operation::EOperationTypes::EO_CampareLessThan:
		{
			if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				break;
			}

			ActOfRose::Value::CValue* rightValue = rightValRef->GetValue();
			switch (rightValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue(_mValue < rightIntValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((float)_mValue < rightFloatValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue(_mValue < (int)(rightBoolValue->GetRawValue()));

					break;
				}

				default:
				{
					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}
			}

			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

			break;
		}

		default:
		{
			return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
		}
	}

	return AOR_SUCCESS;
}
