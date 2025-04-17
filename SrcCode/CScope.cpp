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
