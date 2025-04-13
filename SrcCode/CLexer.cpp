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
#include "Keywords.h"
#include "CSequencer.h"
#include "Utility/StringMisc.h"
#include "Utility/StringConverting.h"


// Checks if the character is a non-printable character
static inline bool IsWhitespace(char value)
{
	return ((value == '\t') || (value == ' ') || (value == 0xA) || (value == 0xD));
}

// Checks if the character value is an alphabetic character
static inline bool IsAlphabetic(char value)
{
	return ((('A' <= value) && (value <= 'Z')) || (('a' <= value) && (value <= 'z')));
}

// Checks if the character value is a digit
static inline bool IsDigit(char value)
{
	return (('0' <= value) && (value <= '9'));
}

// Checks if the character is a delimiter character
static inline bool IsDelimiter(char value)
{
	return ((value == ',') || (value == ';') || (value == ':') || (value == '(') || (value == ')') || (value == '{') || (value == '}'));
}

// Checks if the character is an operator character
static inline bool IsOperator(char value)
{
	return ((value == '+') || (value == '-') || (value == '*') || (value == '/') || (value == '%') || (value == '=') || (value == '<') || (value == '>') || (value == '&') || (value == '|'));
}


// ----- ActOfRose::CLexer class -----

// Constructor
ActOfRose::CLexer::CLexer(std::istream* scriptStream) :
	_pScriptStream(scriptStream)
{}


// Tokenises a chunk of script for execution. If the function returns 0, then there is nothing else to execute
int ActOfRose::CLexer::Tokenise()
{
	_mTokensRetrieved.clear();

	ActOfRose::Context::CSequencer sequencer;
	ActOfRose::Token::SToken token;
	int result;

	while ((result = RetrieveNextToken(&token)) == AOR_SUCCESS)
	{
		_mTokensRetrieved.push_back(token);

		result = sequencer.ProcessToken(&token);
		if (result != AOR_SUCCESS)
		{
			return result;
		}
	}

	if ((result == AOR_TOKEN_END_OF_SCRIPT) && (_mTokensRetrieved.size() != 0))
	{
		ActOfRose::WriteLog(PREF_STRING("Premature end of script"),
			(sizeof(PREF_STRING("Premature end of script")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT;
	}
	
	return result;
}

// Analyses strings of a script and retrieves a token from them
int ActOfRose::CLexer::RetrieveNextToken(ActOfRose::Token::SToken* newToken)
{
	newToken->value.clear();

	while (_pScriptStream->peek() != EOF)
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
				result = RetrieveIdentifierOrKeywordToken(newToken);
			}
			else if ((retrievedChar == '\"') || (retrievedChar == '\''))
			{
				result = RetrieveStringToken(newToken);
			}
			else if (IsDigit(retrievedChar) == true)
			{
				result = RetrieveNumberToken(newToken);
			}
			else if (IsOperator(retrievedChar) == true)
			{
				result = RetrieveOperatorToken(newToken);
			}
			else if (retrievedChar == '.')
			{
				retrievedChar = _pScriptStream->get();
				newToken->value = retrievedChar;

				if (IsDigit(_pScriptStream->peek()) == true)
				{
					result = RetrieveNumberToken(newToken);
				}
				else
				{
					newToken->type = ActOfRose::Token::ETokenType::ETTDot;
					
				#ifdef _DEBUG
					{
						std::string logMsg = "New token (Dot): " + newToken->value;
						ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
					}
				#endif
				}
			}
			else if (IsDelimiter(retrievedChar) == true)
			{
				result = RetrieveDelimiterToken(newToken);
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
int ActOfRose::CLexer::RetrieveIdentifierOrKeywordToken(ActOfRose::Token::SToken* newToken)
{
	while (_pScriptStream->peek() != EOF)
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

	if (ActOfRose::Keyword::IsKeyword(&(newToken->value)) == false)
	{
		newToken->type = ActOfRose::Token::ETokenType::ETTIdentifier;

	#ifdef _DEBUG
		std::string logMsg = "New token (Identifier): " + newToken->value;
		ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
	#endif
	}
	else
	{
		newToken->type = ActOfRose::Token::ETokenType::ETTKeyword;

	#ifdef _DEBUG
		std::string logMsg = "New token (Keyword): " + newToken->value;
		ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
	#endif
	}

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

	while (_pScriptStream->peek() != EOF)
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
					#ifdef _DEBUG
						{
							std::string logMsg = "New token (String): " + newToken->value;
							ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
						}
					#endif

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
					#ifdef _DEBUG
						{
							std::string logMsg = "New token (String): " + newToken->value;
							ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
						}
					#endif

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

// Retrieves a token of the number type
int ActOfRose::CLexer::RetrieveNumberToken(ActOfRose::Token::SToken* newToken)
{
	newToken->type = ActOfRose::Token::ETokenType::ETTNumber;

	bool isDotFound = false;

	if ((newToken->value.length() != 0) && (newToken->value[0] == '.'))
	{
		isDotFound = true;
	}

	while (_pScriptStream->peek() != EOF)
	{
		char retrievedChar = _pScriptStream->peek();

		if (IsDigit(retrievedChar) == true)
		{
			retrievedChar = _pScriptStream->get();
			newToken->value += retrievedChar;
		}
		else if (retrievedChar == '.')
		{
			if (isDotFound == false)
			{
				retrievedChar = _pScriptStream->get();
				newToken->value += retrievedChar;

				isDotFound = true;
			}
			else
			{
				ActOfRose::WriteLog(PREF_STRING("Extra dot has been found in a number while lexical tokenization"),
					(sizeof(PREF_STRING("Extra dot has been found in a number while lexical tokenization")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_INVALID_NUMBER_VALUE;
			}
		}
		else if ((IsWhitespace(retrievedChar) == true) || (IsDelimiter(retrievedChar) == true) || (IsOperator(retrievedChar) == true))
		{
			break;
		}
		else
		{
			ActOfRose::WriteLog(PREF_STRING("Invalid character has been found in a number lexical tokenization"),
				(sizeof(PREF_STRING("Invalid character has been found in a number lexical tokenization")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_TOKEN_INVALID_NUMBER_VALUE;
		}
	}

#ifdef _DEBUG
	{
		std::string logMsg = "New token (Number): " + newToken->value;
		ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
	}
#endif

	return AOR_SUCCESS;
}

// Retrieves a token with an operator
int ActOfRose::CLexer::RetrieveOperatorToken(ActOfRose::Token::SToken* newToken)
{
	newToken->type = ActOfRose::Token::ETokenType::ETTOperator;

	while (_pScriptStream->peek() != EOF)
	{
		char retrievedChar = _pScriptStream->peek();

		if (IsOperator(retrievedChar) == true)
		{
			if (newToken->value.length() == 0)
			{
				retrievedChar = _pScriptStream->get();
				newToken->value = retrievedChar;
			}
			else
			{
				if (retrievedChar == '=')
				{
					retrievedChar = _pScriptStream->get();
					newToken->value += retrievedChar;
				}
				else if ((retrievedChar == '+') && (newToken->value[0] == '+'))
				{
					retrievedChar = _pScriptStream->get();
					newToken->value += retrievedChar;
				}
				else if ((retrievedChar == '-') && (newToken->value[0] == '-'))
				{
					retrievedChar = _pScriptStream->get();
					newToken->value += retrievedChar;
				}
				else if ((retrievedChar == '&') && (newToken->value[0] == '&'))
				{
					retrievedChar = _pScriptStream->get();
					newToken->value += retrievedChar;
				}
				else if ((retrievedChar == '|') && (newToken->value[0] == '|'))
				{
					retrievedChar = _pScriptStream->get();
					newToken->value += retrievedChar;
				}
				else if ((retrievedChar == '<') && (newToken->value[0] == '<'))
				{
					retrievedChar = _pScriptStream->get();
					newToken->value += retrievedChar;
				}
				else if ((retrievedChar == '>') && (newToken->value[0] == '>'))
				{
					retrievedChar = _pScriptStream->get();
					newToken->value += retrievedChar;
				}

				break;
			}
		}
		else
		{
			break;
		}
	}

#ifdef _DEBUG
	{
		std::string logMsg = "New token (Operator): " + newToken->value;
		ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
	}
#endif

	return AOR_SUCCESS;
}

// Retrieves a token with a delimiter character
int ActOfRose::CLexer::RetrieveDelimiterToken(ActOfRose::Token::SToken* newToken)
{
	char retrievedChar = _pScriptStream->get();
	newToken->value = retrievedChar;

	if (retrievedChar == ';')
	{
		newToken->type = ActOfRose::Token::ETokenType::ETTSemicolon;

	#ifdef _DEBUG
		{
			std::string logMsg = "New token (Semicolon): " + newToken->value;
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif
	}
	else if (retrievedChar == ',')
	{
		newToken->type = ActOfRose::Token::ETokenType::ETTComma;

	#ifdef _DEBUG
		{
			std::string logMsg = "New token (Comma): " + newToken->value;
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif
	}
	else if (retrievedChar == ':')
	{
		newToken->type = ActOfRose::Token::ETokenType::ETTColon;

	#ifdef _DEBUG
		{
			std::string logMsg = "New token (Colon): " + newToken->value;
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif
	}
	else if (retrievedChar == '(')
	{
		newToken->type = ActOfRose::Token::ETokenType::ETTRoundBracketLeft;

	#ifdef _DEBUG
		{
			std::string logMsg = "New token (Left round bracket): " + newToken->value;
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif

		_mBlockDelimiterStack.push(retrievedChar);
	}
	else if (retrievedChar == ')')
	{
		if ((_mBlockDelimiterStack.empty() == true) || (_mBlockDelimiterStack.top() != '('))
		{
			ActOfRose::WriteLog(PREF_STRING("No block to close with a right round bracket"),
				(sizeof(PREF_STRING("No block to close with a right round bracket")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_TOKEN_INVALID_ENDING_BLOCK_DELIMITER;
		}

		newToken->type = ActOfRose::Token::ETokenType::ETTRoundBracketRight;

	#ifdef _DEBUG
		{
			std::string logMsg = "New token (Right round bracket): " + newToken->value;
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif

		_mBlockDelimiterStack.pop();
	}
	else if (retrievedChar == '{')
	{
		newToken->type = ActOfRose::Token::ETokenType::ETTCurlyBracketLeft;

	#ifdef _DEBUG
		{
			std::string logMsg = "New token (Left curly bracket): " + newToken->value;
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif

		_mBlockDelimiterStack.push(retrievedChar);
	}
	else if (retrievedChar == '}')
	{
		if ((_mBlockDelimiterStack.empty() == true) || (_mBlockDelimiterStack.top() != '{'))
		{
			ActOfRose::WriteLog(PREF_STRING("No block to close with a right curly bracket"),
				(sizeof(PREF_STRING("No block to close with a right curly bracket")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_TOKEN_INVALID_ENDING_BLOCK_DELIMITER;
		}

		newToken->type = ActOfRose::Token::ETokenType::ETTCurlyBracketRight;

	#ifdef _DEBUG
		{
			std::string logMsg = "New token (Right curly bracket): " + newToken->value;
			ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif

		_mBlockDelimiterStack.pop();
	}

	return AOR_SUCCESS;
}

// Skips a comment
void ActOfRose::CLexer::SkipComment()
{
	while (_pScriptStream->peek() != EOF)
	{
		char retrievedChar = _pScriptStream->get();

		if (retrievedChar == '\n')
		{
			return;
		}
	}
}
