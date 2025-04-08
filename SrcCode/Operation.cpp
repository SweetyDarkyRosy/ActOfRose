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
#include "Value/Value.h"
#include "Utility/StringMisc.h"


/*[
	Global variables
 ]*/

// Map of associations between null-terminated strings with operations and the operation classes
static const std::unordered_map<std::string, ActOfRose::Operation::EOperationTypes> sOperationMap = {
	{ "+", ActOfRose::Operation::EOperationTypes::EO_Summation },
	{ "-", ActOfRose::Operation::EOperationTypes::EO_Subtraction },
	{ "*", ActOfRose::Operation::EOperationTypes::EO_Multiplication },
	{ "/", ActOfRose::Operation::EOperationTypes::EO_Division },
	{ "=", ActOfRose::Operation::EOperationTypes::EO_Assignment }
};


/*[
	Internal functions for operation executions
 ]*/

// Performs a binary operation of a specified type on the given operands
static int ExecuteBinaryOperation(ActOfRose::Value::SValueReference* retValueRefHolder, ActOfRose::Operation::EOperationTypes opType,
	ActOfRose::Value::SValueReference* leftOperandRef, ActOfRose::Value::SValueReference* rightOperandRef)
{
	// ----- Checking if the operation type is the assignment and if left value is lvalue -----

	switch (opType)
	{
		case ActOfRose::Operation::EOperationTypes::EO_Assignment:
		{
			if (leftOperandRef->category != ActOfRose::Value::EValueCategories::EVC_LValue)
			{
				ActOfRose::WriteLog(PREF_STRING("lvalue required as left operand"),
					(sizeof(PREF_STRING("lvalue required as left operand")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
				
				return AOR_ERROR_EXEC_LVALUE_REQUIRED;
			}
		}

		default:
		{
			break;
		}
	}


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

// Performs a unary operation of a specified type on the given operand
static int ExecuteUnaryOperation(ActOfRose::Value::SValueReference* retValueRefHolder, ActOfRose::Operation::EOperationTypes opType,
	ActOfRose::Value::SValueReference* operandRef)
{
	return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
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
