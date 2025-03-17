/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of code for testing.
	
	@Name:			LexerTests.cpp
	@Created:		15.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Tests for elements from the "CLexer.h" file. */

#include <sstream>
#include <vector>

#include <iostream>

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

	const char* testTokenValueArr[] = { "_someToken", "for", "while", "length", "TEST", "func" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTIdentifier,
		ActOfRose::Token::ETokenType::ETTKeyword, ActOfRose::Token::ETokenType::ETTKeyword,
		ActOfRose::Token::ETokenType::ETTIdentifier, ActOfRose::Token::ETokenType::ETTIdentifier,
		ActOfRose::Token::ETokenType::ETTKeyword };

	std::string srcString = "     	_someToken   		for while     length     TEST    func  ";
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

	if (tokenArr.size() != 6)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 6 tokens"), (sizeof(PREF_STRING("There should be 6 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
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

	const char* testTokenValueArr[] = { "SECOND", "var", "const", "__TEST__" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTIdentifier,
		ActOfRose::Token::ETokenType::ETTKeyword, ActOfRose::Token::ETokenType::ETTKeyword,
		ActOfRose::Token::ETokenType::ETTIdentifier };

	std::string srcString = "SECOND var			const		            \\\n __TEST__";
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

	if (tokenArr.size() != 4)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 4 tokens"), (sizeof(PREF_STRING("There should be 4 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}
	}

	ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);

	return true;
}

bool TestSkippingOfComment()
{
	ActOfRose::WriteLog(PREF_STRING("Testing skipping of comments"),
		(sizeof(PREF_STRING("Testing skipping of comments")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "TEST" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTIdentifier };

	std::string srcString = "         # This is just a comment  \n#ANOTHER COMMENT!		\nTEST";
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

	if (tokenArr.size() != 1)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 1 tokens"), (sizeof(PREF_STRING("There should be 3 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}
	}

	ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);

	return true;
}

bool TestStringTokenisation()
{
	ActOfRose::WriteLog(PREF_STRING("Testing string tokenisation"),
		(sizeof(PREF_STRING("Testing string tokenisation")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "Where is this string", "What \"to\" \ndo now?", "String\"#\"...\t" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTString,
		ActOfRose::Token::ETokenType::ETTString, ActOfRose::Token::ETokenType::ETTString };

	std::string srcString = "  \"Where is this string\"       \'What \"to\" \\n\\\n				do now?\' \"String\\\"#\\\"\\\n...\\t\"";
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
		ActOfRose::WriteLog(PREF_STRING("There should be 3 tokens"), (sizeof(PREF_STRING("There should be 3 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}
	}

	ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);

	return true;
}

bool TestStringTokenisationToFail()
{
	ActOfRose::WriteLog(PREF_STRING("Testing string tokenisation (for failures)"),
		(sizeof(PREF_STRING("Testing string tokenisation (for failures)")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "Where is this string" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTString };

	std::string srcString = "  \"Where is this string\'   			";
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

	if (tokenArr.size() != 1)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 1 tokens"), (sizeof(PREF_STRING("There should be 3 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}
	}

	return true;
}

bool TestNumberTokenisation()
{
	ActOfRose::WriteLog(PREF_STRING("Testing number tokenisation"),
		(sizeof(PREF_STRING("Testing number tokenisation")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "253", "4", "-233", "25331.2005", "-484.364", "+361444", ".034633", "-0.2" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTNumber,
		ActOfRose::Token::ETokenType::ETTNumber, ActOfRose::Token::ETokenType::ETTNumber, ActOfRose::Token::ETokenType::ETTNumber,
		ActOfRose::Token::ETokenType::ETTNumber, ActOfRose::Token::ETokenType::ETTNumber, ActOfRose::Token::ETokenType::ETTNumber,
		ActOfRose::Token::ETokenType::ETTNumber };

	std::string srcString = "253 	4    -233	\\\n   25331.2005 -484.364   +361444  		.034633  -0.2      ";
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

	if (tokenArr.size() != 8)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 8 tokens"), (sizeof(PREF_STRING("There should be 3 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}
	}

	ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);

	return true;
}

bool TestNumberTokenisationToFail()
{
	ActOfRose::WriteLog(PREF_STRING("Testing number tokenisation (for failures)"),
		(sizeof(PREF_STRING("Testing number tokenisation (for failures)")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "253", "4", "-233.246.4" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTNumber,
		ActOfRose::Token::ETokenType::ETTNumber, ActOfRose::Token::ETokenType::ETTNumber };

	std::string srcString = "253 	4    -233.246.4";
	std::istringstream strStream(srcString);
	ActOfRose::CLexer lexer(&strStream);

	std::vector<ActOfRose::Token::SToken> tokenArr;


	// ----- Retrieving -----

	{
		ActOfRose::Token::SToken token;
		while (lexer.RetrieveNextToken(&token) == AOR_SUCCESS)
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
		ActOfRose::WriteLog(PREF_STRING("There should be 3 tokens"), (sizeof(PREF_STRING("There should be 3 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}
	}

	return true;
}

bool TestOperatorTokenisation()
{
	ActOfRose::WriteLog(PREF_STRING("Testing operator tokenisation"),
		(sizeof(PREF_STRING("Testing operator tokenisation")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "--", "&=", "&&", "<", "+", ">", "==", "||", "=", "<=", "-", "*", "/", "/=", "++" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTOperator, ActOfRose::Token::ETokenType::ETTOperator,
		ActOfRose::Token::ETokenType::ETTOperator, ActOfRose::Token::ETokenType::ETTOperator, ActOfRose::Token::ETokenType::ETTOperator,
		ActOfRose::Token::ETokenType::ETTOperator, ActOfRose::Token::ETokenType::ETTOperator, ActOfRose::Token::ETokenType::ETTOperator,
		ActOfRose::Token::ETokenType::ETTOperator, ActOfRose::Token::ETokenType::ETTOperator, ActOfRose::Token::ETokenType::ETTOperator,
		ActOfRose::Token::ETokenType::ETTOperator, ActOfRose::Token::ETokenType::ETTOperator, ActOfRose::Token::ETokenType::ETTOperator,
		ActOfRose::Token::ETokenType::ETTOperator };

	std::string srcString = " --	 &= &&	 <  +  > \\\n==  || = <= - * / /= ++";
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

	if (tokenArr.size() != 15)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 15 tokens"), (sizeof(PREF_STRING("There should be 15 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}
	}

	ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);

	return true;
}

bool TestDelimiterTokenisation()
{
	ActOfRose::WriteLog(PREF_STRING("Testing delimiter tokenisation"),
		(sizeof(PREF_STRING("Testing delimiter tokenisation")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "{", "{", "(", ")", ";", "}", ",", ".", "}", "(", ",", ",", ".", ")" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTCurlyBracketLeft, ActOfRose::Token::ETokenType::ETTCurlyBracketLeft,
		ActOfRose::Token::ETokenType::ETTRoundBracketLeft, ActOfRose::Token::ETokenType::ETTRoundBracketRight, ActOfRose::Token::ETokenType::ETTSemicolon,
		ActOfRose::Token::ETokenType::ETTCurlyBracketRight, ActOfRose::Token::ETokenType::ETTComma, ActOfRose::Token::ETokenType::ETTDot,
		ActOfRose::Token::ETokenType::ETTCurlyBracketRight, ActOfRose::Token::ETokenType::ETTRoundBracketLeft, ActOfRose::Token::ETokenType::ETTComma,
		ActOfRose::Token::ETokenType::ETTComma, ActOfRose::Token::ETokenType::ETTDot, ActOfRose::Token::ETokenType::ETTRoundBracketRight };

	std::string srcString = " {   { ();}    , .			}	 (,,.)";
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

	if (tokenArr.size() != 14)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 14 tokens"), (sizeof(PREF_STRING("There should be 14 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}
	}

	ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);

	return true;
}

bool TestDelimiterTokenisationToFail()
{
	ActOfRose::WriteLog(PREF_STRING("Testing delimiter tokenisation (for failures)"),
		(sizeof(PREF_STRING("Testing delimiter tokenisation (for failures)")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Info);

	const char* testTokenValueArr[] = { "{", "{", "(", ";", "}" };
	const ActOfRose::Token::ETokenType testTokenTypeArr[] = { ActOfRose::Token::ETokenType::ETTCurlyBracketLeft, ActOfRose::Token::ETokenType::ETTCurlyBracketLeft,
		ActOfRose::Token::ETokenType::ETTRoundBracketLeft, ActOfRose::Token::ETokenType::ETTRoundBracketRight, ActOfRose::Token::ETokenType::ETTSemicolon,
		ActOfRose::Token::ETokenType::ETTCurlyBracketRight, ActOfRose::Token::ETokenType::ETTComma, ActOfRose::Token::ETokenType::ETTDot,
		ActOfRose::Token::ETokenType::ETTCurlyBracketRight, ActOfRose::Token::ETokenType::ETTRoundBracketLeft, ActOfRose::Token::ETokenType::ETTComma,
		ActOfRose::Token::ETokenType::ETTComma, ActOfRose::Token::ETokenType::ETTDot, ActOfRose::Token::ETokenType::ETTRoundBracketRight };

	std::string srcString = " {   { (;}";
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

	if (tokenArr.size() != 5)
	{
		ActOfRose::WriteLog(PREF_STRING("There should be 5 tokens"), (sizeof(PREF_STRING("There should be 5 tokens")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		return false;
	}

	for (unsigned int i = 0; i < (unsigned int)(tokenArr.size()); i++)
	{
		if (tokenArr[i].value.compare(testTokenValueArr[i]) != 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong value!"), (sizeof(PREF_STRING("Wrong value!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return false;
		}

		if (tokenArr[i].type != testTokenTypeArr[i])
		{
			ActOfRose::WriteLog(PREF_STRING("Wrong token type!"), (sizeof(PREF_STRING("Wrong token type!")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);
			return false;
		}
	}

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

	if (TestSkippingOfComment() != true)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for skipping of comments has failed"),
			(sizeof(PREF_STRING("Test for skipping of comments has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}

	if (TestStringTokenisation() != true)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for string tokenisation has failed"),
			(sizeof(PREF_STRING("Test for string tokenisation has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}

	if (TestStringTokenisationToFail() != false)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for string tokenisation with failure has failed"),
			(sizeof(PREF_STRING("Test for string tokenisation with failure has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}
	else
	{
		ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);
	}

	if (TestNumberTokenisation() != true)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for number tokenisation has failed"),
			(sizeof(PREF_STRING("Test for number tokenisation has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}

	if (TestNumberTokenisationToFail() != false)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for number tokenisation with failure has failed"),
			(sizeof(PREF_STRING("Test for number tokenisation with failure has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}
	else
	{
		ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);
	}

	if (TestOperatorTokenisation() != true)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for operator tokenisation has failed"),
			(sizeof(PREF_STRING("Test for operator tokenisation has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}

	if (TestDelimiterTokenisation() != true)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for delimiter tokenisation has failed"),
			(sizeof(PREF_STRING("Test for delimiter tokenisation has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}

	if (TestDelimiterTokenisationToFail() != false)
	{
		ActOfRose::WriteLog(PREF_STRING("Test for delimiter tokenisation with failure has failed"),
			(sizeof(PREF_STRING("Test for delimiter tokenisation with failure has failed")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return 1;
	}
	else
	{
		ActOfRose::WriteLog(PREF_STRING("PASSED\n"), (sizeof(PREF_STRING("PASSED\n")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Info);
	}

	return 0;
}
