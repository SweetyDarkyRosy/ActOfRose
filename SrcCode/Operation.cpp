/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Operation.cpp
	@Created:		31.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Operation.h"

#include <unordered_map>

#include "ReturnCodes.h"
#include "Log.h"
#include "SystemAPI.h"
#include "Value/Value.h"
#include "Utility/StringMisc.h"

#include "Value/CBooleanValue.h"


/*[
	Global variables
 ]*/

// Map of associations between null-terminated strings with operations and the operation classes
static const std::unordered_map<std::string, ActOfRose::Operation::EOperationTypes> sOperationMap = {
	{ "+", ActOfRose::Operation::EOperationTypes::EO_Summation },
	{ "-", ActOfRose::Operation::EOperationTypes::EO_Subtraction },
	{ "*", ActOfRose::Operation::EOperationTypes::EO_Multiplication },
	{ "/", ActOfRose::Operation::EOperationTypes::EO_Division },
	{ "=", ActOfRose::Operation::EOperationTypes::EO_Assignment },
	{ "==", ActOfRose::Operation::EOperationTypes::EO_CampareEqualTo },
	{ "!=", ActOfRose::Operation::EOperationTypes::EO_CampareNotEqualTo },
	{ "<", ActOfRose::Operation::EOperationTypes::EO_CampareLessThan },
	{ ">", ActOfRose::Operation::EOperationTypes::EO_CampareGreaterThan },
	{ "<=", ActOfRose::Operation::EOperationTypes::EO_CampareLessThanOrEqualTo },
	{ ">=", ActOfRose::Operation::EOperationTypes::EO_CampareGreaterThanOrEqualTo },
	{ "&&", ActOfRose::Operation::EOperationTypes::EO_LogicalAND },
	{ "||", ActOfRose::Operation::EOperationTypes::EO_LogicalOR },
};


/*[
	Internal functions for operation executions
 ]*/

// Performs a binary operation of a specified type on the given operands
static int ExecuteBinaryOperation(ActOfRose::Value::SValueReference* retValueRefHolder, ActOfRose::Operation::EOperationTypes opType,
	ActOfRose::Value::SValueReference* leftOperandRef, ActOfRose::Value::SValueReference* rightOperandRef)
{
	switch (opType)
	{
		case ActOfRose::Operation::EOperationTypes::EO_Assignment:
		{
			// ----- Checking if the operation type is the assignment and if left value is lvalue -----

			if (leftOperandRef->category != ActOfRose::Value::EValueCategories::EVC_LValue)
			{
				ActOfRose::WriteLog(PREF_STRING("lvalue required as left operand"),
					(sizeof(PREF_STRING("lvalue required as left operand")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
				
				return AOR_ERROR_EXEC_LVALUE_REQUIRED;
			}


			// ----- Execution -----

			delete *(leftOperandRef->value.valueHolder);

			if (rightOperandRef->category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				*(leftOperandRef->value.valueHolder) = nullptr;
			}
			else if (rightOperandRef->category == ActOfRose::Value::EValueCategories::EVC_LValue)
			{
				*(leftOperandRef->value.valueHolder) = ActOfRose::AORSystemCopyValue(*(rightOperandRef->value.valueHolder));
			}
			else
			{
				*(leftOperandRef->value.valueHolder) = ActOfRose::AORSystemCopyValue(rightOperandRef->value.value);
			}

			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_LValue;
			retValueRefHolder->value.valueHolder = leftOperandRef->value.valueHolder;

			break;
		}

		case ActOfRose::Operation::EOperationTypes::EO_LogicalAND:
		{
			if ((leftOperandRef->category == ActOfRose::Value::EValueCategories::EVC_None) ||
				(rightOperandRef->category == ActOfRose::Value::EValueCategories::EVC_None))
			{
				retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue(false);
			}
			else
			{
				retValueRefHolder->value.value = new ActOfRose::Value::CBooleanValue(
					(leftOperandRef->GetValue()->IsZero() == false) && (rightOperandRef->GetValue()->IsZero() == false));
			}

			retValueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;

			break;
		}

		default:
		{
			ActOfRose::Value::CValue* leftValue;
			if (leftOperandRef->category == ActOfRose::Value::EValueCategories::EVC_LValue)
			{
				leftValue = *(leftOperandRef->value.valueHolder);
			}
			else
			{
				leftValue = leftOperandRef->value.value;
			}

			return leftValue->ExecuteOperation(retValueRefHolder, opType, rightOperandRef);
		}
	}

	return AOR_SUCCESS;
}

// Performs a unary operation of a specified type on the given operand
static int ExecuteUnaryOperation(ActOfRose::Value::SValueReference* retValueRefHolder, ActOfRose::Operation::EOperationTypes opType,
	ActOfRose::Value::SValueReference* operandRef)
{
	switch (opType)
	{
		case ActOfRose::Operation::EOperationTypes::EO_Summation:
		{
			if (operandRef->category == ActOfRose::Value::EValueCategories::EVC_LValue)
			{
				retValueRefHolder->value.valueHolder = operandRef->value.valueHolder;
			}
			else
			{
				retValueRefHolder->value.value = operandRef->value.value;
			}

			retValueRefHolder->category = operandRef->category;

			break;
		}

		case ActOfRose::Operation::EOperationTypes::EO_Subtraction:
		{
			ActOfRose::Value::CValue* currValue;
			if (operandRef->category == ActOfRose::Value::EValueCategories::EVC_LValue)
			{
				currValue = *(operandRef->value.valueHolder);
			}
			else
			{
				currValue = operandRef->value.value;
			}

			switch (currValue->GetValueType())
			{
				case ActOfRose::Value::EValueType::EVT_Boolean:
				case ActOfRose::Value::EValueType::EVT_Integer:
				case ActOfRose::Value::EValueType::EVT_FloatingPoint:
				{
					break;
				}

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Invalid operand to unary expression"),
						(sizeof(PREF_STRING("Invalid operand to unary expression")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}
			}

			return operandRef->GetValue()->ExecuteOperation(retValueRefHolder, ActOfRose::Operation::EOperationTypes::EO_Negation, nullptr);
		}

		default:
		{
			return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
		}
	}

	return AOR_SUCCESS;
}


// Returns an operation type determined by string
bool ActOfRose::Operation::GetOperationType(ActOfRose::Operation::EOperationTypes* operationHolder, const std::string* str)
{
	auto it = sOperationMap.find(*str);
	if (it == sOperationMap.end())
	{
		return false;
	}
	
	*operationHolder = it->second;

	return true;
}

// Performs an operation of a specified type on the given operands
int ActOfRose::Operation::ExecuteOperation(ActOfRose::Value::SValueReference* retValueRefHolder, ActOfRose::Operation::EOperationTypes opType,
	ActOfRose::Value::SValueReference* leftValRef, ActOfRose::Value::SValueReference* rightValRef)
{
	int opExecResult;
	if (leftValRef->category == ActOfRose::Value::EValueCategories::EVC_None)
	{
		opExecResult = ExecuteUnaryOperation(retValueRefHolder, opType, rightValRef);
	}
	else
	{
		opExecResult = ExecuteBinaryOperation(retValueRefHolder, opType, leftValRef, rightValRef);
	}

	if (opExecResult != AOR_SUCCESS)
	{
		if (opExecResult == AOR_ERROR_EXEC_UNSUPPORTED_OPERATION)
		{
			ActOfRose::WriteLog(PREF_STRING("Unsupported operation"), (sizeof(PREF_STRING("Unsupported operation")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
		}
	}

	return opExecResult;
}
