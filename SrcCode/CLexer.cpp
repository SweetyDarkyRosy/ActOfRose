/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CLexer.cpp
	@Created:		15.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CLexer.h"

#include "ReturnCodes.h"
#include "Token.h"


// Checks if the character is a non-printable character
inline bool IsWhitespace(char value)
{
	return ((value == '\t') || (value == ' ') || (value == 0xA) || (value == 0xD));
}


// ----- ActOfRose::CLexer class -----

// Constructor
ActOfRose::CLexer::CLexer(std::istream* scriptStream) :
	_pScriptStream(scriptStream)
{}


// Analyses strings of a script and retrieves a token from them
int ActOfRose::CLexer::RetrieveNextToken(ActOfRose::Token::SToken* newToken)
{
	while (_pScriptStream->eof() == false)
	{
		char retrievedChar = _pScriptStream->peek();

		if (IsWhitespace(retrievedChar) == true)
		{
			retrievedChar = _pScriptStream->get();
		}
		else
		{
			retrievedChar = _pScriptStream->get();
		}
	}

	return AOR_TOKEN_END_OF_SCRIPT;
}
