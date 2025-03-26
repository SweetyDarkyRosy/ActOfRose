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
	namespace Value
	{
		class CValue;				// Abstract class of value / data type
	} // !namespace Value

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

		// Abstract class of a node for an AST for expression evaluation
		class CExprASTNode
		{
		public:
			// Constructor
			CExprASTNode(ActOfRose::AST::EExprASTNodeType nodeType, ActOfRose::AST::CExprASTNode* parentNode = nullptr) :
				_mType(nodeType), _pParentNode(parentNode)
			{}

			// Destructor
			virtual ~CExprASTNode() {}

		public:
			// Returns the type of an expression AST's node
			inline ActOfRose::AST::EExprASTNodeType GetType() const { return _mType; }

			// Sets a specified node as the parent node
			inline void SetParent(ActOfRose::AST::CExprASTNode* parentNode) { _pParentNode = parentNode; }
			// Returns a pointer to a node specified as a parent node
			inline ActOfRose::AST::CExprASTNode* GetParent() { return _pParentNode; }

			// Retrieves a value and sets it to the value holder pointed to by valueHolder
			virtual int RetrieveValue(ActOfRose::Value::CValue** valueHolder) = 0;

		protected:
			ActOfRose::AST::EExprASTNodeType		_mType;			// Node type
			ActOfRose::AST::CExprASTNode*			_pParentNode;	// Pointer to a parent node

		};

	} // !namespace AST
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_EXPRESSION_EVALUATION_H__
