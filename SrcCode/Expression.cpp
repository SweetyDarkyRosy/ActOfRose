/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Expression.cpp
	@Created:		30.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Expression.h"

#include "ReturnCodes.h"
#include "Log.h"
#include "Utility/StringMisc.h"


// ----- ActOfRose::AST::CExprASTOperandNode class -----

// Destructor
ActOfRose::AST::CExprASTOperandNode::~CExprASTOperandNode()
{
	if ((_mValueRef.value != nullptr) && (_mValueRef.category != ActOfRose::Value::EValueCategories::EVC_LValue))
	{
		delete _mValueRef.value;
	}
}

// Retrieves a value and sets it to the value reference holder pointed to by valueRefHolder
int ActOfRose::AST::CExprASTOperandNode::RetrieveValue(ActOfRose::Value::SValueReference* valueRefHolder)
{
	valueRefHolder->value = _mValueRef.value;
	valueRefHolder->category = _mValueRef.category;

	return AOR_SUCCESS;
}


// ----- ActOfRose::AST::CExprASTOperatorNode class -----

// Destructor
ActOfRose::AST::CExprASTOperatorNode::~CExprASTOperatorNode()
{
	if (_pLeftChild != nullptr)
	{
		delete _pLeftChild;
	}

	if (_pRightChild != nullptr)
	{
		delete _pRightChild;
	}
}

// Returns the precedence of an operator
unsigned int ActOfRose::AST::CExprASTOperatorNode::GetPrecedence() const
{
	switch (_mOperationType)
	{
		case ActOfRose::Operation::EOperationTypes::EO_Multiplication:
		case ActOfRose::Operation::EOperationTypes::EO_Division:
		{
			return 1;
		}

		case ActOfRose::Operation::EOperationTypes::EO_Summation:
		case ActOfRose::Operation::EOperationTypes::EO_Subtraction:
		{
			return 2;
		}

		case ActOfRose::Operation::EOperationTypes::EO_Assignment:
		{
			return 3;
		}

		default:
		{
			return 0;
		}
	}
}

// Retrieves a value and sets it to the value reference holder pointed to by valueRefHolder
int ActOfRose::AST::CExprASTOperatorNode::RetrieveValue(ActOfRose::Value::SValueReference* valueRefHolder)
{
	if (_pRightChild == nullptr)
	{
		ActOfRose::WriteLog(PREF_STRING("Internal error while operation execution. Operator node has no children"),
			(sizeof(PREF_STRING("Internal error while operation execution. Operator node has no children")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_EXEC_NO_OPERATOR_CHILD;
	}

	return AOR_SUCCESS;
}
