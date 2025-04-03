/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Expression.h
	@Created:		26.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Elements for building AST for expression evaluation. */

#ifndef __ACT_OF_ROSE_EXPRESSION_EVALUATION_H__
#define __ACT_OF_ROSE_EXPRESSION_EVALUATION_H__

#include "Operation.h"
#include "Value/Value.h"


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

			// Retrieves a value and sets it to the value reference holder pointed to by valueRefHolder
			virtual int RetrieveValue(ActOfRose::Value::SValueReference* valueRefHolder) = 0;

		protected:
			ActOfRose::AST::EExprASTNodeType		_mType;			// Node type
			ActOfRose::AST::CExprASTNode*			_pParentNode;	// Pointer to a parent node

		};

		// Class of an operand node for an AST for expression evaluation
		class CExprASTOperandNode : public ActOfRose::AST::CExprASTNode
		{
		public:
			// Constructor
			CExprASTOperandNode(ActOfRose::Value::CValue** valueHolder, ActOfRose::Value::EValueCategories valueCategory,
				ActOfRose::AST::CExprASTNode* parentNode = nullptr);
			// Constructor that takes another value reference to copy data
			CExprASTOperandNode(ActOfRose::Value::SValueReference* valueRef, ActOfRose::AST::CExprASTNode* parentNode = nullptr);

			// Destructor
			~CExprASTOperandNode() {}

		public:
			// Retrieves a value and sets it to the value reference holder pointed to by valueRefHolder
			virtual int RetrieveValue(ActOfRose::Value::SValueReference* valueRefHolder) override;

		private:
			ActOfRose::Value::SValueReference _mValueRef;					// Reference to an associated value

		};

		// Class of an operator node for an AST for expression evaluation
		class CExprASTOperatorNode : public ActOfRose::AST::CExprASTNode
		{
		public:
			// Constructor
			CExprASTOperatorNode(ActOfRose::Operation::EOperationTypes opType, ActOfRose::AST::CExprASTNode* parentNode = nullptr) :
				ActOfRose::AST::CExprASTNode(ActOfRose::AST::EExprASTNodeType::EESTNTOperator, parentNode),
				_mOperationType(opType), _pLeftChild(nullptr), _pRightChild(nullptr)
			{}

			// Destructor
			~CExprASTOperatorNode();

		public:
			// Returns an operation type
			inline ActOfRose::Operation::EOperationTypes GetOperationType() const { return _mOperationType; }

			// Returns the precedence of an operator
			unsigned int GetPrecedence() const;

			// Returns a pointer to the left child
			inline ActOfRose::AST::CExprASTNode* GetLeftChild() { return _pLeftChild; }
			// Sets the node as a left child
			inline void SetLeftChild(ActOfRose::AST::CExprASTNode* node)
			{
				_pLeftChild = node;
				_pLeftChild->SetParent(this);
			}

			// Returns a pointer to the right child
			inline ActOfRose::AST::CExprASTNode* GetRightChild() { return _pRightChild; }
			// Sets the node as a right child
			inline void SetRightChild(ActOfRose::AST::CExprASTNode* node)
			{
				_pRightChild = node;
				_pRightChild->SetParent(this);
			}

			// Retrieves a value and sets it to the value reference holder pointed to by valueRefHolder
			virtual int RetrieveValue(ActOfRose::Value::SValueReference* valueRefHolder) override;

		private:
			ActOfRose::Operation::EOperationTypes _mOperationType;		// Operation type

			ActOfRose::AST::CExprASTNode* _pLeftChild;					// Left child node
			ActOfRose::AST::CExprASTNode* _pRightChild;					// Right child node

		};

	} // !namespace AST
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_EXPRESSION_EVALUATION_H__
