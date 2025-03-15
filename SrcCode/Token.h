/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Token.h
	@Created:		15.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Types of tokens and token structure. */

#ifndef __ACT_OF_ROSE_TOKEN_H__
#define __ACT_OF_ROSE_TOKEN_H__

namespace ActOfRose
{
	namespace Token
	{
		// Token types
		enum ETokenType
		{
			ETTIdentifier,						// Identifier
			ETTKeyword,							// Keyword
			ETTNumber,							// Number
			ETTString,							// String
			ETTSemicolon,						// Semicolon
			ETTDot,								// Dot
			ETTColon,							// Colon
			ETTComma,							// Comma
			ETTOperator,						// Operator
			ETTRoundBracketLeft,				// Left round bracket
			ETTRoundBracketRight,				// Right round bracket
			ETTCurlyBracketLeft,				// Left curly bracket
			ETTCurlyBracketRight				// Right curly bracket
		};

	} // !namespace Token
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_TOKEN_H__
