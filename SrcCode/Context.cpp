/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Context.cpp
	@Created:		24.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Context.h"

#include "Log.h"
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

		case ActOfRose::Token::ETokenType::ETTKeyword:
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
				case ActOfRose::Keyword::EKeywords::EK_True:
				case ActOfRose::Keyword::EKeywords::EK_False:
				{

					break;
				}

				default:
				{
					std::string errorMsg = "Keyword '" + token->value + "' is not a value";
					ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.size(), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

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
		case ActOfRose::Token::ETokenType::ETTSquareBracketLeft:
		case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
		{
			_mNestingDepth++;
			break;
		}

		case ActOfRose::Token::ETokenType::ETTCurlyBracketRight:
		case ActOfRose::Token::ETokenType::ETTSquareBracketRight:
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


// ----- ActOfRose::Context::CIndividualExpressionContext class -----

// Constructor
ActOfRose::Context::CIndividualExpressionContext::CIndividualExpressionContext() :
	_mState(ActOfRose::Context::CIndividualExpressionContext::EIndividualExprCtxStates::EIECS_Initial)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New individual expression context has been created"),
		(sizeof(PREF_STRING("New individual expression context has been created")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CIndividualExpressionContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (_mState)
	{
		case ActOfRose::Context::CIndividualExpressionContext::EIndividualExprCtxStates::EIECS_Initial:
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
					_mState = ActOfRose::Context::CIndividualExpressionContext::EIndividualExprCtxStates::EIECS_ExpressionEnd;

					return AOR_CONTEXT_CREATE;
				}

				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_True:
						case ActOfRose::Keyword::EKeywords::EK_False:
						{
							_mState = ActOfRose::Context::CIndividualExpressionContext::EIndividualExprCtxStates::EIECS_ExpressionEnd;

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

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Expected expression"), (sizeof(PREF_STRING("Expected expression")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

			break;
		}

		case ActOfRose::Context::CIndividualExpressionContext::EIndividualExprCtxStates::EIECS_ExpressionEnd:
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
			if (token->type != ActOfRose::Token::ETokenType::ETTKeyword)
			{
				ActOfRose::WriteLog(PREF_STRING("Expected keyword"),
					(sizeof(PREF_STRING("Expected keyword")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
	
				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

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
				case ActOfRose::Keyword::EKeywords::EK_Local:
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

				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_True:
						case ActOfRose::Keyword::EKeywords::EK_False:
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
				case ActOfRose::Token::ETokenType::ETTComma:
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


// ----- ActOfRose::Context::CConditionalContext class -----

// Constructor
ActOfRose::Context::CConditionalContext::CConditionalContext() :
	_mState(ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_Initial),
	_mLastBranchtype(ActOfRose::Context::CConditionalContext::EConditionalCtxBranchTypes::ECCBT_If)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New conditional context has been created"), (sizeof(PREF_STRING("New conditional context has been created")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CConditionalContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (_mState)
	{
		case ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_Initial:
		{
			_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_ConditionBegin;

			break;
		}

		case ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_ConditionBegin:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left round bracket was expected"), (sizeof(PREF_STRING("Left round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_ConditionalExpr;

			break;
		}

		case ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_ConditionalExpr:
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
					_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_ConditionEnd;

					return AOR_CONTEXT_CREATE;
				}

				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_True:
						case ActOfRose::Keyword::EKeywords::EK_False:
						{
							_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_ConditionEnd;

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

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Expected expression"), (sizeof(PREF_STRING("Expected expression")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

			break;
		}

		case ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_ConditionEnd:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTRoundBracketRight)
			{
				ActOfRose::WriteLog(PREF_STRING("Right round bracket was expected"), (sizeof(PREF_STRING("Right round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_BodyStart;

			break;
		}

		case ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_BodyStart:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left curly bracket was expected"),
					(sizeof(PREF_STRING("Left curly bracket was expected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_BodyRoutine;

			break;
		}

		case ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_BodyRoutine:
		{
			switch (token->type)
			{
				case ActOfRose::Token::ETokenType::ETTCurlyBracketRight:
				{
					_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_BranchDisjunction;

					break;
				}

				case ActOfRose::Token::ETokenType::ETTNumber:
				case ActOfRose::Token::ETokenType::ETTString:
				case ActOfRose::Token::ETokenType::ETTIdentifier:
				case ActOfRose::Token::ETokenType::ETTOperator:
				case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
				case ActOfRose::Token::ETokenType::ETTCurlyBracketLeft:
				case ActOfRose::Token::ETokenType::ETTKeyword:
				{
					_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_SyntacticUnitEnd;

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

		case ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_SyntacticUnitEnd:
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

			_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_BodyRoutine;

			break;
		}

		case ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_BranchDisjunction:
		{
			switch (token->type)
			{
				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_Elif:
						{
							if (_mLastBranchtype == ActOfRose::Context::CConditionalContext::EConditionalCtxBranchTypes::ECCBT_Else)
							{
								ActOfRose::WriteLog(PREF_STRING("'elif' statement cannot appear after 'else'"),
									(sizeof(PREF_STRING("'elif' statement cannot appear after 'else'")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

								return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
							}

							_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_ConditionBegin;
							_mLastBranchtype = ActOfRose::Context::CConditionalContext::EConditionalCtxBranchTypes::ECCBT_Elif;

							break;
						}

						case ActOfRose::Keyword::EKeywords::EK_Else:
						{
							_mState = ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_BodyStart;
							_mLastBranchtype = ActOfRose::Context::CConditionalContext::EConditionalCtxBranchTypes::ECCBT_Else;

							break;
						}

						default:
						{
							return AOR_CONTEXT_LOOKAHEAD_KEEP;
						}
					}

					break;
				}

				case ActOfRose::Token::ETokenType::ETTCurlyBracketRight:
				{
					return AOR_CONTEXT_COMPLETE;
				}

				default:
				{
					return AOR_CONTEXT_LOOKAHEAD_KEEP;
				}
			}

			break;
		}
	}

	return AOR_SUCCESS;
}

// Commits finalisation of context with current set of tokens if possible
int ActOfRose::Context::CConditionalContext::CommitContextFinalisation()
{
	if (_mState == ActOfRose::Context::CConditionalContext::EConditionalCtxStates::ECCS_BranchDisjunction)
	{
		return AOR_CONTEXT_COMPLETE;
	}

	return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT;
}


// ----- ActOfRose::Context::CWhileLoopContext class -----

// Constructor
ActOfRose::Context::CWhileLoopContext::CWhileLoopContext() :
	_mState(ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_Initial)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New 'while' loop context has been created"),
		(sizeof(PREF_STRING("New 'while' loop context has been created")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CWhileLoopContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (_mState)
	{
		case ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_Initial:
		{
			_mState = ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_ConditionBegin;

			break;
		}

		case ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_ConditionBegin:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left round bracket was expected"), (sizeof(PREF_STRING("Left round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_ConditionalExpr;

			break;
		}

		case ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_ConditionalExpr:
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
					_mState = ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_ConditionEnd;

					return AOR_CONTEXT_CREATE;
				}

				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_True:
						case ActOfRose::Keyword::EKeywords::EK_False:
						{
							_mState = ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_ConditionEnd;

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

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Expected expression"), (sizeof(PREF_STRING("Expected expression")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

			break;
		}

		case ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_ConditionEnd:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTRoundBracketRight)
			{
				ActOfRose::WriteLog(PREF_STRING("Right round bracket was expected"), (sizeof(PREF_STRING("Right round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_BodyStart;

			break;
		}

		case ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_BodyStart:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left curly bracket was expected"),
					(sizeof(PREF_STRING("Left curly bracket was expected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_BodyRoutine;

			break;
		}

		case ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_BodyRoutine:
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
					_mState = ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_SyntacticUnitEnd;

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

		case ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_SyntacticUnitEnd:
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

			_mState = ActOfRose::Context::CWhileLoopContext::EWhileLoopCtxStates::EWLCS_BodyRoutine;

			break;
		}
	}

	return AOR_SUCCESS;
}


// ----- ActOfRose::Context::CForLoopContext class -----

// Constructor
ActOfRose::Context::CForLoopContext::CForLoopContext() :
	_mState(ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_Initial)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New 'for' loop context has been created"),
		(sizeof(PREF_STRING("New 'for' loop context has been created")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CForLoopContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (_mState)
	{
		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_Initial:
		{
			_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ControlBlockBegin;

			break;
		}

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ControlBlockBegin:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left round bracket was expected"), (sizeof(PREF_STRING("Left round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_InitExprStmtDisjunction;

			break;
		}

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_InitExprStmtDisjunction:
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
					_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_InitExprStmtEnd;

					return AOR_CONTEXT_CREATE;
				}

				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_True:
						case ActOfRose::Keyword::EKeywords::EK_False:
						case ActOfRose::Keyword::EKeywords::EK_Var:
						case ActOfRose::Keyword::EKeywords::EK_Override:
						case ActOfRose::Keyword::EKeywords::EK_Strict:
						case ActOfRose::Keyword::EKeywords::EK_Local:
						{
							_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_InitExprStmtEnd;

							return AOR_CONTEXT_CREATE;
						}

						default:
						{
							ActOfRose::WriteLog(PREF_STRING("Expected expression"), (sizeof(PREF_STRING("Expected expression")) / sizeof(PChar)),
								ActOfRose::ELogLevel::ELL_Error);

							return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
						}
					}
				}

				case ActOfRose::Token::ETokenType::ETTSemicolon:
				{
					_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ConditionalExprDisjunction;
					break;
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

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_InitExprStmtEnd:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTSemicolon)
			{
				ActOfRose::WriteLog(PREF_STRING("Expected ';'"), (sizeof(PREF_STRING("Expected ';'")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ConditionalExprDisjunction;

			break;
		}

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ConditionalExprDisjunction:
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
					_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ConditionalExprEnd;

					return AOR_CONTEXT_CREATE;
				}

				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_True:
						case ActOfRose::Keyword::EKeywords::EK_False:
						{
							_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ConditionalExprEnd;

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

				case ActOfRose::Token::ETokenType::ETTSemicolon:
				{
					_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_AdvancementExprDisjunction;
					break;
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

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ConditionalExprEnd:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTSemicolon)
			{
				ActOfRose::WriteLog(PREF_STRING("Expected ';'"), (sizeof(PREF_STRING("Expected ';'")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_AdvancementExprDisjunction;

			break;
		}

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_AdvancementExprDisjunction:
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
					_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ControlBlockEnd;

					return AOR_CONTEXT_CREATE;
				}

				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_True:
						case ActOfRose::Keyword::EKeywords::EK_False:
						{
							_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ControlBlockEnd;

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

				case ActOfRose::Token::ETokenType::ETTRoundBracketRight:
				{
					_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_BodyStart;
					break;
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

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_ControlBlockEnd:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTRoundBracketRight)
			{
				ActOfRose::WriteLog(PREF_STRING("Right round bracket was expected"), (sizeof(PREF_STRING("Right round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_BodyStart;

			break;
		}

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_BodyStart:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left curly bracket was expected"),
					(sizeof(PREF_STRING("Left curly bracket was expected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_BodyRoutine;

			break;
		}

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_BodyRoutine:
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
					_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_SyntacticUnitEnd;

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

		case ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_SyntacticUnitEnd:
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

			_mState = ActOfRose::Context::CForLoopContext::EForLoopCtxStates::EFLCS_BodyRoutine;

			break;
		}
	}

	return AOR_SUCCESS;
}


// ----- ActOfRose::Context::CDoWhileLoopContext class -----

// Constructor
ActOfRose::Context::CDoWhileLoopContext::CDoWhileLoopContext() :
	_mState(ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_Initial)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New 'do-while' loop context has been created"),
		(sizeof(PREF_STRING("New 'do-while' loop context has been created")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CDoWhileLoopContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (_mState)
	{
		case ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_Initial:
		{
			_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_BodyStart;

			break;
		}

		case ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_BodyStart:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left curly bracket was expected"),
					(sizeof(PREF_STRING("Left curly bracket was expected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_BodyRoutine;

			break;
		}

		case ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_BodyRoutine:
		{
			switch (token->type)
			{
				case ActOfRose::Token::ETokenType::ETTCurlyBracketRight:
				{
					_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_WhileKeyword;
					break;
				}

				case ActOfRose::Token::ETokenType::ETTNumber:
				case ActOfRose::Token::ETokenType::ETTString:
				case ActOfRose::Token::ETokenType::ETTIdentifier:
				case ActOfRose::Token::ETokenType::ETTOperator:
				case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
				case ActOfRose::Token::ETokenType::ETTCurlyBracketLeft:
				case ActOfRose::Token::ETokenType::ETTKeyword:
				{
					_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_SyntacticUnitEnd;

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

		case ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_SyntacticUnitEnd:
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

			_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_BodyRoutine;

			break;
		}

		case ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_WhileKeyword:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTKeyword)
			{
				ActOfRose::WriteLog(PREF_STRING("Expected 'while' keyword"),
					(sizeof(PREF_STRING("Expected 'while' keyword")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
	
				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			ActOfRose::Keyword::EKeywords keyword;
			if (GetKeyword(&keyword, &(token->value)) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Keyword not found. Internal error"),
					(sizeof(PREF_STRING("Keyword not found. Internal error")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INTERNAL_ERROR;
			}

			if (keyword != ActOfRose::Keyword::EKeywords::EK_While)
			{
				ActOfRose::WriteLog(PREF_STRING("Expected 'while' keyword"),
					(sizeof(PREF_STRING("Expected 'while' keyword")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
	
				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_ConditionBegin;

			break;
		}

		case ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_ConditionBegin:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
			{
				ActOfRose::WriteLog(PREF_STRING("Left round bracket was expected"), (sizeof(PREF_STRING("Left round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_ConditionalExpr;

			break;
		}

		case ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_ConditionalExpr:
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
					_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_ConditionEnd;

					return AOR_CONTEXT_CREATE;
				}

				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_True:
						case ActOfRose::Keyword::EKeywords::EK_False:
						{
							_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_ConditionEnd;

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

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Expected expression"), (sizeof(PREF_STRING("Expected expression")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

			break;
		}

		case ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_ConditionEnd:
		{
			if (token->type != ActOfRose::Token::ETokenType::ETTRoundBracketRight)
			{
				ActOfRose::WriteLog(PREF_STRING("Right round bracket was expected"), (sizeof(PREF_STRING("Right round bracket was expected")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}

			_mState = ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_StatementEnd;

			break;
		}

		case ActOfRose::Context::CDoWhileLoopContext::EDoWhileLoopCtxStates::EDWLCS_StatementEnd:
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
	}

	return AOR_SUCCESS;
}


// ----- ActOfRose::Context::CReturnStatementContext class -----

// Constructor
ActOfRose::Context::CReturnStatementContext::CReturnStatementContext() :
	_mState(ActOfRose::Context::CReturnStatementContext::EReturnStatementCtxStates::ERSCS_Initial)
{
#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("New return statement context has been created"),
		(sizeof(PREF_STRING("New return statement context has been created")) / sizeof(PChar)),
		ActOfRose::ELogLevel::ELL_Debug);
#endif
}

// Analyses the given token, checks current sequence for logical errors and updates a context
int ActOfRose::Context::CReturnStatementContext::ProcessToken(ActOfRose::Token::SToken* token)
{
	switch (_mState)
	{
		case ActOfRose::Context::CReturnStatementContext::EReturnStatementCtxStates::ERSCS_Initial:
		{
			_mState = ActOfRose::Context::CReturnStatementContext::EReturnStatementCtxStates::ERSCS_ExpressionBegin;

			break;
		}

		case ActOfRose::Context::CReturnStatementContext::EReturnStatementCtxStates::ERSCS_ExpressionBegin:
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
					_mState = ActOfRose::Context::CReturnStatementContext::EReturnStatementCtxStates::ERSCS_ExpressionEnd;

					return AOR_CONTEXT_CREATE;
				}

				case ActOfRose::Token::ETokenType::ETTKeyword:
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
						case ActOfRose::Keyword::EKeywords::EK_True:
						case ActOfRose::Keyword::EKeywords::EK_False:
						{
							_mState = ActOfRose::Context::CReturnStatementContext::EReturnStatementCtxStates::ERSCS_ExpressionEnd;

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

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Expected expression"), (sizeof(PREF_STRING("Expected expression")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

			break;
		}

		case ActOfRose::Context::CReturnStatementContext::EReturnStatementCtxStates::ERSCS_ExpressionEnd:
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
