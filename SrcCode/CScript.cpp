/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CScript.cpp
	@Created:		12.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CScript.h"

#if defined (WIN32) || defined (_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	#include <stdlib.h>
#endif

#include "ErrorCodes.h"
#include "Utility/StringConverting.h"


// ----- ActOfRose::CScript class -----

// Constructor that takes a path contained in the null-terminated UTF-8-encoded byte string
ActOfRose::CScript::CScript(const char* scriptFilePath)
{
#if defined (WIN32) || defined (_WIN32)
	std::wstring utf16BEPath;
	if (ConvertStringUTF8ToUTF16BE(&utf16BEPath, scriptFilePath, std::strlen(scriptFilePath)) == AOR_ERROR_SUCCESS)
	{
		int ansiStrLength = WideCharToMultiByte(CP_ACP, 0, utf16BEPath.c_str(), (-1), NULL, 0, 0, 0);
		char* ansiPath = (char*)malloc(ansiStrLength);
		WideCharToMultiByte(CP_ACP, 0, utf16BEPath.c_str(), (-1), ansiPath, ansiStrLength, 0, 0);

		_mScriptFile.open(ansiPath);

		free(ansiPath);
	}
#elif defined (__linux__)
	_mScriptFile.open(scriptFilePath);
#endif
}

// Constructor that takes a path contained in the null-terminated UTF-16BE-encoded wide string
ActOfRose::CScript::CScript(const wchar_t* scriptFilePath)
{
#if defined (WIN32) || defined (_WIN32)
	int ansiStrLength = WideCharToMultiByte(CP_ACP, 0, scriptFilePath, (-1), NULL, 0, 0, 0);
	char* ansiPath = (char*)malloc(ansiStrLength);
	WideCharToMultiByte(CP_ACP, 0, scriptFilePath, (-1), ansiPath, ansiStrLength, 0, 0);

	_mScriptFile.open(ansiPath);

	free(ansiPath);
#elif defined (__linux__)
	std::string utf8Path;

	if (ConvertStringUTF16BEToUTF8(&utf8Path, scriptFilePath, std::wcslen(scriptFilePath)) == AOR_ERROR_SUCCESS)
	{
		_mScriptFile.open(utf8Path);
	}
#endif
}

// Destructor
ActOfRose::CScript::~CScript()
{
	_mScriptFile.close();
}


// Processes and executes a script contained in the file
int ActOfRose::CScript::Execute()
{
	return AOR_ERROR_SUCCESS;
}
