/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CExecutor.cpp
	@Created:		20.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CExecutor.h"

#include "ReturnCodes.h"
#include "Log.h"
#include "SystemAPI.h"
#include "Keywords.h"
#include "CVariable.h"
#include "Expression.h"
#include "Utility/StringMisc.h"


// ----- ActOfRose::CExecutor class -----

// Constructor
ActOfRose::CExecutor::CExecutor()
{}


// Executes an operation based on a token group pointed to by tokenGroup
int ActOfRose::CExecutor::Execute(std::vector<ActOfRose::Token::SToken>* tokenGroup)
{
	_pCurrTokenGroup = tokenGroup;
	_mCurrTokenIndex = 0;

#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("Execution"), (sizeof(PREF_STRING("Execution")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Debug);
#endif

	switch ((*tokenGroup)[0].type)
	{
		case ActOfRose::Token::ETokenType::ETTKeyword:
		{
			ActOfRose::Keyword::EKeywords keyword;
			if (GetKeyword(&keyword, &((*tokenGroup)[0].value)) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Keyword not found. Internal error"),
					(sizeof(PREF_STRING("Keyword not found. Internal error")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INTERNAL_ERROR;
			}

			switch (keyword)
			{
				case ActOfRose::Keyword::EKeywords::EK_Var:
				{
					return DeclareAndInitialiseVariable(tokenGroup);
				}

				default:
				{
					ActOfRose::WriteLog(PREF_STRING("Unexpected token"), (sizeof(PREF_STRING("Unexpected token")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
				}
			}

			break;
		}

		default:
		{
			ActOfRose::WriteLog(PREF_STRING("Unexpected token"), (sizeof(PREF_STRING("Unexpected token")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
		}
	}

	return AOR_SUCCESS;
}

// Executes a variable declaration and initialisation
int ActOfRose::CExecutor::DeclareAndInitialiseVariable(std::vector<ActOfRose::Token::SToken>* tokenGroup)
{
	if (AORSystemIsIdentifierUsed((*tokenGroup)[1].value.c_str()) == true)
	{
		LogAlreadyUsedIdentifier((*tokenGroup)[1].value.c_str());
		return AOR_ERROR_EXEC_IDENTIFIER_ALREADY_IN_USE;
	}

	// New variable
	ActOfRose::CVariable* newVariable;

	if ((*tokenGroup)[2].type == ActOfRose::Token::ETokenType::ETTSemicolon)
	{
		newVariable = new ActOfRose::CVariable();
	}
	else if (((*tokenGroup)[2].type == ActOfRose::Token::ETokenType::ETTOperator) &&
		((*tokenGroup)[2].value.compare("=") == 0))
	{
		ActOfRose::Value::SValueReference newValueRef;
		{
			ActOfRose::AST::CExprASTNode* exprRoot;
			{
				int result = BuildExpressionAST(&exprRoot);
				if (result != AOR_SUCCESS)
				{
					delete exprRoot;

					return result;
				}
			}

			int result = exprRoot->RetrieveValue(&newValueRef);
			delete exprRoot;

			if (result != AOR_SUCCESS)
			{
				delete exprRoot;

				return result;
			}
		}

		if (newValueRef.category == ActOfRose::Value::EValueCategories::EVC_LValue)
		{
			newVariable = new ActOfRose::CVariable(CopyValue(newValueRef.value));
		}
		else
		{
			newVariable = new ActOfRose::CVariable(newValueRef.value);
		}
	}

	if (AORSystemRegisterIdentifierAndElement((*tokenGroup)[1].value.c_str(), ActOfRose::EElementType::EET_Variable, (void*)newVariable) == nullptr)
	{
		delete newVariable;

		std::string errorMsg = "Runtime error. Failed to declare the ";
		errorMsg += (*_pCurrTokenGroup)[1].value;
		errorMsg += " variable";

		ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_INTERNAL_ERROR;
	}

#ifdef _DEBUG
	{
		std::string msg = "Variable ";
		msg += (*_pCurrTokenGroup)[1].value;
		msg += " has been created";

		ActOfRose::WriteLog(msg.c_str(), msg.length(), ActOfRose::ELogLevel::ELL_Debug);
	}
#endif

	return AOR_SUCCESS;
}


/**
	Builds an AST for expression evaluation based on given token array and saves a root of the expression AST to a pointer pointed
	to by treeRootNodeHolder
 */
int ActOfRose::CExecutor::BuildExpressionAST(ActOfRose::AST::CExprASTNode** treeRootNodeHolder)
{
	return AOR_SUCCESS;
}


// Logs information about that a specified identifier already used
void ActOfRose::CExecutor::LogAlreadyUsedIdentifier(const char* identifier)
{
	std::string errorMsg = "Identifier ";
	errorMsg += identifier;
	errorMsg += " is already in use";

	ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);
}
