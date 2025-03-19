/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of code for testing.
	
	@Name:			ValueTests.cpp
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Tests for methods of value classes. */

#include <iostream>

#include <Value/CIntegerValue.h>


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

	return 0;
}
