/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CArrayValue.h
	@Created:		19.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of dynamic array. */

#ifndef __ACT_OF_ROSE_ARRAY_VALUE_CLASS_H__
#define __ACT_OF_ROSE_ARRAY_VALUE_CLASS_H__

#include <vector>

#include "Value.h"


namespace ActOfRose
{
	namespace Value
	{
		// Class of dynamic array
		class CArrayValue : public ActOfRose::Value::CValue
		{
		public:
			// Constructor
			CArrayValue() :
				CValue(ActOfRose::Value::EValueType::EVT_Array), _mValueArray()
			{}

			// Constructor that takes another array and makes a copy
			CArrayValue(const ActOfRose::Value::CArrayValue* array);

			// Destructor
			~CArrayValue();

		public:
			// Adds a new value to an array
			inline void AddValue(ActOfRose::Value::CValue* newValue) { _mValueArray.push_back(newValue); }

			// Returns a pointer to the raw underlying array of values
			std::vector<ActOfRose::Value::CValue*>* GetRawArray() { return &_mValueArray; }


			// Converts the array to a byte string
			virtual std::string ConvertValueToByteString() const override;
			// Converts the array to a wide string
			virtual std::wstring ConvertValueToWideString() const override;

		private:
			std::vector<ActOfRose::Value::CValue*>					_mValueArray;	// Underlying array of values

		};

	} // !namespace Value
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_ARRAY_VALUE_CLASS_H__
