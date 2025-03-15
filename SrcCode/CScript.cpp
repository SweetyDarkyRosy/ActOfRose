/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CScript.cpp
	@Created:		12.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CScript.h"


// ----- ActOfRose::CScript class -----

// Constructor that takes a path contained in the null-terminated UTF-8-encoded byte string
ActOfRose::CScript::CScript(const char* scriptFilePath)
{}

// Constructor that takes a path contained in the null-terminated UTF-16BE-encoded wide string
ActOfRose::CScript::CScript(const wchar_t* scriptFilePath)
{}

// Destructor
ActOfRose::CScript::~CScript()
{
	_mScriptFile.close();
}
