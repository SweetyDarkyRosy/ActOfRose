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
			CContext(ActOfRose::Context::CContext* parentContext = nullptr) :
				_pParentContext(parentContext)
			{}

		protected:
			ActOfRose::Context::CContext* _pParentContext;

		};

	} // !namespace Context
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_CONTEXT_CLASSES_H__
