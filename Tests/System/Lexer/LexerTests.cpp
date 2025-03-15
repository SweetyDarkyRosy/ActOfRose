/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of code for testing.
	
	@Name:			LexerTests.cpp
	@Created:		15.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Tests for elements from the "CLexer.h" file. */

#include <sstream>
#include <vector>

#include <ReturnCodes.h>
#include <Log.h>
#include <Token.h>
#include <CLexer.h>
#include <Utility/StringMisc.h>


/*[
	Functions with test cases
 ]*/

bool TestIdentifierTokenisationA()
{
	ActOfRose::WriteLog(PREF_STRING("Testing of identifier tokenisation #1"),
		(sizeof(PREF_STRING("Testing of identifier tokenisation #1")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "_someToken", "length", "TEST" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTIdentifier,
		ActOfRose::Token::ETokenType::ETTIdentifier, ActOfRose::Token::ETokenType::ETTIdentifier };

	std::string srcString = "     	_someToken        length     TEST      ";
	std::istringstream strStream(srcString);
	ActOfRose::CLexer lexer(&strStream);

	std::vector<ActOfRose::Token::SToken> tokenArr;


	// ----- Retrieving -----

	{
		ActOfRose::Token::SToken token;
		while (lexer.RetrieveNextToken(&token) != AOR_TOKEN_END_OF_SCRIPT)
		{
			if (token.value == "\0")
			{
				continue;
			}

			tokenArr.push_back(token);

			token.value.clear();
		}
	}


	// ----- Checking -----

	if (tokenArr.size() != 3)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 3 identifiers"), (sizeof(PREF_STRING("There should be 3 identifiers")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong identidier value!"), (sizeof(PREF_STRING("Wrong identidier value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			break;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong identidier type!"), (sizeof(PREF_STRING("Wrong identidier type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			break;
		}
	}

	ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);

	return true;
}

bool TestIdentifierTokenisationB()
{
	ActOfRose::WriteLog(PREF_STRING("Testing of identifier tokenisation #2"),
		(sizeof(PREF_STRING("Testing of identifier tokenisation #2")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "SECOND", "__TEST__" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTIdentifier,
		ActOfRose::Token::ETokenType::ETTIdentifier };

	std::string srcString = "SECOND  __TEST__";
	std::istringstream strStream(srcString);
	ActOfRose::CLexer lexer(&strStream);

	std::vector<ActOfRose::Token::SToken> tokenArr;


	// ----- Retrieving -----

	{
		ActOfRose::Token::SToken token;
		while (lexer.RetrieveNextToken(&token) != AOR_TOKEN_END_OF_SCRIPT)
		{
			if (token.value == "\0")
			{
				continue;
			}

			tokenArr.push_back(token);

			token.value.clear();
		}
	}


	// ----- Checking -----

	if (tokenArr.size() != 2)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 2 identifiers"), (sizeof(PREF_STRING("There should be 3 identifiers")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong identidier value!"), (sizeof(PREF_STRING("Wrong identidier value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			break;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong identidier type!"), (sizeof(PREF_STRING("Wrong identidier type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			break;
		}
	}

	ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);

	return true;
}


// High-level entry point
int main(int argc, char* argv[])
{
	if (TestIdentifierTokenisationA() != true)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for tokenisation of identifiers #1 has failed"),
			(sizeof(PREF_STRING("Test for tokenisation of identifiers #1 has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}

	if (TestIdentifierTokenisationB() != true)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for tokenisation of identifiers #2 has failed"),
			(sizeof(PREF_STRING("Test for tokenisation of identifiers #2 has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}

	return 0;
}
