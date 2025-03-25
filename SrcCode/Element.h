/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Element.h
	@Created:		25.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Element-related code. */

#ifndef __ACT_OF_ROSE_ELEMENTS_H__
#define __ACT_OF_ROSE_ELEMENTS_H__

namespace ActOfRose
{
	// Element types
	enum EElementType
	{
		EET_Variable,						// Variable
		EET_Constant,						// Constant
		EET_Function						// Function
	};

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_ELEMENTS_H__
