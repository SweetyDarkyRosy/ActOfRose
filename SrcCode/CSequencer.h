/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CSequencer.h
	@Created:		20.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class for analysing sequences of tokens and determining the context. */

#ifndef __ACT_OF_ROSE_SEQUENCER_CLASS_H__
#define __ACT_OF_ROSE_SEQUENCER_CLASS_H__

#include <stack>


namespace ActOfRose
{
	namespace Token
	{
		struct SToken;			// Token structure
	} // !namespace Token

	namespace Context
	{
		class CContext;			// Abstract class of context

		// Class for analysing sequences of tokens and determining the context
		class CSequencer
		{
		public:
			// Constructor
			CSequencer() {}

		public:
			// Analyses the given token and manages current context
			int ProcessToken(ActOfRose::Token::SToken* token);

		private:
			std::stack<ActOfRose::Context::CContext*> _mContexts;		// Stack of contexts

		};

	} // !namespace Context
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_SEQUENCER_CLASS_H__
