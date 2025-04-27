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
		if ((result = DetermineAndCreateContext(token)) != AOR_SUCCESS)
		{
			return result;
		}
	}

	while (_mContexts.size() != 0)
	{
		result = _mContexts.top()->ProcessToken(token);
		if (result == AOR_CONTEXT_CREATE)
		{
			if ((result = DetermineAndCreateContext(token)) != AOR_SUCCESS)
			{
				break;
			}
		}
		else if (result == AOR_CONTEXT_COMPLETE)
		{
			delete _mContexts.top();
			_mContexts.pop();

			if (_mContexts.size() == 0)
			{
				result = AOR_CONTEXT_EXECUTE;
			}
		}
		else if (result == AOR_CONTEXT_LOOKAHEAD_KEEP)
		{
			delete _mContexts.top();
			_mContexts.pop();

			if (_mContexts.size() == 0)
			{
				result = AOR_CONTEXT_EXECUTE_KEEP_LAST;
			}
			else
			{
				if ((result = DetermineAndCreateContext(token)) != AOR_SUCCESS)
				{
					return result;
				}
			}
		}
		else
		{
			break;
		}
	}

	return result;
}

// Commits finalisation of contexts and returns a status code
int ActOfRose::Context::CSequencer::CommitFinalisation()
{
	int result;

	while (_mContexts.empty() == false)
	{
		result = _mContexts.top()->CommitContextFinalisation();
		if (result < 0)
		{
			return result;
		}

		delete _mContexts.top();
		_mContexts.pop();
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
				case ActOfRose::Keyword::EKeywords::EK_Override:
				case ActOfRose::Keyword::EKeywords::EK_Strict:
				case ActOfRose::Keyword::EKeywords::EK_Local:
				{
					_mContexts.push(new ActOfRose::Context::CVarDeclarationContext());

					break;
				}

				case ActOfRose::Keyword::EKeywords::EK_Func:
				{
					_mContexts.push(new ActOfRose::Context::CFuncDeclarationContext());

					break;
				}

				case ActOfRose::Keyword::EKeywords::EK_If:
				{
					_mContexts.push(new ActOfRose::Context::CConditionalContext());

					break;
				}

				case ActOfRose::Keyword::EKeywords::EK_Elif:
				{
					ActOfRose::WriteLog(PREF_STRING("'elif' without a previous 'if'"),
						(sizeof(PREF_STRING("'elif' without a previous 'if'")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}

				case ActOfRose::Keyword::EKeywords::EK_Else:
				{
					ActOfRose::WriteLog(PREF_STRING("'else' without a previous 'if'"),
						(sizeof(PREF_STRING("'else' without a previous 'if'")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}

				case ActOfRose::Keyword::EKeywords::EK_While:
				{
					_mContexts.push(new ActOfRose::Context::CWhileLoopContext());

					break;
				}

				case ActOfRose::Keyword::EKeywords::EK_Return:
				{
					_mContexts.push(new ActOfRose::Context::CReturnStatementContext());

					break;
				}

				default:
				{
					goto DetermineAndCreateContextError;
				}
			}

			break;
		}

		case ActOfRose::Token::ETokenType::ETTNumber:
		case ActOfRose::Token::ETokenType::ETTString:
		case ActOfRose::Token::ETokenType::ETTIdentifier:
		case ActOfRose::Token::ETokenType::ETTOperator:
		case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
		case ActOfRose::Token::ETokenType::ETTCurlyBracketLeft:
		{
			_mContexts.push(new ActOfRose::Context::CExpressionEvaluationContext());

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
