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
#define AOR_ERROR_INVALID_ARG_NUMBER						(-2)		// Error due to some internal error
#define AOR_ERROR_INVALID_PARAMETER							(-3)		// Error due to invalid parameter/parameters
#define AOR_ERROR_INVALID_CHARACTER							(-4)		// Error due to invalid character (according to actual character/string encoding)
#define AOR_ERROR_SCRIPT_FILE_NOT_LOADED					(-5)		// Error due to invalid path to a file with a script or internal error happened while opening a file
#define AOR_ERROR_TOKEN_UNEXPECTED_TOKEN					(-6)		// Error due to unexpected token encountered
#define AOR_ERROR_TOKEN_UNTERMINATED_STRING					(-7)		// Error due to unterminated/unquoted string when the end of a script is reached
#define AOR_ERROR_TOKEN_INVALID_ESCAPE_SEQUENCE				(-8)		// Error due to invalid escape sequence found while string tokenisation
#define AOR_ERROR_TOKEN_INVALID_NUMBER_VALUE				(-9)		// Error due to invalid number value
#define AOR_ERROR_TOKEN_INVALID_ENDING_BLOCK_DELIMITER		(-10)		// Error due to invalid ending block delimiter encountered
#define AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT				(-11)		// Error due to premature end of script
#define AOR_ERROR_TOKEN_IDENTIFIER_EXPECTED					(-12)		// Error due to that identifier was expected while tokenisation
#define AOR_ERROR_CONTEXT_DETERMINATION_ERROR				(-13)		// Error due to the fact that the context could not be determined
#define AOR_ERROR_EXEC_IDENTIFIER_ALREADY_IN_USE			(-14)		// Error due to that a given identifier is already in use
#define AOR_ERROR_EXEC_NO_OPERATOR_CHILD					(-15)		// Error due to that an operator node has no child operand nodes
#define AOR_ERROR_EXEC_UNSUPPORTED_OPERATION				(-16)		// Error due to unsupported operation
#define AOR_ERROR_EXEC_NON_VALUE_TOKEN						(-17)		// Error due to trying to retrieve a value from a non-value token
#define AOR_ERROR_EXEC_EXPRESSION_EXPECTED					(-18)		// Error due to that expression was expected
#define AOR_ERROR_EXEC_UNDECLARED_IDENTIFIER				(-19)		// Error due to undeclared identifier
#define AOR_ERROR_EXEC_INVALID_EXPRESSION					(-20)		// Error due to an invalid expression
#define AOR_ERROR_EXEC_LVALUE_REQUIRED						(-21)		// Error due to invalid expression because of that lvalue was required

#endif // !__ACT_OF_ROSE_ERROR_CODES_H__
