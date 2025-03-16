/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CLexer.cpp
	@Created:		15.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CLexer.h"

#include "ReturnCodes.h"
#include "Log.h"
#include "Token.h"
#include "Utility/StringConverting.h"


// Checks if the character is a non-printable character
inline bool IsWhitespace(char value)
{
	return ((value == '\t') || (value == ' ') || (value == 0xA) || (value == 0xD));
}

// Checks if the character value is an alphabetic character
inline bool IsAlphabetic(char value)
{
	return ((('A' <= value) && (value <= 'Z')) || (('a' <= value) && (value <= 'z')));
}

// Checks if the character value is a digit
inline bool IsDigit(char value)
{
	return (('0' <= value) && (value <= '9'));
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
		else if (retrievedChar == '#')
		{
			SkipComment();
		}
		else
		{
			int result = AOR_SUCCESS;

			if ((IsAlphabetic(retrievedChar) == true) || (retrievedChar == '_'))
			{
				result = RetrieveIdentifierToken(newToken);
			}
			else
			{
				retrievedChar = _pScriptStream->get();
			}

			return result;
		}
	}

	return AOR_TOKEN_END_OF_SCRIPT;
}


// Retrieves a token of the identifier type
int ActOfRose::CLexer::RetrieveIdentifierToken(ActOfRose::Token::SToken* newToken)
{
	while (_pScriptStream->eof() == false)
	{
		char retrievedChar = _pScriptStream->peek();

		if ((IsAlphabetic(retrievedChar) == true) || (IsDigit(retrievedChar) == true) || (retrievedChar == '_'))
		{
			retrievedChar = _pScriptStream->get();
			newToken->value += retrievedChar;
		}
		else
		{
			break;
		}
	}

	newToken->type = ActOfRose::Token::ETokenType::ETTIdentifier;

#ifdef _DEBUG
	{
		std::string logMsg = "New token (Identifier): " + newToken->value;
		ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
	}
#endif

	return AOR_SUCCESS;
}

// Skips a comment
void ActOfRose::CLexer::SkipComment()
{
	while (_pScriptStream->eof() == false)
	{
		char retrievedChar = _pScriptStream->get();

		if (retrievedChar == '\n')
		{
			return;
		}
	}
}
