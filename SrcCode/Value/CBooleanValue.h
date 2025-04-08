/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CBooleanValue.h
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of boolean value. */

#ifndef __ACT_OF_ROSE_BOOLEAN_VALUE_CLASS_H__
#define __ACT_OF_ROSE_BOOLEAN_VALUE_CLASS_H__

#include "Value.h"


namespace ActOfRose
{
	namespace Value
	{
		// Class of boolean type value
		class CBooleanValue : public ActOfRose::Value::CValue
		{
		public:
			// Default constructor
			CBooleanValue() :
				CValue(ActOfRose::Value::EValueType::EVT_Boolean), _mValue(false)
			{}

			// Constructor that takes an initial value
			CBooleanValue(bool value) :
				CValue(ActOfRose::Value::EValueType::EVT_Boolean), _mValue(value)
			{}

		public:
			// Sets the boolean value
			inline void SetRawValue(bool value) { _mValue = value; }
			// Returns the raw boolean value
			inline bool GetRawValue() const { return _mValue; }


			// Converts a value to a byte string
			virtual std::string ConvertValueToByteString() const override;
			// Converts a value to a wide string
			virtual std::wstring ConvertValueToWideString() const override;


			// Performs an operation of a specified type with a current value and a given right value
			virtual int ExecuteOperation(ActOfRose::Value::SValueReference* retValueRefHolder,
				ActOfRose::Operation::EOperationTypes opType, ActOfRose::Value::SValueReference* rightValRef) override;

		private:
			bool _mValue;		// Raw boolean value

		};

	} // !namespace Value
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_BOOLEAN_VALUE_CLASS_H__
