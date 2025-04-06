/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Context.h
	@Created:		22.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Classes of different contexts. */

#ifndef __ACT_OF_ROSE_CONTEXT_CLASSES_H__
#define __ACT_OF_ROSE_CONTEXT_CLASSES_H__

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
			// Constructor
			CContext() {}

			// Destructor
			virtual ~CContext() {}

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) = 0;

		};


		/*[
			Context classes
		 ]*/

		// Class of variable declaration context
		class CVarDeclarationContext : public CContext
		{
			// States of variable declaration context
			enum EVarDeclarationCtxStates
			{
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

		private:
			ActOfRose::Context::CVarDeclarationContext::EVarDeclarationCtxStates _mState;		// Context state

		};

		// Class of expression evaluation context
		class CExpressionEvaluationContext : public CContext
		{
		public:
			// Constructor
			CExpressionEvaluationContext();

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) override;

		private:
			unsigned int _mNestingDepth;			// Nesting depth

		};

	} // !namespace Context
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_CONTEXT_CLASSES_H__
