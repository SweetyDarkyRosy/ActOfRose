/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CFloatValue.h
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of floating-point type value. */

#ifndef __ACT_OF_ROSE_FLOATING_POINT_VALUE_CLASS_H__
#define __ACT_OF_ROSE_FLOATING_POINT_VALUE_CLASS_H__

#include "Value.h"


namespace ActOfRose
{
	namespace Value
	{
		// Class of floating-point type value (32-bit integer)
		class CFloatValue : public CValue
		{
		public:
			// Default constructor
			CFloatValue() :
				CValue(ActOfRose::Value::EValueType::EVT_FloatingPoint), _mValue(0.0f)
			{}

			// Constructor that takes an initial value
			CFloatValue(float value) :
				CValue(ActOfRose::Value::EValueType::EVT_FloatingPoint), _mValue(value)
			{}

		public:
			// Sets the single-precision floating-point value
			inline void SetRawValue(float value) { _mValue = value; }
			// Returns the raw 32-bit single-precision floating-point value
			inline float GetRawValue() const { return _mValue; }


			// Converts a value to a byte string
			virtual std::string ConvertValueToByteString() const override;
			// Converts a value to a wide string
			virtual std::wstring ConvertValueToWideString() const override;

		private:
			float _mValue;		// Raw 32-bit IEEE-754 single-precision floating-point value

		};

	} // !namespace Value
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_FLOATING_POINT_VALUE_CLASS_H__
