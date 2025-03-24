/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CSequencer.cpp
	@Created:		23.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CSequencer.h"

#include "ReturnCodes.h"
#include "Log.h"
#include "Token.h"
#include "Context.h"
#include "Utility/StringMisc.h"


// ----- ActOfRose::Context::CSequencer class -----

// Destructor
ActOfRose::Context::CSequencer::~CSequencer()
{
	while (_mContexts.size() != 0)
	{
		delete _mContexts.top();
		_mContexts.pop();
	}
}

// Analyses the given token and manages current context
int ActOfRose::Context::CSequencer::ProcessToken(ActOfRose::Token::SToken* token)
{
	return AOR_SUCCESS;
}

// Determines a context based on a given token and creates it
int ActOfRose::Context::CSequencer::DetermineAndCreateContext(ActOfRose::Token::SToken* token)
{
	switch (token->type)
	{
		default:
		{
			ActOfRose::WriteLog(PREF_STRING("Could not determine context"), (sizeof(PREF_STRING("Could not determine context")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_CONTEXT_DETERMINATION_ERROR;
		}
	}
}
