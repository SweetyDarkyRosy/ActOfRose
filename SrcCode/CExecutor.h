/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CExecutor.h
	@Created:		20.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of script executor. */

#ifndef __ACT_OF_ROSE_EXECUTOR_CLASS_H__
#define __ACT_OF_ROSE_EXECUTOR_CLASS_H__

#include <vector>

#include "Token.h"


namespace ActOfRose
{
	// Class of script executor
	class CExecutor
	{
	public:
		// Constructor
		CExecutor();

	public:
		// Executes an operation based on a token group pointed to by tokenGroup
		int Execute(std::vector<ActOfRose::Token::SToken>* tokenGroup);

	private:
		std::vector<ActOfRose::Token::SToken>* _pCurrTokenGroup;			// Pointer to a current array of retrieved tokens (token group)
		unsigned int _mCurrTokenIndex;								// Index of current token in the array

	};

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_EXECUTOR_CLASS_H__
