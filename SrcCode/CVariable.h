/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CVariable.h
	@Created:		25.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of variable. */

#ifndef __ACT_OF_ROSE_VARIABLE_CLASS_H__
#define __ACT_OF_ROSE_VARIABLE_CLASS_H__

#include "SystemAPI.h"
#include "Value/Value.h"


namespace ActOfRose
{
	namespace Value
	{
		class CValue;				// Abstract class of value / data type
	} // !namespace Value

	// Class of variable
	class CVariable
	{
	public:
		// Default constructor
		inline CVariable() :
			_pValue(nullptr)
		{}

		// Move constructor
		CVariable(ActOfRose::CVariable&& existingValue) :
			_pValue(std::move(existingValue._pValue))
		{}


		// Constructor that takes a pointer to an existing value
		inline CVariable(ActOfRose::Value::CValue* existingValue) :
			_pValue(existingValue)
		{}


		// Destructor
		inline ~CVariable()
		{
			if (_pValue != nullptr)
			{
				delete _pValue;
			}
		}

	public:
		// Returns the value as the current one
		void SetValue(ActOfRose::Value::CValue* newValue)
		{
			if (_pValue != nullptr)
			{
				delete _pValue;
			}

			_pValue = newValue;
		}

		// Returns a pointer to an underlying value
		inline ActOfRose::Value::CValue* GetValue() { return _pValue; }
		// Returns a pointer to a local value holder
		inline ActOfRose::Value::CValue** GetValueHolder() { return &_pValue; }

	private:
		ActOfRose::Value::CValue* _pValue;				// Underlying value

	};

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_VARIABLE_CLASS_H__
