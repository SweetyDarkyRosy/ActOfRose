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
	namespace Context
	{
		// Abstract class of context
		class CContext
		{
		public:
			// Constructor
			CContext() {}

		public:
			// Analyses the given token, checks current sequence for logical errors and updates a context
			virtual int ProcessToken(ActOfRose::Token::SToken* token) = 0;

		};

	} // !namespace Context
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_CONTEXT_CLASSES_H__
