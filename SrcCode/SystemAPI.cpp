/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			SystemAPI.cpp
	@Created:		25.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "SystemAPI.h"

#include <string>
#include <map>

#include "Element.h"


/*[
	Global variables
 ]*/

extern std::map<std::string, ActOfRose::SElement>		gIdentifierMap;		// Map of associations between identifiers and elements


// Checks if the identifier is already in use
bool ActOfRose::AORSystemIsIdentifierUsed(const char* identifier)
{
	std::map<std::string, ActOfRose::SElement>::iterator identifierIt = gIdentifierMap.find(identifier);

	if (identifierIt == gIdentifierMap.end())
	{
		return false;
	}

	return true;
}
