/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CIntegerValue.h
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of integer type value. */

#ifndef __ACT_OF_ROSE_INTEGER_VALUE_CLASS_H__
#define __ACT_OF_ROSE_INTEGER_VALUE_CLASS_H__

#include "Value.h"


namespace ActOfRose
{
	namespace Value
	{
		// Class of integer type value (32-bit integer)
		class CIntegerValue : public ActOfRose::Value::CValue
		{
		public:
			// Default constructor
			CIntegerValue() :
				CValue(ActOfRose::Value::EValueType::EVT_Integer), _mValue(0)
			{}

			// Constructor that takes an initial value
			CIntegerValue(int value) :
				CValue(ActOfRose::Value::EValueType::EVT_Integer), _mValue(value)
			{}

		public:
			// Sets the integer value
			inline void SetRawValue(int value) { _mValue = value; }
			// Returns the raw 32-bit signed integer value
			inline int GetRawValue() const { return _mValue; }

			// Returns true if a value is non-zero
			virtual bool IsZero() const override { return (_mValue == 0); }


			// Converts a value to a byte string
			virtual std::string ConvertValueToByteString() const override;
			// Converts a value to a wide string
			virtual std::wstring ConvertValueToWideString() const override;


			// Performs an operation of a specified type with a current value and a given right value
			virtual int ExecuteOperation(ActOfRose::Value::SValueReference* retValueRefHolder,
				ActOfRose::Operation::EOperationTypes opType, ActOfRose::Value::SValueReference* rightValRef) override;

		private:
			int _mValue;		// Raw 32-bit signed integer value

		};

	} // !namespace Value
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_INTEGER_VALUE_CLASS_H__
