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
#include "Operation.h"
#include "Utility/StringMisc.h"
#include "Value/Value.h"


// ----- ActOfRose::AST::CExprASTOperandNode class -----

// Constructor
ActOfRose::AST::CExprASTOperandNode::CExprASTOperandNode(ActOfRose::Value::CValue** valueHolder, ActOfRose::Value::EValueCategories valueCategory,
	ActOfRose::AST::CExprASTNode* parentNode) :
	ActOfRose::AST::CExprASTNode(ActOfRose::AST::EExprASTNodeType::EESTNTOperand, parentNode)
{
	_mValueRef.category = valueCategory;

	if (_mValueRef.category == ActOfRose::Value::EValueCategories::EVC_LValue)
	{
		_mValueRef.value.valueHolder = valueHolder;
	}
	else
	{
		_mValueRef.value.value = *valueHolder;
	}
}

// Constructor that takes another value reference to copy data
ActOfRose::AST::CExprASTOperandNode::CExprASTOperandNode(ActOfRose::Value::SValueReference* valueRef, ActOfRose::AST::CExprASTNode* parentNode) :
	ActOfRose::AST::CExprASTNode(ActOfRose::AST::EExprASTNodeType::EESTNTOperand, parentNode)
{
	_mValueRef.category = valueRef->category;

	if (_mValueRef.category == ActOfRose::Value::EValueCategories::EVC_LValue)
	{
		_mValueRef.value.valueHolder = valueRef->value.valueHolder;
	}
	else
	{
		_mValueRef.value.value = valueRef->value.value;
	}
}

// Destructor
ActOfRose::AST::CExprASTOperandNode::~CExprASTOperandNode()
{
	if (_mValueRef.category == ActOfRose::Value::EValueCategories::EVC_RValue)
	{
		delete _mValueRef.value.value;
	}
}

// Retrieves a value and sets it to the value reference holder pointed to by valueRefHolder
int ActOfRose::AST::CExprASTOperandNode::RetrieveValue(ActOfRose::Value::SValueReference* valueRefHolder)
{
	valueRefHolder->category = _mValueRef.category;

	if (_mValueRef.category == ActOfRose::Value::EValueCategories::EVC_LValue)
	{
		valueRefHolder->value.valueHolder = _mValueRef.value.valueHolder;
	}
	else
	{
		valueRefHolder->value.value = _mValueRef.value.value;
	}

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

		case ActOfRose::Operation::EOperationTypes::EO_CampareEqualTo:
		case ActOfRose::Operation::EOperationTypes::EO_CampareNotEqualTo:
		{
			return 3;
		}

		case ActOfRose::Operation::EOperationTypes::EO_Assignment:
		{
			return 4;
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

	ActOfRose::Value::SValueReference leftValueRef;
	if (_pLeftChild == nullptr)
	{
		leftValueRef.category = ActOfRose::Value::EValueCategories::EVC_None;
	}
	else
	{
		int valueRetrievingResult = _pLeftChild->RetrieveValue(&leftValueRef);
		if (valueRetrievingResult != AOR_SUCCESS)
		{
			return valueRetrievingResult;
		}

		if (leftValueRef.category == ActOfRose::Value::EValueCategories::EVC_None)
		{
			ActOfRose::WriteLog(PREF_STRING("Child node of operator returned Null value"),
			(sizeof(PREF_STRING("Child node of operator returned Null value")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_EXEC_INITIAL_VALUE_REQUIRED;
		}
	}

	ActOfRose::Value::SValueReference rightValueRef;
	{
		int valueRetrievingResult = _pRightChild->RetrieveValue(&rightValueRef);
		if (valueRetrievingResult != AOR_SUCCESS)
		{
			if (leftValueRef.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
			{
				delete leftValueRef.value.value;
			}

			return valueRetrievingResult;
		}
	}

	int execResult = ActOfRose::Operation::ExecuteOperation(valueRefHolder, _mOperationType, &leftValueRef, &rightValueRef);

	if (leftValueRef.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
	{
		delete leftValueRef.value.value;
	}

	if (rightValueRef.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
	{
		delete rightValueRef.value.value;
	}

	return execResult;
}


// ----- ActOfRose::AST::CExprASTRoundBracketNode class -----

// Retrieves a value and sets it to the value reference holder pointed to by valueRefHolder
int ActOfRose::AST::CExprASTRoundBracketNode::RetrieveValue(ActOfRose::Value::SValueReference* valueRefHolder)
{
	_pChildNode->RetrieveValue(valueRefHolder);

	return AOR_SUCCESS;
}
