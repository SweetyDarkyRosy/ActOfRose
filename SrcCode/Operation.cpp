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
static const std::unordered_map<std::string, ActOfRose::Operation::EOperations> sOperationMap = {
	{ "+", ActOfRose::Operation::EOperations::EO_Summation },
	{ "-", ActOfRose::Operation::EOperations::EO_Subtraction },
	{ "*", ActOfRose::Operation::EOperations::EO_Multiplication },
	{ "/", ActOfRose::Operation::EOperations::EO_Division },
	{ "=", ActOfRose::Operation::EOperations::EO_Assignment }
};
