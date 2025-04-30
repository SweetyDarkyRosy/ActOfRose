/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			BuiltInFuncOS.h
	@Created:		22.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	OS-related built-in functions. */

#ifndef __ACT_OF_ROSE_BUILTIN_FUNCTIONS_OS_RELATED_H__
#define __ACT_OF_ROSE_BUILTIN_FUNCTIONS_OS_RELATED_H__

#include <vector>


namespace ActOfRose
{
	namespace Value
	{
		class CValue;						// Abstract class of value / data type
		struct SValueReference;				// Structure of value reference
	}

	namespace BuiltIn
	{
		// Prints a desired value to console
		int Print(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params);

		// Executes a command line
		int Execute(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params);

		// Checks the given OS name and returns true if current host OS is equal to a specified one. Otherwise, the function returns false
		int IsCurrentOS(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params);

	} // !namespace BuiltIn
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_BUILTIN_FUNCTIONS_OS_RELATED_H__
