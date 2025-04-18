/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CFunction.h
	@Created:		18.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of function. */

#ifndef __ACT_OF_ROSE_FUNCTION_CLASS_H__
#define __ACT_OF_ROSE_FUNCTION_CLASS_H__

#include <vector>
#include <string>

#include "Token.h"


namespace ActOfRose
{
	// Class of function
	class CFunction
	{
	public:
		// Constructor
		CFunction() {}

	public:
		// Adds a function parameter instance with a specific name
		inline void AddParameter(const char* paramName) { _mParamNames.push_back(paramName); }
		// Adds a token related to the body of a function
		inline void AddToken(ActOfRose::Token::SToken token) { _mTokens.push_back(token); }

	private:
		std::vector<const std::string> _mParamNames;				// Array of names of function parameters
		std::vector<ActOfRose::Token::SToken> _mTokens;				// Array of tokens inside function body

	};

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_FUNCTION_CLASS_H__
