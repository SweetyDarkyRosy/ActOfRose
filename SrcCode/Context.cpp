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


// ----- ActOfRose::Context::CExpressionEvaluationContext class -----

// Constructor
ActOfRose::Context::CExpressionEvaluationContext::CExpressionEvaluationContext() :
	_mNestingDepth(0)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New expression evaluation context has been created"),
		(sizeof(PREF_STRING("New expression evaluation context has been created")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CExpressionEvaluationContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (token->type)
	{
		case ActOfRose::Token::ETokenType::ETTNumber:
		case ActOfRose::Token::ETokenType::ETTDot:
		case ActOfRose::Token::ETokenType::ETTString:
		case ActOfRose::Token::ETokenType::ETTIdentifier:
		case ActOfRose::Token::ETokenType::ETTOperator:
		{
			break;
		}

		case ActOfRose::Token::ETokenType::ETTSemicolon:
		{
			if (_mNestingDepth == 0)
			{
				return AOR_CONTEXT_COMPLETE;
			}
			else
			{
				ActOfRose::WriteLog(PREF_STRING("Expected expression before ';' token"),
					(sizeof(PREF_STRING("Expected expression before ';' token")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);
	
				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}
		}

		case ActOfRose::Token::ETokenType::ETTComma:
		{
			if (_mNestingDepth == 0)
			{
				return AOR_CONTEXT_COMPLETE;
			}
			else
			{
				break;
			}
		}

		case ActOfRose::Token::ETokenType::ETTCurlyBracketLeft:
		case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
		{
			_mNestingDepth++;
			break;
		}

		case ActOfRose::Token::ETokenType::ETTCurlyBracketRight:
		case ActOfRose::Token::ETokenType::ETTRoundBracketRight:
		{
			if (_mNestingDepth == 0)
			{
				return AOR_CONTEXT_COMPLETE;
			}
			else
			{
				_mNestingDepth--;
				break;
			}
		}

		default:
		{
			ActOfRose::WriteLog(PREF_STRING("Unexpected token"), (sizeof(PREF_STRING("Unexpected token")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
		}
	}

	return AOR_SUCCESS;
}


// ----- ActOfRose::Context::CVarDeclarationContext class -----

// Constructor
ActOfRose::Context::CVarDeclarationContext::CVarDeclarationContext() :
	_mState(ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_Initial)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New variable declaration context has been created"),
		(sizeof(PREF_STRING("New variable declaration context has been created")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CVarDeclarationContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (_mState)
	{
		case ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_Initial:
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
				case ActOfRose::Keyword::EKeywords::EK_Override:
				case ActOfRose::Keyword::EKeywords::EK_Strict:
				{
					_mState = ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_VarKeyword;
					break;
				}

				default:
				{
					_mState = ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_VariableName;
					break;
				}
			}

			break;
		}

		case ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_VarKeyword:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTKeyword)
			{
				ActOfRose::WriteLog(PREF_STRING("Expected 'var' keyword"),
					(sizeof(PREF_STRING("Expected 'var' keyword")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
	
				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			ActOfRose::Keyword::EKeywords keyword;
			if (GetKeyword(&keyword, &(token->value)) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Keyword not found. Internal error"),
					(sizeof(PREF_STRING("Keyword not found. Internal error")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INTERNAL_ERROR;
			}

			if (keyword != ActOfRose::Keyword::EKeywords::EK_Var)
			{
				ActOfRose::WriteLog(PREF_STRING("Expected 'var' keyword"),
					(sizeof(PREF_STRING("Expected 'var' keyword")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
	
				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_VariableName;

			break;
		}

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
				return AOR_CONTEXT_COMPLETE;
			}
			else if ((token->type == ActOfRose::Token::ETokenType::ETTOperator) &&
				(token->value.compare("=") == 0))
			{
				_mState = ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_ExpressionBegin;

				break;
			}
			else
			{
				/*
				ActOfRose::WriteLog(PREF_STRING("Expected ';' after variable declaration"),
					(sizeof(PREF_STRING("Expected ';' after variable declaration")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);
				*/

				ActOfRose::WriteLog(PREF_STRING("Expected initialiser"), (sizeof(PREF_STRING("Expected initialiser")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			break;
		}

		case ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_ExpressionBegin:
		{
			switch (token->type)
			{
				case ActOfRose::Token::ETokenType::ETTNumber:
				case ActOfRose::Token::ETokenType::ETTString:
				case ActOfRose::Token::ETokenType::ETTIdentifier:
				case ActOfRose::Token::ETokenType::ETTOperator:
				case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
				case ActOfRose::Token::ETokenType::ETTCurlyBracketLeft:
				{
					_mState = ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_ExpressionEnd;

					return AOR_CONTEXT_CREATE;
				}

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Expected expression"), (sizeof(PREF_STRING("Expected expression")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

			break;
		}

		case ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates::EVDCS_ExpressionEnd:
		{
			switch (token->type)
			{
				case ActOfRose::Token::ETokenType::ETTSemicolon:
				{
					return AOR_CONTEXT_COMPLETE;
				}

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Expected ';'"), (sizeof(PREF_STRING("Expected ';'")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
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


// ----- ActOfRose::Context::CFuncDeclarationContext class -----

// Constructor
ActOfRose::Context::CFuncDeclarationContext::CFuncDeclarationContext() :
	_mState(ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_Initial)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New function declaration context has been created"),
		(sizeof(PREF_STRING("New function declaration context has been created")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CFuncDeclarationContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (_mState)
	{
		case ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_Initial:
		{
			_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_FuncName;

			break;
		}

		case ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_FuncName:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTIdentifier)
			{
				ActOfRose::WriteLog(PREF_STRING("Identifier was expected"), (sizeof(PREF_STRING("Identifier was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_IDENTIFIER_EXPECTED;
			}

			_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_ParamStart;

			break;
		}

		case ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_ParamStart:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left round bracket was expected"), (sizeof(PREF_STRING("Left round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_ParamDisjunctionInitial;

			break;
		}

		case ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_ParamDisjunctionInitial:
		{
			if (token->type == ActOfRose::Token::ETokenType::ETTRoundBracketRight)
			{
				_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_BodyStart;
			}
			else if (token->type == ActOfRose::Token::ETokenType::ETTIdentifier)
			{
				_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_ParamDisjunctionSubsequent;
			}
			else
			{
				ActOfRose::WriteLog(PREF_STRING("Identifier as parameter name was expected"),
					(sizeof(PREF_STRING("Identifier as parameter name was expected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			break;
		}

		case ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_ParamDisjunctionSubsequent:
		{
			if (token->type == ActOfRose::Token::ETokenType::ETTRoundBracketRight)
			{
				_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_BodyStart;
			}
			else if (token->type == ActOfRose::Token::ETokenType::ETTComma)
			{
				_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_ParamName;
			}
			else
			{
				ActOfRose::WriteLog(PREF_STRING("Right round bracket was expected"), (sizeof(PREF_STRING("Right round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			break;
		}

		case ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_ParamName:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTIdentifier)
			{
				ActOfRose::WriteLog(PREF_STRING("Identifier as parameter name was expected"),
					(sizeof(PREF_STRING("Identifier as parameter name was expected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_IDENTIFIER_EXPECTED;
			}

			_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_ParamDisjunctionSubsequent;

			break;
		}

		case ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_BodyStart:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left curly bracket was expected"),
					(sizeof(PREF_STRING("Left curly bracket was expected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_BodyRoutine;

			break;
		}

		case ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_BodyRoutine:
		{
			switch (token->type)
			{
				case ActOfRose::Token::ETokenType::ETTCurlyBracketRight:
				{
					return AOR_CONTEXT_COMPLETE;
				}

				case ActOfRose::Token::ETokenType::ETTNumber:
				case ActOfRose::Token::ETokenType::ETTString:
				case ActOfRose::Token::ETokenType::ETTIdentifier:
				case ActOfRose::Token::ETokenType::ETTOperator:
				case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
				case ActOfRose::Token::ETokenType::ETTCurlyBracketLeft:
				case ActOfRose::Token::ETokenType::ETTKeyword:
				{
					_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_SyntacticUnitEnd;

					return AOR_CONTEXT_CREATE;
				}

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Expected expression"), (sizeof(PREF_STRING("Expected expression")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

			break;
		}

		case ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_SyntacticUnitEnd:
		{
			switch (token->type)
			{
				case ActOfRose::Token::ETokenType::ETTCurlyBracketRight:
				case ActOfRose::Token::ETokenType::ETTSemicolon:
				{
					break;
				}

				default:
				{
					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

			_mState = ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates::EFDCS_BodyRoutine;

			break;
		}
	}

	return AOR_SUCCESS;
}
