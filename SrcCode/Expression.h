/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Expression.h
	@Created:		26.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Elements for building AST for expression evaluation. */

#ifndef __ACT_OF_ROSE_EXPRESSION_EVALUATION_H__
#define __ACT_OF_ROSE_EXPRESSION_EVALUATION_H__

namespace ActOfRose
{
	namespace AST
	{
		// Types of nodes for expression's AST
		enum EExprASTNodeType
		{
			EESTNTOperator,						// Operator node
			EESTNTOperand,						// Operand node
			EESTNTRoundBracket,					// Round bracket (left) node
			EESTNTCurlyBracket					// Curly bracket (left) node
		};

	} // !namespace AST
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_EXPRESSION_EVALUATION_H__
