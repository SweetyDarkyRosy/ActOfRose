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
	// Types of visibility of elements in scopes
	enum EScopeVisibilityTypes
	{
		ESIT_InheritingScope,			// Scope inherits local elements from lower scopes
		ESIT_LocalScope					// Scope can access only its local elements
	};

	// Class of scope in a script context with local variables and constants
	class CScope
	{
	public:
		// Constructor
		CScope(ActOfRose::EScopeVisibilityTypes visibilityType) :
			_mVisibilityType(visibilityType)
		{}

		// Move constructor
		CScope(ActOfRose::CScope&& scope) :
			_mLocalIdentifierMap(std::move(scope._mLocalIdentifierMap)), _mVisibilityType(scope._mVisibilityType)
		{}

		// Destructor
		~CScope();

	public:
		// Registers an identifier and builds an association with an element (variable or constant) in a scope
		ActOfRose::SElement* RegisterIdentifierAndElement(const char* identifier, ActOfRose::EElementType type, void* addr);

		// Checks if the identifier is already in use in a scope
		bool IsIdentifierUsed(const char* identifier);

		// Returns a pointer to a block of information about registered element by the given identifier if it exists in a scope
		ActOfRose::SElement* GetElementByIdentifier(const char* identifier);


		// Returns the type of visibility of elements in a scope
		inline ActOfRose::EScopeVisibilityTypes GetElementVisibilityType() const { return _mVisibilityType; }

	private:
		std::map<const std::string, ActOfRose::SElement> _mLocalIdentifierMap;		// Local map of associations between identifiers and elements

		ActOfRose::EScopeVisibilityTypes _mVisibilityType;							// Type of visibility of elements in a scope

	};

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_SCOPE_CLASS_H__
