/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CExecutor.h
	@Created:		20.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of script executor. */

#ifndef __ACT_OF_ROSE_EXECUTOR_CLASS_H__
#define __ACT_OF_ROSE_EXECUTOR_CLASS_H__

#include <vector>

#include "Token.h"


namespace ActOfRose
{
	namespace Value
	{
		class CValue;						// Abstract class of value / data type
		struct SValueReference;				// Structure of value reference
	} // !namespace Value

	namespace AST
	{
		class CExprASTNode;					// Abstract class of a node for an AST for expression evaluation
	} // !namespace AST

	// Class of script executor
	class CExecutor
	{
	public:
		// Constructor
		CExecutor();

	public:
		// Executes an operation based on a token group pointed to by tokenGroup
		int Execute(std::vector<ActOfRose::Token::SToken>* tokenGroup);

	private:
		/**
			Builds an AST for expression evaluation based on given token array and saves a root of the expression AST to a
			pointer pointed to by treeRootNodeHolder
		 */
		int BuildExpressionAST(ActOfRose::AST::CExprASTNode** treeRootNodeHolder);

		// Evaluates an encountered expression
		int EvaluateExpression(ActOfRose::Value::CValue** valueHolder);


		// Executes a variable declaration and initialisation
		int DeclareAndInitialiseVariable(std::vector<ActOfRose::Token::SToken>* tokenGroup);
		
		// Retrieves a value starting in a token with the current index and saved into the value reference pointer to by valueRefHolder
		int RetrieveValue(ActOfRose::Value::SValueReference* valueRefHolder);


		// Logs information about that a specified identifier already used
		void LogAlreadyUsedIdentifier(const char* identifier);

	private:
		std::vector<ActOfRose::Token::SToken>* _pCurrTokenGroup;			// Pointer to a current array of retrieved tokens (token group)
		unsigned int _mCurrTokenIndex;								// Index of current token in the array

	};

} // !namespace ActOfRose


extern ActOfRose::CExecutor			gExecutor;				// Global instance of executor

#endif // !__ACT_OF_ROSE_EXECUTOR_CLASS_H__
