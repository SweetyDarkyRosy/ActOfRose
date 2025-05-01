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
	namespace Value
	{
		struct SValueReference;				// Structure of value reference
	} // !namespace Value

	namespace Operation
	{
		// Types of operations
		enum class EOperationTypes
		{
			EO_Summation,						// Summation
			EO_Subtraction,						// Subtraction
			EO_Multiplication,					// Multiplication
			EO_Division,						// Division
			EO_Assignment,						// Assignment
			EO_Negation,						// Negation
			EO_ElementAccess,					// Access to an element through index
			EO_CampareEqualTo,					// Comparison operation (equal to)
			EO_CampareNotEqualTo,				// Comparison operation (not equal to)
			EO_CampareLessThan,					// Comparison operation (less than)
			EO_CampareGreaterThan,				// Comparison operation (greater than)
			EO_CampareLessThanOrEqualTo,		// Comparison operation (less than or equal to)
		};


		// Returns an operation type determined by string
		bool GetOperationType(ActOfRose::Operation::EOperationTypes* operationHolder, const std::string* str);

		// Performs an operation of a specified type on the specified operands
		int ExecuteOperation(ActOfRose::Value::SValueReference* retValueRefHolder, ActOfRose::Operation::EOperationTypes opType,
			ActOfRose::Value::SValueReference* leftValRef, ActOfRose::Value::SValueReference* rightValRef);

	} // !namespace Operation
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_OPERATIONS_H__
