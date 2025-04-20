/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CFunction.cpp
	@Created:		19.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CFunction.h"

#include "ReturnCodes.h"
#include "CExecutor.h"


// ----- ActOfRose::CFunction class -----

// Executes a function with a specified set of parameters
int ActOfRose::CFunction::Execute(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::SValueReference>* params)
{
	return AOR_SUCCESS;
}
