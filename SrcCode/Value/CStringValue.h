/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CStringValue.h
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of UTF-8-encoded character. */

#ifndef __ACT_OF_ROSE_STRING_VALUE_CLASS_H__
#define __ACT_OF_ROSE_STRING_VALUE_CLASS_H__

#include <vector>

#include "Value.h"


namespace ActOfRose
{
	namespace Value
	{
		// Class of string (UTF-8-encoded)
		class CStringValue : public CValue
		{
		public:
			// Default constructor
			CStringValue() :
				CValue(ActOfRose::Value::EValueType::EVT_String), _mRawString(), _mUnicodeIndices()
			{}

			// Constructor that takes a C-style null-terminated byte string
			CStringValue(const char* string);

		public:
			// Sets the UTF-8-encoded null-terminated byte string
			void SetRawString(const char* string);
			// Returns a pointer to the underlying raw UTF-8-encoded null-terminated byte string
			const char* GetRawString() const { return _mRawString.c_str(); }


			// Converts the string to a byte string
			virtual std::string ConvertValueToByteString() const override;
			// Converts the string to a wide string
			virtual std::wstring ConvertValueToWideString() const override;

		private:
			std::string _mRawString;						// Raw set of characters to form a unicode
			std::vector<unsigned int> _mUnicodeIndices;		// Dynamic array of indices of unicodes in the UTF-8-encoded null-terminated byte string

		};

	} // !namespace Value
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_STRING_VALUE_CLASS_H__
