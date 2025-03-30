/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Expression.cpp
	@Created:		30.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Expression.h"

#include "ReturnCodes.h"


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
