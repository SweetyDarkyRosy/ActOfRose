/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of code for testing.
	
	@Name:			StringConvertingTests.cpp
	@Created:		13.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Tests for elements from the "StringConverting.h" file. */

#include <iostream>

#include <ErrorCodes.h>
#include <Utility/StringConverting.h>


// High-level entry point
int main(int argc, char* argv[])
{
	// ----- UTF-8 to UTF-16BE -----

	// Test to be passed
	{
		std::string utf8Str(u8"This is just a string | Это просто строка");
		std::wstring utf16BEStr;

		ConvertStringUTF8ToUTF16BE(&utf16BEStr, &utf8Str);

		if (utf16BEStr.compare(L"This is just a string | Это просто строка") == 0)
		{
			std::cout << "Test #1 for ConvertStringUTF8ToUTF16BE() has been passed\n";
		}
		else
		{
			std::cout << "Test #1 for ConvertStringUTF8ToUTF16BE() has not been passed...\n";
		}
	}

	// Test to be failed
	{
		std::string utf8Str = "\xD0\xAD\xD1\x82\xD0\xB0\x20\xD1\x81\xD1\x82\xD1\x80\xD0\xBE\xD0\xBA\xD0\xB0\x20\xD1\x81\xD0\xBB\xD0\xBE\xD0\x00"; // "Эта строка слом<повр. символ>"
		std::wstring utf16BEStr;

		if (ConvertStringUTF8ToUTF16BE(&utf16BEStr, &utf8Str) != AOR_ERROR_SUCCESS)
		{
			std::cout << "Test #2 for ConvertStringUTF8ToUTF16BE() has been passed\n";
		}
		else
		{
			std::cout << "Test #2 for ConvertStringUTF8ToUTF16BE() has not been passed...\n";
		}
	}


	// ----- UTF-16BE to UTF-8-----

	// Test to be passed
	{
		std::wstring utf16BEStr(L"This is just a string | Это просто строка");
		std::string utf8Str;

		ConvertStringUTF16BEToUTF8(&utf8Str, &utf16BEStr);

		if (utf8Str.compare(u8"This is just a string | Это просто строка") == 0)
		{
			std::cout << "Test #1 for ConvertStringUTF16BEToUTF8() has been passed\n";
		}
		else
		{
			std::cout << "Test #1 for ConvertStringUTF16BEToUTF8() has not been passed...\n";
		}
	}

	// Test to be failed
	{
		std::wstring utf16BEStr = L"\x041E\x0448\x0438\x0431\x043A\x0430\x0020\xD801\xDC37\xD852\x00"; // "Ошибка 𐐷<повр. символ>"
		std::string utf8Str;

		if (ConvertStringUTF16BEToUTF8(&utf8Str, &utf16BEStr) != AOR_ERROR_SUCCESS)
		{
			std::cout << "Test #2 for ConvertStringUTF16BEToUTF8() has been passed\n";
		}
		else
		{
			std::cout << "Test #2 for ConvertStringUTF16BEToUTF8() has not been passed...\n";
		}
	}

	return 0;
}
