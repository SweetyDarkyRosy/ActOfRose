/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Value.h
	@Created:		18.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Types of values and abstract class of value. */

#ifndef __ACT_OF_ROSE_VALUE_BASIS_H__
#define __ACT_OF_ROSE_VALUE_BASIS_H__

#include <string>

#include <Operation.h>


namespace ActOfRose
{
	namespace Value
	{
		// Value types / data types
		enum EValueType
		{
			EVT_Boolean = 0,							// Boolean
			EVT_Character = 1,							// Character
			EVT_Integer = 2,							// Integer number
			EVT_FloatingPoint = 3,						// Floating-point number
			EVT_String = 4,								// String
			EVT_Array = 5,								// Array
		};


		struct SValueReference;							// Structure of value reference


		// Abstract class of value / data type
		class CValue
		{
		public:
			// Default constructor
			CValue(ActOfRose::Value::EValueType type) :
				_mType(type)
			{}

			// Destructor
			virtual ~CValue() {}

		public:
			// Returns the type of a value
			inline ActOfRose::Value::EValueType GetValueType() const { return _mType; }

			// Returns true if a value is non-zero
			virtual bool IsZero() const = 0;


			// Returns a pointer to a null-terminated UTF-8-encoded string with type name
			inline const char* GetTypeByteString() const;
			// Returns a pointer to a null-terminated UTF-16BE-encoded string with type name
			inline const wchar_t* GetTypeWideString() const;


			// Converts a value to a byte string
			virtual std::string ConvertValueToByteString() const = 0;
			// Converts a value to a wide string
			virtual std::wstring ConvertValueToWideString() const = 0;


			// Performs an operation of a specified type with a current value and a given right value
			virtual int ExecuteOperation(ActOfRose::Value::SValueReference* retValueRefHolder,
				ActOfRose::Operation::EOperationTypes opType, ActOfRose::Value::SValueReference* rightValRef) = 0;

		protected:
			ActOfRose::Value::EValueType _mType;					// Value/data type

		};


		// Categories of values
		enum EValueCategories
		{
			EVC_None = -1,							// None (no value)
			EVC_PRValue = 0,						// prvalue
			EVC_RValue = 1,							// rvalue
			EVC_LValue = 2,							// lvalue
		};


		// Structure of value reference
		struct SValueReference
		{
			ActOfRose::Value::CValue* GetValue() const
			{
				if (category == ActOfRose::Value::EValueCategories::EVC_LValue)
				{
					return *(value.valueHolder);
				}
				else if (category == ActOfRose::Value::EValueCategories::EVC_None)
				{
					return nullptr;
				}
				else
				{
					return value.value;
				}
			}


			union
			{
				ActOfRose::Value::CValue* value;				// Pointer to a value
				ActOfRose::Value::CValue** valueHolder;			// Pointer to a value holder (pointer to a pointer to a value)
			} value;

			ActOfRose::Value::EValueCategories category;		// Value category
		};

	} // !namespace Value
} // !namespace ActOfRose


// ----- ActOfRose::Value::CValue class -----

// Returns a pointer to a null-terminated UTF-8-encoded string with type name
const char* ActOfRose::Value::CValue::GetTypeByteString() const
{
	static const char* valueTypeNames[] = { "Boolean", "Character", "Integer number", "Floating-point number", "String", "Array" };

	return valueTypeNames[(unsigned int)_mType];
}

// Returns a pointer to a null-terminated UTF-16BE-encoded string with type name
const wchar_t* ActOfRose::Value::CValue::GetTypeWideString() const
{
	static const wchar_t* valueTypeNames[] = { L"Boolean", L"Character", L"Integer number", L"Floating-point number", L"String", L"Array" };

	return valueTypeNames[(unsigned int)_mType];
}

#endif // !__ACT_OF_ROSE_VALUE_BASIS_H__
