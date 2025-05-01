/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CBooleanValue.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CBooleanValue.h"

#include <ReturnCodes.h>
#include <Utility/StringConverting.h>

#include "CIntegerValue.h"
#include "CFloatValue.h"


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
				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					int intResult = (int)_mValue + (int)(rightBoolValue->GetRawValue());

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					int intResult = (int)_mValue + rightIntValue->GetRawValue();

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
				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					int intResult = (int)_mValue - (int)(rightBoolValue->GetRawValue());

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					int intResult = (int)_mValue - rightIntValue->GetRawValue();

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
				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					int intResult = (int)_mValue * (int)(rightBoolValue->GetRawValue());

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					int intResult = (int)_mValue * rightIntValue->GetRawValue();

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
				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					int intResult = (int)_mValue / (int)(rightBoolValue->GetRawValue());

					retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue(intResult);

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					int intResult = (int)_mValue / rightIntValue->GetRawValue();

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
			retValueRefHolder->value.value = new ActOfRose::Value::CIntegerValue((int)_mValue * (-1));
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
				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue == (int)(rightBoolValue->GetRawValue()));

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue == rightIntValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((float)_mValue == rightFloatValue->GetRawValue());

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
				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue != (int)(rightBoolValue->GetRawValue()));

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue != rightIntValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((float)_mValue != rightFloatValue->GetRawValue());

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
				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue < (int)(rightBoolValue->GetRawValue()));

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue < rightIntValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((float)_mValue < rightFloatValue->GetRawValue());

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

		case ActOfRose::Operation::EOperationTypes::EO_CampareGreaterThan:
		{
			if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				break;
			}

			ActOfRose::Value::CValue* rightValue = rightValRef->GetValue();
			switch (rightValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue > (int)(rightBoolValue->GetRawValue()));

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue > rightIntValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((float)_mValue > rightFloatValue->GetRawValue());

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

		case ActOfRose::Operation::EOperationTypes::EO_CampareLessThanOrEqualTo:
		{
			if (rightValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				break;
			}

			ActOfRose::Value::CValue* rightValue = rightValRef->GetValue();
			switch (rightValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Boolean:
				{
					ActOfRose::Value::CBooleanValue* rightBoolValue = (ActOfRose::Value::CBooleanValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue <= (int)(rightBoolValue->GetRawValue()));

					break;
				}

				case ActOfRose::Value::EValueType::EVT_Integer:
				{
					ActOfRose::Value::CIntegerValue* rightIntValue = (ActOfRose::Value::CIntegerValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((int)_mValue <= rightIntValue->GetRawValue());

					break;
				}

				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					ActOfRose::Value::CFloatValue* rightFloatValue = (ActOfRose::Value::CFloatValue*)rightValue;
					
					retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue((float)_mValue <= rightFloatValue->GetRawValue());

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
