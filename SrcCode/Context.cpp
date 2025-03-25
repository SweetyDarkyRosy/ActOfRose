/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Context.cpp
	@Created:		24.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Context.h"

#include "Log.h"
#include "ReturnCodes.h"
#include "Token.h"
#include "Utility/StringMisc.h"


// ----- ActOfRose::Context::CVarDeclarationContext class -----

// Constructor
ActOfRose::Context::CVarDeclarationContext::CVarDeclarationContext() :
	_mState(ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_VariableName)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New variable declaration context context has been created"),
		(sizeof(PREF_STRING("New variable declaration context context has been created")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CVarDeclarationContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (_mState)
	{
		case ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_VariableName:
		{
			if (token->type == ActOfRose::Token::ETokenType::ETTIdentifier)
			{
				_mState = ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_InitDisjunction;
			}
			else
			{
				ActOfRose::WriteLog(PREF_STRING("Identifier was expected"), (sizeof(PREF_STRING("Identifier was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_IDENTIFIER_EXPECTED;
			}

			break;
		}

		case ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_InitDisjunction:
		{
			if (token->type == ActOfRose::Token::ETokenType::ETTSemicolon)
			{
				return AOR_CONTEXT_EXECUTE;
			}
			else
			{
				ActOfRose::WriteLog(PREF_STRING("Expected ';' after variable declaration"),
					(sizeof(PREF_STRING("Expected ';' after variable declaration")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			break;
		}

		default:
		{
			return AOR_ERROR_INTERNAL_ERROR;
		}
	}

	return AOR_SUCCESS;
}
