/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			BuiltInFuncScript.h
	@Created:		29.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Script-related built-in functions. */

#ifndef __ACT_OF_ROSE_BUILTIN_FUNCTIONS_SCRIPT_RELATED_H__
#define __ACT_OF_ROSE_BUILTIN_FUNCTIONS_SCRIPT_RELATED_H__

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
		// Executes a script of ActOfRose-specific format located at a specified path
		int Act(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params);

	} // !namespace BuiltIn
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_BUILTIN_FUNCTIONS_SCRIPT_RELATED_H__
