/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CCharValue.h
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of UTF-8-encoded character. */

#ifndef __ACT_OF_ROSE_CHARACTER_VALUE_CLASS_H__
#define __ACT_OF_ROSE_CHARACTER_VALUE_CLASS_H__

#include "Value.h"


namespace ActOfRose
{
	namespace Value
	{
		// Class of a single character (unicode) represented in UTF-8-encoded set of characters
		class CCharValue : public ActOfRose::Value::CValue
		{
		public:
			// Default constructor
			CCharValue() :
				CValue(ActOfRose::Value::EValueType::EVT_Character), _mChar{ 0, }
			{}

			// Constructor that takes an initial character set
			CCharValue(const char* characterSet);

		public:
			// Sets the character (character set / unicode)
			void SetRawCharSet(const char* srcCharSet);
			// Returns a pointer to the raw character set
			const char* GetRawCharSet() const { return _mChar; }


			// Converts a value to a byte string
			virtual std::string ConvertValueToByteString() const override;
			// Converts a value to a wide string
			virtual std::wstring ConvertValueToWideString() const override;


			// Performs an operation of a specified type with a current value and a given right value
			virtual int ExecuteOperation(ActOfRose::Value::SValueReference* retValueRefHolder,
				ActOfRose::Operation::EOperationTypes opType, ActOfRose::Value::SValueReference* rightValRef) override;

		private:
			char _mChar[5];		// Raw set of characters to form a unicode

		};

	} // !namespace Value
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_CHARACTER_VALUE_CLASS_H__
