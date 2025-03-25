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
#include "Keywords.h"
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
	int result;

	if (_mContexts.size() == 0)
	{
		result = DetermineAndCreateContext(token);
	}
	else
	{
		result = _mContexts.top()->ProcessToken(token);

		switch (result)
		{
			case AOR_CONTEXT_CREATE:
			{
				if ((result = DetermineAndCreateContext(token)) != AOR_SUCCESS)
				{
					break;
				}

				result = _mContexts.top()->ProcessToken(token);
				
				break;
			}

			case AOR_CONTEXT_COMPLETE:
			{
				delete _mContexts.top();
				_mContexts.pop();

				if (_mContexts.size() != 0)
				{
					result = _mContexts.top()->ProcessToken(token);

					/**
						Parent context cannot create a new context anyway without further token
					*/
				}

				break;
			}

			case AOR_CONTEXT_EXECUTE:
			{
				delete _mContexts.top();
				_mContexts.pop();

				break;
			}

			default:
			{
				break;
			}
		}
	}

	return result;
}

// Determines a context based on a given token and creates it
int ActOfRose::Context::CSequencer::DetermineAndCreateContext(ActOfRose::Token::SToken* token)
{
	switch (token->type)
	{
		case ActOfRose::Token::ETTKeyword:
		{
			ActOfRose::Keyword::EKeywords keyword;
			if (GetKeyword(&keyword, &(token->value)) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Keyword not found. Internal error"),
					(sizeof(PREF_STRING("Keyword not found. Internal error")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INTERNAL_ERROR;
			}

			switch (keyword)
			{
				case ActOfRose::Keyword::EKeywords::EK_Var:
				{
					_mContexts.push(new ActOfRose::Context::CVarDeclarationContext());

					break;
				}

				default:
				{
					goto DetermineAndCreateContextError;
				}
			}

			break;
		}

		default:
		{
		DetermineAndCreateContextError:
			ActOfRose::WriteLog(PREF_STRING("Could not determine context"), (sizeof(PREF_STRING("Could not determine context")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_CONTEXT_DETERMINATION_ERROR;
		}
	}

	return AOR_SUCCESS;
}
