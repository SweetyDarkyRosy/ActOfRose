/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CScope.h
	@Created:		17.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of scope. */

#ifndef __ACT_OF_ROSE_SCOPE_CLASS_H__
#define __ACT_OF_ROSE_SCOPE_CLASS_H__

#include <map>
#include <string>

#include "Element.h"


namespace ActOfRose
{
	// Class of scope in a script context with local variables and constants
	class CScope
	{
	public:
		// Destructor
		~CScope();

	private:
		std::map<const std::string, ActOfRose::SElement>		_mLocalIdentifierMap;		// Local map of associations between identifiers and elements

	};

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_SCOPE_CLASS_H__
