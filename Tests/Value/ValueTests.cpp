/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of code for testing.
	
	@Name:			ValueTests.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Tests for methods of value classes. */

#include <iostream>

#include <Value/CIntegerValue.h>
#include <Value/CFloatValue.h>
#include <Value/CBooleanValue.h>
#include <Value/CCharValue.h>
#include <Value/CStringValue.h>


// High-level entry point
int main(int argc, char* argv[])
{
	// ----- Integer value -----

	{
		ActOfRose::Value::CIntegerValue intValue(352);

		std::string utf8Str = intValue.ConvertValueToByteString();
		std::wstring utf16BEStr = intValue.ConvertValueToWideString();


		if (utf8Str.compare("352") == 0)
		{
			std::cout << "Test for intValue.ConvertValueToByteString() has been passed\n";
		}
		else
		{
			std::cout << "Test for intValue.ConvertValueToByteString() has not been passed...\n";
			return 1;
		}

		if (utf16BEStr.compare(L"352") == 0)
		{
			std::cout << "Test for intValue.ConvertValueToWideString() has been passed\n";
		}
		else
		{
			std::cout << "Test for intValue.ConvertValueToWideString() has not been passed...\n";
			return 1;
		}
	}


	// ----- Floating-point value -----

	{
		ActOfRose::Value::CFloatValue floatValue(144.63f);

		std::string utf8Str = floatValue.ConvertValueToByteString();
		std::wstring utf16BEStr = floatValue.ConvertValueToWideString();


		if (utf8Str.compare(0, 6, "144.63") == 0)
		{
			std::cout << "Test for floatValue.ConvertValueToByteString() has been passed\n";
		}
		else
		{
			std::cout << "Test for floatValue.ConvertValueToByteString() has not been passed...\n";
			return 1;
		}

		if (utf16BEStr.compare(0, 6, L"144.63") == 0)
		{
			std::cout << "Test for floatValue.ConvertValueToWideString() has been passed\n";
		}
		else
		{
			std::cout << "Test for floatValue.ConvertValueToWideString() has not been passed...\n";
			return 1;
		}
	}

	// ----- Boolean value -----

	{
		ActOfRose::Value::CBooleanValue booleanValue(false);

		std::string utf8Str = booleanValue.ConvertValueToByteString();
		if (utf8Str.compare("False") == 0)
		{
			std::cout << "Test for booleanValue.ConvertValueToByteString() has been passed\n";
		}
		else
		{
			std::cout << "Test for booleanValue.ConvertValueToByteString() has not been passed...\n";
			return 1;
		}

		booleanValue.SetRawValue(true);

		std::wstring utf16BEStr = booleanValue.ConvertValueToWideString();
		if (utf16BEStr.compare(L"True") == 0)
		{
			std::cout << "Test for booleanValue.ConvertValueToWideString() has been passed\n";
		}
		else
		{
			std::cout << "Test for booleanValue.ConvertValueToWideString() has not been passed...\n";
			return 1;
		}
	}

	// ----- Character -----

	{
		ActOfRose::Value::CCharValue charValue("П");

		std::string utf8Str = charValue.ConvertValueToByteString();
		if (utf8Str.compare("П") == 0)
		{
			std::cout << "Test for charValue.ConvertValueToByteString() has been passed\n";
		}
		else
		{
			std::cout << "Test for charValue.ConvertValueToByteString() has not been passed...\n";
			return 1;
		}

		charValue.SetRawCharSet("𐍈");

		std::wstring utf16BEStr = charValue.ConvertValueToWideString();
		if (utf16BEStr.compare(L"\xD800\xDF48") == 0)				// "𐍈"
		{
			std::cout << "Test for charValue.ConvertValueToWideString() has been passed\n";
		}
		else
		{
			std::cout << "Test for charValue.ConvertValueToWideString() has not been passed...\n";
			return 1;
		}
	}

	// ----- String -----

	{
		ActOfRose::Value::CStringValue testString("Где же мой чай?");

		std::string utf8Str = testString.ConvertValueToByteString();
		if (utf8Str.compare("Где же мой чай?") == 0)
		{
			std::cout << "Test for testString.ConvertValueToByteString() has been passed\n";
		}
		else
		{
			std::cout << "Test for testString.ConvertValueToByteString() has not been passed...\n";
			return 1;
		}

		testString.SetRawString("𐍈 / Символ...");

		std::wstring utf16BEStr = testString.ConvertValueToWideString();
		if (utf16BEStr.compare(L"\xD800\xDF48\x0020\x002F\x0020\x0421\x0438\x043C\x0432\x043E\x043B\x002E\x002E\x002E") == 0)				// "𐍈 / Символ..."
		{
			std::cout << "Test for testString.ConvertValueToWideString() has been passed\n";
		}
		else
		{
			std::cout << "Test for testString.ConvertValueToWideString() has not been passed...\n";
			return 1;
		}
	}

	return 0;
}
