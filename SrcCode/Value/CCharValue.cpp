/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CCharValue.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CCharValue.h"

#include <Utility/StringConverting.h>


// ----- ActOfRose::Value::CCharValue class -----

// Constructor that takes an initial character set
ActOfRose::Value::CCharValue::CCharValue(const char* characterSet) :
	CValue(ActOfRose::Value::EValueType::EVT_Character)
{
	SetRawCharSet(characterSet);
}

// Sets the boolean value
void ActOfRose::Value::CCharValue::SetRawCharSet(const char* srcCharSet)
{
	const char* srcCharSetWalker = srcCharSet;
	char* destCharSetWalker = _mChar;

	while (*srcCharSetWalker != '\0')
	{
		*destCharSetWalker = *srcCharSetWalker;

		srcCharSetWalker++;
		destCharSetWalker++;
	}

	*destCharSetWalker = '\0';
}

// Converts a value to a byte string
std::string ActOfRose::Value::CCharValue::ConvertValueToByteString() const
{
	return std::string(_mChar);
}

// Converts a value to a wide string
std::wstring ActOfRose::Value::CCharValue::ConvertValueToWideString() const
{
	std::wstring utf16BEStr;

	ConvertStringUTF8ToUTF16BE(&utf16BEStr, _mChar, 5);

	return utf16BEStr;
}
