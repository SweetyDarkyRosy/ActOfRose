/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CStringValue.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CStringValue.h"

#include <Utility/StringConverting.h>


// ----- ActOfRose::Value::CStringValue class -----

// Constructor that takes a C-style null-terminated byte string
ActOfRose::Value::CStringValue::CStringValue(const char* string) :
	CValue(ActOfRose::Value::EValueType::EVT_String)
{
	SetRawString(string);
}

// Constructor that takes another string value and makes a copy
ActOfRose::Value::CStringValue::CStringValue(const ActOfRose::Value::CStringValue* string) :
	CValue(ActOfRose::Value::EValueType::EVT_String), _mRawString(string->_mRawString), _mUnicodeIndices(string->_mUnicodeIndices)
{}

// Sets the UTF-8-encoded null-terminated byte string
void ActOfRose::Value::CStringValue::SetRawString(const char* string)
{
	_mRawString = string;

	_mUnicodeIndices.clear();

	unsigned int currentIndex = 0;
	const char* srcStrWalker = string;

	while (*srcStrWalker != '\0')
	{
		_mUnicodeIndices.emplace_back(currentIndex);

		if ((unsigned char)(*srcStrWalker) < 0x80)
		{
			currentIndex++;
			srcStrWalker++;
		}
		else if ((unsigned char)(*srcStrWalker) < 0xE0)
		{
			currentIndex += 2;
			srcStrWalker += 2;
		}
		else if ((unsigned char)(*srcStrWalker) < 0xF0)
		{
			currentIndex += 3;
			srcStrWalker += 3;
		}
		else
		{
			currentIndex += 4;
			srcStrWalker += 4;
		}
	}
}

// Converts a value to a byte string
std::string ActOfRose::Value::CStringValue::ConvertValueToByteString() const
{
	return std::string(_mRawString);
}

// Converts a value to a wide string
std::wstring ActOfRose::Value::CStringValue::ConvertValueToWideString() const
{
	std::wstring utf16BEStr;

	ConvertStringUTF8ToUTF16BE(&utf16BEStr, _mRawString.c_str(), _mRawString.length());

	return utf16BEStr;
}
