/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CExecutor.cpp
	@Created:		20.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CExecutor.h"

#include "ReturnCodes.h"


// ----- ActOfRose::CExecutor class -----

// Constructor
ActOfRose::CExecutor::CExecutor()
{}


// Executes an operation based on a token group pointed to by tokenGroup
int ActOfRose::CExecutor::Execute(std::vector<ActOfRose::Token::SToken>* tokenGroup)
{
	_pCurrTokenGroup = tokenGroup;
	_mCurrTokenIndex = 0;

	return AOR_SUCCESS;
}
