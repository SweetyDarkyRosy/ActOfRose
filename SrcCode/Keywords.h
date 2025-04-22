/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Keywords.h
	@Created:		17.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Set of keywords. */

#ifndef __ACT_OF_ROSE_KEYWORDS_H__
#define __ACT_OF_ROSE_KEYWORDS_H__

#include <string>


namespace ActOfRose
{
	namespace Keyword
	{
		// Keyword classes
		enum class EKeywords
		{
			EK_Var,				// "var" keyword
			EK_Const,			// "const" keyword
			EK_Func,			// "func" keyword
			EK_If,				// "if" keyword
			EK_Elif,			// "elif" keyword
			EK_Else,			// "else" keyword
			EK_For,				// "for" keyword
			EK_While,			// "while" keyword
			EK_Do,				// "do" keyword
			EK_Override,		// "override" keyword
			EK_Strict,			// "strict" keyword
			EK_Return,			// "return" keyword
			EK_Local,			// "local" keyword
		};


		// Checks if the given string is a keyword
		bool IsKeyword(const std::string* str);

		// Returns a keyword determined by string
		bool GetKeyword(ActOfRose::Keyword::EKeywords* keywordHolder, const std::string* str);

	} // !namespace Keyword
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_KEYWORDS_H__
