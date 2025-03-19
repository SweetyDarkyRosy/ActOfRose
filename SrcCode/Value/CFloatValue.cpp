/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CFloatValue.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CFloatValue.h"

#include <Utility/StringConverting.h>


// ----- ActOfRose::Value::CFloatValue class -----

// Converts a value to a byte string
std::string ActOfRose::Value::CFloatValue::ConvertValueToByteString() const
{
	return std::to_string(_mValue);
}

// Converts a value to a wide string
std::wstring ActOfRose::Value::CFloatValue::ConvertValueToWideString() const
{
	std::string utf8Str = std::to_string(_mValue);
	std::wstring utf16BEStr;

	ConvertStringUTF8ToUTF16BE(&utf16BEStr, &utf8Str);

	return utf16BEStr;
}
