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
#include <vector>
#include <stack>

#include "Token.h"


namespace ActOfRose
{
	// Lexer class
	class CLexer
	{
	public:
		// Constructor
		CLexer(std::istream* scriptStream);


		/*[
			Functions for retrieving tokens
		 ]*/

	public:
		// Tokenises a chunk of script for execution. If the function returns 0, then there is nothing else to execute
		int Tokenise();
		// Analyses strings of a script and retrieves a token from them
		int RetrieveNextToken(ActOfRose::Token::SToken* newToken);

		// Returns a pointer to the array of retrieved tokens
		inline std::vector<ActOfRose::Token::SToken>* GetTokensRetrievedArr() { return & _mTokensRetrieved; }

	private:
		// Retrieves a token of the identifier type
		int RetrieveIdentifierOrKeywordToken(ActOfRose::Token::SToken* newToken);
		// Retrieves a token of the string type
		int RetrieveStringToken(ActOfRose::Token::SToken* newToken);
		// Retrieves a token of the number type
		int RetrieveNumberToken(ActOfRose::Token::SToken* newToken);
		// Retrieves a token with an operator
		int RetrieveOperatorToken(ActOfRose::Token::SToken* newToken);
		// Retrieves a token with a delimiter character
		int RetrieveDelimiterToken(ActOfRose::Token::SToken* newToken);

		// Skips a comment
		void SkipComment();

	private:
		std::istream* _pScriptStream;							// Pointer to a script stream
		std::vector<ActOfRose::Token::SToken> _mTokensRetrieved;		// Array of retrieved tokens
		std::stack<char> _mBlockDelimiterStack;					// Stack for collecting the starting (left) block delimiters and checking for the sequence of their use

	};

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_LEXER_CLASS_H__
