/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Operation.h
	@Created:		31.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Operation-related elements. */

#ifndef __ACT_OF_ROSE_OPERATIONS_H__
#define __ACT_OF_ROSE_OPERATIONS_H__

#include <string>


namespace ActOfRose
{
	namespace Operation
	{
		// Types of operations
		enum class EOperationTypes
		{
			EO_Summation,				// Summation
			EO_Subtraction,				// Subtraction
			EO_Multiplication,			// Multiplication
			EO_Division,				// Division
			EO_Assignment				// Assignment
		};


		// Returns an operation type determined by string
		bool GetOperationType(ActOfRose::Operation::EOperationTypes* operationHolder, const std::string* str);

	} // !namespace Operation
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_OPERATIONS_H__
