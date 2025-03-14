/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of utility set source code.
	
	@Name:			StringMisc.h
	@Created:		14.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Miscellaneous code for working with strings. */

#ifndef __ACT_OF_ROSE_UTILITY_C_STYLE_STRING_MISCELLANEOUS_H__
#define __ACT_OF_ROSE_UTILITY_C_STYLE_STRING_MISCELLANEOUS_H__

#if defined (WIN32) || defined (_WIN32)
	typedef wchar_t							PChar;			// Preferred character data type
#elif defined (__linux__)
	typedef char							PChar;			// Preferred character data type
#endif

#endif // !__ACT_OF_ROSE_UTILITY_C_STYLE_STRING_MISCELLANEOUS_H__
