/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			SystemAPI.h
	@Created:		25.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	High-level API of script execution system. */

#ifndef __ACT_OF_ROSE_SYSTEM_API_H__
#define __ACT_OF_ROSE_SYSTEM_API_H__

namespace ActOfRose
{
	/*[
		Functions for working with elements
	 ]*/

	// Checks if the identifier is already in use
	bool AORSystemIsIdentifierUsed(const char* identifier);

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_SYSTEM_API_H__
