/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			ReturnCodes.h
	@Created:		13.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Return codes. */

#ifndef __ACT_OF_ROSE_ERROR_CODES_H__
#define __ACT_OF_ROSE_ERROR_CODES_H__

#define AOR_SUCCESS									0			// Code of successfull execution

#define AOR_TOKEN_END_OF_SCRIPT						1			// Code returned if the end of a script is reached

#define AOR_ERROR_INVALID_PARAMETER					(-1)		// Error due to invalid parameter/parameters
#define AOR_ERROR_INVALID_CHARACTER					(-2)		// Error due to invalid character (according to actual character/string encoding)
#define AOR_ERROR_SCRIPT_FILE_NOT_LOADED			(-3)		// Error due to invalid path to a file with a script or internal error happened while opening a file
#define AOR_ERROR_TOKEN_UNTERMINATED_STRING			(-4)		// Error due to unterminated/unquoted string when the end of a script is reached
#define AOR_ERROR_TOKEN_INVALID_ESCAPE_SEQUENCE		(-5)		// Error due to invalid escape sequence found while string tokenisation
#define AOR_ERROR_TOKEN_INVALID_NUMBER_VALUE		(-6)		// Error due to invalid number value


#endif // !__ACT_OF_ROSE_ERROR_CODES_H__
