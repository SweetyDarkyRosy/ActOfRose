/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			ReturnCodes.h
	@Created:		13.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Return codes. */

#ifndef __ACT_OF_ROSE_ERROR_CODES_H__
#define __ACT_OF_ROSE_ERROR_CODES_H__

#define AOR_SUCCESS											0			// Code of successfull execution

#define AOR_TOKEN_END_OF_SCRIPT								1			// Code returned if the end of a script is reached
#define AOR_CONTEXT_COMPLETE								2			// Code returned to indicate that current context is complete
#define AOR_CONTEXT_EXECUTE									3			// Code returned to indicate that current context is ready for execution
#define AOR_CONTEXT_CREATE									4			// Code returned to indicate that a new context should be created

#define AOR_ERROR_INTERNAL_ERROR							(-1)		// Error due to some internal error
#define AOR_ERROR_INVALID_PARAMETER							(-2)		// Error due to invalid parameter/parameters
#define AOR_ERROR_INVALID_CHARACTER							(-3)		// Error due to invalid character (according to actual character/string encoding)
#define AOR_ERROR_SCRIPT_FILE_NOT_LOADED					(-4)		// Error due to invalid path to a file with a script or internal error happened while opening a file
#define AOR_ERROR_TOKEN_UNEXPECTED_TOKEN					(-5)		// Error due to unexpected token encountered
#define AOR_ERROR_TOKEN_UNTERMINATED_STRING					(-6)		// Error due to unterminated/unquoted string when the end of a script is reached
#define AOR_ERROR_TOKEN_INVALID_ESCAPE_SEQUENCE				(-7)		// Error due to invalid escape sequence found while string tokenisation
#define AOR_ERROR_TOKEN_INVALID_NUMBER_VALUE				(-8)		// Error due to invalid number value
#define AOR_ERROR_TOKEN_INVALID_ENDING_BLOCK_DELIMITER		(-9)		// Error due to invalid ending block delimiter encountered
#define AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT				(-10)		// Error due to premature end of script
#define AOR_ERROR_TOKEN_IDENTIFIER_EXPECTED					(-11)		// Error due to that identifier was expected while tokenisation
#define AOR_ERROR_CONTEXT_DETERMINATION_ERROR				(-12)		// Error due to the fact that the context could not be determined
#define AOR_ERROR_EXEC_IDENTIFIER_ALREADY_IN_USE			(-13)		// Error due to that a given identifier is already in use

#endif // !__ACT_OF_ROSE_ERROR_CODES_H__
