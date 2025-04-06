/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			SystemAPI.h
	@Created:		25.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	High-level API of script execution system. */

#ifndef __ACT_OF_ROSE_SYSTEM_API_H__
#define __ACT_OF_ROSE_SYSTEM_API_H__

#include "Element.h"


namespace ActOfRose
{
	namespace Token
	{
		struct SToken;			// Token structure
	} // !namespace Token

	namespace Value
	{
		class CValue;			// Abstract class of value / data type
	} // !namespace Value


	/*[
		Functions for working with elements
	 ]*/

	// Registers an identifier and builds an association with an element (variable, constant or function)
	ActOfRose::SElement* AORSystemRegisterIdentifierAndElement(const char* identifier, ActOfRose::EElementType type, void* addr);

	// Checks if the identifier is already in use
	bool AORSystemIsIdentifierUsed(const char* identifier);

	// Returns a pointer to a block of information about registered element by the given identifier if it exists
	ActOfRose::SElement* AORSystemGetElementByIdentifier(const char* identifier);


	/*[
		Functions for working with values
	 ]*/
	
	// Creates a value from token and saves it to a value holder pointed to by valueHolder
	int CreateValueFromToken(ActOfRose::Value::CValue** valueHolder, ActOfRose::Token::SToken* token);

	// Creates a copy of a value pointed to by originalValue and returns a pointer to the copy
	ActOfRose::Value::CValue* CopyValue(ActOfRose::Value::CValue* originalValue);

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_SYSTEM_API_H__
