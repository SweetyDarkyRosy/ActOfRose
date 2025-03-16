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
			else if ((retrievedChar == '\"') || (retrievedChar == '\''))
			{
				result = RetrieveStringToken(newToken);
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

// Retrieves a token of the string type
int ActOfRose::CLexer::RetrieveStringToken(ActOfRose::Token::SToken* newToken)
{
	newToken->type = ActOfRose::Token::ETokenType::ETTString;

	bool isBackslashFound = false;
	bool isSkipping = false;
	bool isStartedWithQuotation = false;

	if (_pScriptStream->peek() == '\"')
	{
		isStartedWithQuotation = true;
	}

	while (_pScriptStream->eof() == false)
	{
		if (isSkipping == true)
		{
			if (IsWhitespace(_pScriptStream->peek()) == true)
			{
				_pScriptStream->get();

				continue;
			}
			else
			{
				isSkipping = false;
			}
		}

		char retrievedChar = _pScriptStream->get();

		if (isBackslashFound == true)
		{
			isBackslashFound = false;

			if (IsAlphabetic(retrievedChar) == true)
			{
				if (retrievedChar == 'n')
				{
					newToken->value += '\n';

					continue;
				}
				else if (retrievedChar == 't')
				{
					newToken->value += '\t';

					continue;
				}
				else if (retrievedChar == 'v')
				{
					newToken->value += '\v';

					continue;
				}

				// A character just needs to be added
			}
			else if (retrievedChar == '\n')
			{
				isSkipping = true;

				continue;
			}
			else if ((retrievedChar == '\\') || (retrievedChar == '\'') || (retrievedChar == '\"') )
			{
				// A character just needs to be added
			}
			else if (retrievedChar == '\r')
			{
				if (_pScriptStream->peek() != '\n')
				{
					return AOR_ERROR_TOKEN_INVALID_ESCAPE_SEQUENCE;
				}

				retrievedChar = _pScriptStream->get();

				continue;
			}
			else
			{
				return AOR_ERROR_TOKEN_INVALID_ESCAPE_SEQUENCE;
			}
		}
		else
		{
			if (retrievedChar == '\"')
			{
				if (isStartedWithQuotation == true)
				{
					if (newToken->value.size() == 0)
					{
						continue;
					}
					else
					{
						{
							std::string logMsg = "New token (String): " + newToken->value;
							ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
						}

						return AOR_SUCCESS;
					}
				}
			}
			else if (retrievedChar == '\'')
			{
				if (isStartedWithQuotation == false)
				{
					if (newToken->value.size() == 0)
					{
						continue;
					}
					else
					{
						{
							std::string logMsg = "New token (String): " + newToken->value;
							ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
						}

						return AOR_SUCCESS;
					}
				}
			}
			else if (retrievedChar == '\n')
			{
				return AOR_ERROR_TOKEN_UNTERMINATED_STRING;
			}
			else if (retrievedChar == '\\')
			{
				isBackslashFound = true;

				continue;
			}
		}

		newToken->value += retrievedChar;
	}

	return AOR_ERROR_TOKEN_UNTERMINATED_STRING;
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
