/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Operation.cpp
	@Created:		31.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Operation.h"

#include <unordered_map>


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
