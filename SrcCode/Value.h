/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Value.h
	@Created:		18.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Classes of values (different data types). */

#ifndef __ACT_OF_ROSE_VALUE_CLASS_H__
#define __ACT_OF_ROSE_VALUE_CLASS_H__

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

	} // !namespace Value
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_VALUE_CLASS_H__
