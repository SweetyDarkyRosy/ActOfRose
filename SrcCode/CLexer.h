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


namespace ActOfRose
{
	namespace Token
	{
		struct SToken;				// Token structure

	} // !namespace Token

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
		// Analyses strings of a script and retrieves a token from them
		int RetrieveNextToken(ActOfRose::Token::SToken* newToken);

	private:
		// Retrieves a token of the identifier type
		int RetrieveIdentifierToken(ActOfRose::Token::SToken* newToken);
		// Retrieves a token of the string type
		int RetrieveStringToken(ActOfRose::Token::SToken* newToken);

		// Skips a comment
		void SkipComment();

	private:
		std::istream* _pScriptStream;							// Pointer to a script stream

	};

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_LEXER_CLASS_H__
