/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Keywords.cpp
	@Created:		17.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Keywords.h"

#include <unordered_map>


/*[
	Global variables
 ]*/

// Map of associations between null-terminated strings with keywords and the keyword classes
static const std::unordered_map<std::string, ActOfRose::Keyword::EKeywords> sKeywordMap = {
	{ "var", ActOfRose::Keyword::EKeywords::EK_Var },
	{ "const", ActOfRose::Keyword::EKeywords::EK_Const },
	{ "func", ActOfRose::Keyword::EKeywords::EK_Func },
	{ "if", ActOfRose::Keyword::EKeywords::EK_If },
	{ "elif", ActOfRose::Keyword::EKeywords::EK_Elif },
	{ "else", ActOfRose::Keyword::EKeywords::EK_Else },
	{ "for", ActOfRose::Keyword::EKeywords::EK_For },
	{ "while", ActOfRose::Keyword::EKeywords::EK_While },
	{ "do", ActOfRose::Keyword::EKeywords::EK_Do },
	{ "override", ActOfRose::Keyword::EKeywords::EK_Override },
	{ "strict", ActOfRose::Keyword::EKeywords::EK_Strict },
};


// Checks if the given string is a keyword
bool ActOfRose::Keyword::IsKeyword(const std::string* str)
{
	auto it = sKeywordMap.find(*str);
	if (it == sKeywordMap.end())
	{
		return false;
	}
	
	return true;
}

// Returns a keyword determined by string
bool ActOfRose::Keyword::GetKeyword(ActOfRose::Keyword::EKeywords* keywordHolder, const std::string* str)
{
	auto it = sKeywordMap.find(*str);
	if (it == sKeywordMap.end())
	{
		return false;
	}
	
	*keywordHolder = it->second;

	return true;
}
