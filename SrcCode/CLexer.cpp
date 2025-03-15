/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CLexer.cpp
	@Created:		15.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CLexer.h"


// ----- ActOfRose::CLexer class -----

// Constructor
ActOfRose::CLexer::CLexer(std::istream* scriptStream) :
	_pScriptStream(scriptStream)
{}
