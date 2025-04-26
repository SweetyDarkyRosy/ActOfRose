/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Context.h
	@Created:		22.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Classes of different contexts. */

#ifndef __ACT_OF_ROSE_CONTEXT_CLASSES_H__
#define __ACT_OF_ROSE_CONTEXT_CLASSES_H__

#include "ReturnCodes.h"
#include "Keywords.h"


namespace ActOfRose
{
	namespace Token
	{
		struct SToken;			// Token structure
	} // !namespace Token

	namespace Context
	{
		// Abstract class of context
		class CContext
		{
		public:
			// Destructor
			virtual ~CContext() {}

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) = 0;

			// Commits finalisation of context with current set of tokens if possible
			virtual int CommitContextFinalisation() = 0;

		};


		/*[
			Context classes
		 ]*/

		// Class of expression evaluation context
		class CExpressionEvaluationContext : public CContext
		{
		public:
			// Constructor
			CExpressionEvaluationContext();

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) override;

			// Commits finalisation of context with current set of tokens if possible
			virtual int CommitContextFinalisation() override { return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT; }

		private:
			unsigned int _mNestingDepth;			// Nesting depth

		};

		// Class of individual independent expression
		class CIndividualExpressionContext : public CContext
		{
			// States of variable declaration context
			enum EIndividualExprCtxStates
			{
				EIECS_Initial,					// Initial state of context
				EIECS_ExpressionEnd,			// Ending of expression (semicolon) is expected
			};

		public:
			// Constructor
			CIndividualExpressionContext();

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) override;

			// Commits finalisation of context with current set of tokens if possible
			virtual int CommitContextFinalisation() override { return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT; }

		private:
			ActOfRose::Context::CIndividualExpressionContext::EIndividualExprCtxStates _mState;		// Context state

		};

		// Class of variable declaration context
		class CVarDeclarationContext : public CContext
		{
			// States of variable declaration context
			enum EVarDeclarationCtxStates
			{
				EVDCS_Initial,						// Initial state of context
				EVDCS_VarKeyword,				// "var" keyword is expected
				EVDCS_VariableName,				// Identifier with variable name is expected
				EVDCS_InitDisjunction,			// Initialisation branching between two options
				EVDCS_ExpressionBegin,			// Expression is expected
				EVDCS_ExpressionEnd,			// Ending of expression (semicolon) is expected
			};

		public:
			// Constructor
			CVarDeclarationContext();

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) override;

			// Commits finalisation of context with current set of tokens if possible
			virtual int CommitContextFinalisation() override { return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT; }

		private:
			ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates _mState;		// Context state

		};

		// Class of function declaration context
		class CFuncDeclarationContext : public CContext
		{
			// States of function declaration context
			enum EFuncDeclarationCtxStates
			{
				EFDCS_Initial,						// Initial state of context
				EFDCS_FuncName,						// Identifier with function name is expected
				EFDCS_ParamStart,					// Left round bracket for opening a sequence of parameter names is expected
				EFDCS_ParamName,					// Parameter name is expected
				EFDCS_ParamDisjunctionInitial,		// Branching between end of parameter name sequence and waiting for parameter name
				EFDCS_ParamDisjunctionSubsequent,	// Branching between end of parameter name sequence and waiting for comma
				EFDCS_BodyStart,					// Left curly bracket is expected to start a function body
				EFDCS_BodyRoutine,					// Some expression, simple statement, compound statement or right curly bracket is expected
				EFDCS_SyntacticUnitEnd,				// Ending of expression, simple statement or compound statement (comma or semicolon) is expected
			};

		public:
			// Constructor
			CFuncDeclarationContext();

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) override;

			// Commits finalisation of context with current set of tokens if possible
			virtual int CommitContextFinalisation() override { return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT; }

		private:
			ActOfRose::Context::CFuncDeclarationContext::EFuncDeclarationCtxStates _mState;		// Context state

		};

		// Class of conditional context
		class CConditionalContext : public CContext
		{
			// States of conditional context
			enum EConditionalCtxStates
			{
				ECCS_Initial,					// Initial state of context
				ECCS_ConditionBegin,			// Left round bracket for expression start is expected
				ECCS_ConditionalExpr,			// Expression for condition is expected
				ECCS_ConditionEnd,				// Right round bracket to end conditional expression is expected
				ECCS_BodyStart,					// Left curly bracket is expected to start a function body
				ECCS_BodyRoutine,				// Some expression, simple statement, compound statement or right curly bracket is expected
				ECCS_SyntacticUnitEnd,			// Ending of expression, simple statement or compound statement (comma or semicolon) is expected
				ECCS_BranchDisjunction,			// The 'elif' keyword, the 'else' keyword or end of branching (right curly bracket) is expected
			};

			// Types of branches
			enum EConditionalCtxBranchTypes
			{
				ECCBT_If,			// "if"
				ECCBT_Elif,			// "elif"
				ECCBT_Else,			// "else"
			};

		public:
			// Constructor
			CConditionalContext();

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) override;

			// Commits finalisation of context with current set of tokens if possible
			virtual int CommitContextFinalisation() override;

		private:
			ActOfRose::Context::CConditionalContext::EConditionalCtxStates _mState;					// Context state
			ActOfRose::Context::CConditionalContext::EConditionalCtxBranchTypes _mLastBranchtype;	// Types of last branch

		};

		// Class of return statement context
		class CReturnStatementContext : public CContext
		{
			// States of return statement context
			enum EReturnStatementCtxStates
			{
				ERSCS_Initial,					// Initial state of context
				ERSCS_ExpressionBegin,			// Expression is expected
				ERSCS_ExpressionEnd,			// Ending of expression (semicolon) is expected
			};

		public:
			// Constructor
			CReturnStatementContext();

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) override;

			// Commits finalisation of context with current set of tokens if possible
			virtual int CommitContextFinalisation() override { return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT; }

		private:
			ActOfRose::Context::CReturnStatementContext::EReturnStatementCtxStates _mState;		// Context state

		};

	} // !namespace Context
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_CONTEXT_CLASSES_H__
