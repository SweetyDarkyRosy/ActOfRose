/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of utility set source code.
	
	@Name:			StringConverting.cpp
	@Created:		13.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "StringConverting.h"

#include <iostream>
#include <ReturnCodes.h>


// Converts the given UTF-8-encoded null-terminated byte string to a UTF-16BE-encoded null-terminated wide string and returns a result
int ConvertStringUTF8ToUTF16BE(std::wstring* destUTF16BEStr, const char* srcUTF8Str, std::size_t srcLength)
{
	if ((destUTF16BEStr == nullptr) || (srcUTF8Str == nullptr) || (srcLength == 0))
	{
		return AOR_ERROR_INVALID_PARAMETER;
	}

	wchar_t utf16BEChar[3];
	const char* utf8StrWalker = srcUTF8Str;

	destUTF16BEStr->clear();

	while ((*utf8StrWalker != '\0') && (srcLength != 0))
	{
		if ((unsigned char)(*utf8StrWalker) < 0xF0)
		{
			if ((unsigned char)(*utf8StrWalker) < 0x80)
			{
				utf16BEChar[0] = (wchar_t)(utf8StrWalker[0]);

				utf8StrWalker++;
				srcLength--;
			}
			else if ((unsigned char)(*utf8StrWalker) < 0xE0)
			{
				if (srcLength < 2)
				{
					return AOR_ERROR_INVALID_CHARACTER;
				}

				utf16BEChar[0] = ((((wchar_t)(utf8StrWalker[0])) << 6) & 0x7C0) | (((wchar_t)(utf8StrWalker[1])) & 0x3F);

				utf8StrWalker += 2;
				srcLength -= 2;
			}
			else
			{
				if (srcLength < 3)
				{
					return AOR_ERROR_INVALID_CHARACTER;
				}

				utf16BEChar[0] = ((((wchar_t)(utf8StrWalker[0])) << 12) & 0xF000) | ((((wchar_t)(utf8StrWalker[1])) << 6) & 0xFC0) |
					(((wchar_t)(utf8StrWalker[2])) & 0x3F);
			
				utf8StrWalker += 3;
				srcLength -= 3;
			}

			utf16BEChar[1] = L'\0';
		}
		else
		{
			if (srcLength < 4)
			{
				return AOR_ERROR_INVALID_CHARACTER;
			}

			unsigned int unicode = ((((unsigned int)(utf8StrWalker[0])) << 18) & 0x1C0000) | ((((unsigned int)(utf8StrWalker[1])) << 12) & 0x3F000) |
				((((unsigned int)(utf8StrWalker[2])) << 6) & 0xFC0) | (((unsigned int)(utf8StrWalker[3])) & 0x3F);
			unicode = unicode - 0x10000;

			utf16BEChar[1] = (unicode & 0x3FF) | 0xDC00;
			unicode = unicode >> 10;
			utf16BEChar[0] = (unicode & 0x3FF) | 0xD800;

			utf16BEChar[2] = L'\0';

			utf8StrWalker += 4;
			srcLength -= 4;
		}

		destUTF16BEStr->append(utf16BEChar);
	}

	return AOR_SUCCESS;
}

// Converts the given UTF-8-encoded null-terminated byte string to a UTF-16BE-encoded null-terminated wide string and returns a result
int ConvertStringUTF8ToUTF16BE(std::wstring* destUTF16BEStr, const std::string* srcUTF8Str)
{
	return ConvertStringUTF8ToUTF16BE(destUTF16BEStr, srcUTF8Str->c_str(), srcUTF8Str->length());
}

// Converts the given UTF-16BE-encoded null-terminated wide string to a UTF-8-encoded null-terminated byte string and returns a result
int ConvertStringUTF16BEToUTF8(std::string* destUTF8Str, const wchar_t* srcUTF16BEStr, std::size_t srcLength)
{
	if ((destUTF8Str == nullptr) || (srcUTF16BEStr == nullptr))
	{
		return AOR_ERROR_INVALID_PARAMETER;
	}

	char utf8Char[5];
	const wchar_t* utf16BEStrWalker = srcUTF16BEStr;

	destUTF8Str->clear();

	while ((*utf16BEStrWalker != L'\0') && (srcLength != 0))
	{
		if ((*utf16BEStrWalker & 0xFC00) == 0xD800)
		{
			if (srcLength < 2)
			{
				return AOR_ERROR_INVALID_CHARACTER;
			}

			unsigned int unicode = ((((unsigned int)(utf16BEStrWalker[0])) << 10) & 0xFFC00) | (((unsigned int)(utf16BEStrWalker[1])) & 0x3FF);
			unicode = unicode + 0x10000;

			utf8Char[3] = (unicode & 0x3F) | 0x80;
			unicode = unicode >> 6;
			utf8Char[2] = (unicode & 0x3F) | 0x80;
			unicode = unicode >> 6;
			utf8Char[1] = (unicode & 0x3F) | 0x80;
			unicode = unicode >> 6;
			utf8Char[0] = (unicode & 0x7) | 0xF0;

			utf8Char[4] = '\0';

			utf16BEStrWalker += 2;
			srcLength -= 2;
		}
		else
		{
			if (*utf16BEStrWalker < 0x80)
			{
				utf8Char[0] = (char)(utf16BEStrWalker[0]);
				utf8Char[1] = '\0';
			}
			else if (*utf16BEStrWalker < 0x800)
			{
				utf8Char[1] = (((char)(utf16BEStrWalker[0])) & 0x3F) | 0x80;
				utf8Char[0] = (((char)(utf16BEStrWalker[0] >> 6)) & 0x1F) | 0xC0;
				utf8Char[2] = '\0';
			}
			else
			{
				utf8Char[2] = (((char)(utf16BEStrWalker[0])) & 0x3F) | 0x80;
				utf8Char[1] = (((char)(utf16BEStrWalker[0] >> 6)) & 0x3F) | 0x80;
				utf8Char[0] = (((char)(utf16BEStrWalker[0] >> 12)) & 0xF) | 0xE0;
				utf8Char[3] = '\0';
			}

			utf16BEStrWalker++;
			srcLength--;
		}

		destUTF8Str->append(utf8Char);
	}

	return AOR_SUCCESS;
}

// Converts the given UTF-16BE-encoded null-terminated wide string to a UTF-8-encoded null-terminated byte string and returns a result
int ConvertStringUTF16BEToUTF8(std::string* destUTF8Str, const std::wstring* srcUTF16BEStr)
{
	return ConvertStringUTF16BEToUTF8(destUTF8Str, srcUTF16BEStr->c_str(), srcUTF16BEStr->length());
}
