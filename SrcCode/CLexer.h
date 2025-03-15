/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CLexer.h
	@Created:		15.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Lexer class. */

#ifndef __ACT_OF_ROSE_LEXER_CLASS_H__
#define __ACT_OF_ROSE_LEXER_CLASS_H__

#include <istream>

#include "Token.h"


namespace ActOfRose
{
	// Lexer class
	class CLexer
	{
	public:
		// Constructor
		CLexer(std::istream* scriptStream);

	public:
		// Analyses strings of a script and retrieves a token from them
		int RetrieveNextToken(ActOfRose::Token::SToken* newToken);

	private:
		std::istream* _pScriptStream;							// Pointer to a script stream

	};

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_LEXER_CLASS_H__
