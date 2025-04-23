/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CScope.cpp
	@Created:		17.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CScope.h"

#include "CVariable.h"


// ----- ActOfRose::CScope class -----

// Destructor
ActOfRose::CScope::~CScope()
{
	std::map<const std::string, ActOfRose::SElement>::iterator elIt = _mLocalIdentifierMap.begin();

	for (; elIt != _mLocalIdentifierMap.end(); elIt++)
	{
		std::pair<const std::string, ActOfRose::SElement>* elPair = &(*elIt);

		switch (elPair->second.type)
		{
			case ActOfRose::EElementType::EET_Variable:
			{
				ActOfRose::CVariable* var = (ActOfRose::CVariable*)(elPair->second.addr);
				delete var;

				break;
			}

			default:
			{
				break;
			}
		}
	}
}

// Registers an identifier and builds an association with an element (variable or constant) in a scope
ActOfRose::SElement* ActOfRose::CScope::RegisterIdentifierAndElement(const char* identifier, ActOfRose::EElementType type, void* addr)
{
	std::pair<std::map<const std::string, ActOfRose::SElement>::iterator, bool> result = _mLocalIdentifierMap.insert({ identifier, { type, addr } });
	if (result.second == true)
	{
		std::pair<const std::string, ActOfRose::SElement>* newElPair = &(*(result.first));

		return &(newElPair->second);
	}

	return nullptr;
}

// Checks if the identifier is already in use in a scope
bool ActOfRose::CScope::IsIdentifierUsed(const char* identifier)
{
	std::map<const std::string, ActOfRose::SElement>::iterator identifierIt = _mLocalIdentifierMap.find(identifier);
	if (identifierIt == _mLocalIdentifierMap.end())
	{
		return false;
	}

	return true;
}

// Returns a pointer to a block of information about registered element by the given identifier if it exists in a scope
ActOfRose::SElement* ActOfRose::CScope::GetElementByIdentifier(const char* identifier)
{
	std::map<const std::string, ActOfRose::SElement>::iterator identifierIt = _mLocalIdentifierMap.find(identifier);
	if (identifierIt == _mLocalIdentifierMap.end())
	{
		return nullptr;
	}

	std::pair<const std::string, ActOfRose::SElement>* elementPair = &(*identifierIt);
	
	return &(elementPair->second);
}
