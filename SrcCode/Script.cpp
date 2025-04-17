/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Script.cpp
	@Created:		12.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "Script.h"

#if defined (WIN32) || defined (_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	#include <stdlib.h>
#endif

#include "ReturnCodes.h"
#include "Token.h"
#include "CLexer.h"
#include "Log.h"
#include "CExecutor.h"
#include "Utility/StringConverting.h"


// ----- ActOfRose::CScript class -----

// Constructor that takes a path contained in the null-terminated UTF-8-encoded byte string
ActOfRose::CScript::CScript(const char* scriptFilePath)
{
#if defined (WIN32) || defined (_WIN32)
	std::wstring utf16BEPath;
	if (ConvertStringUTF8ToUTF16BE(&utf16BEPath, scriptFilePath, std::strlen(scriptFilePath)) == AOR_SUCCESS)
	{
		int ansiStrLength = WideCharToMultiByte(CP_ACP, 0, utf16BEPath.c_str(), (-1), NULL, 0, 0, 0);
		char* ansiPath = (char*)malloc(ansiStrLength);
		WideCharToMultiByte(CP_ACP, 0, utf16BEPath.c_str(), (-1), ansiPath, ansiStrLength, 0, 0);

		_mScriptFile.open(ansiPath);
		_mScriptPath = ansiPath;

		free(ansiPath);
	}
#elif defined (__linux__)
	_mScriptPath = scriptFilePath;
	_mScriptFile.open(_mScriptPath);
#endif

	_mScriptFile.seekg(0, std::ios_base::beg);
}

// Constructor that takes a path contained in the null-terminated UTF-16BE-encoded wide string
ActOfRose::CScript::CScript(const wchar_t* scriptFilePath)
{
#if defined (WIN32) || defined (_WIN32)
	_mScriptPath = scriptFilePath;
	_mScriptFile.open(_mScriptPath);
#elif defined (__linux__)
	std::string utf8ScriptPath;
	if (ConvertStringUTF16BEToUTF8(&utf8ScriptPath, scriptFilePath, std::wcslen(scriptFilePath)) == AOR_SUCCESS)
	{
		_mScriptPath = utf8ScriptPath;
		_mScriptFile.open(_mScriptPath);
	}
#endif

	_mScriptFile.seekg(0, std::ios_base::beg);
}

// Constructor that takes an STL's path
ActOfRose::CScript::CScript(std::filesystem::path* scriptFilePath) :
	_mScriptFile(*scriptFilePath), _mScriptPath(*scriptFilePath)
{
	_mScriptFile.seekg(0, std::ios_base::beg);
}

// Destructor
ActOfRose::CScript::~CScript()
{
	_mScriptFile.close();
}


// Processes and executes a script contained in the file
int ActOfRose::CScript::Execute()
{
	{
	#if defined (WIN32) || defined (_WIN32)
		std::wstring msg = L"Executing a script from a file at ";
		msg += std::filesystem::absolute(_mScriptPath).wstring();
		msg += L"\n";
	#elif defined (__linux__)
		std::string msg = "Executing a script from a file at ";
		msg += std::filesystem::absolute(_mScriptPath).string();
		msg += "\n";
	#endif

		ActOfRose::WriteLog(msg.c_str(), msg.length(), ActOfRose::ELogLevel::ELL_Info);
	}

	ActOfRose::CLexer lexer(&_mScriptFile);				// Local lexer
	int result;											// Result value

	while (true)
	{
		result = lexer.Tokenise();
		if (result == AOR_TOKEN_END_OF_SCRIPT)
		{
			return AOR_SUCCESS;
		}
		else if (result == AOR_CONTEXT_EXECUTE)
		{
			// Executes a retrieved sequence of tokens
			result = gExecutor.Execute(lexer.GetTokensRetrievedArr());
			if (result != AOR_SUCCESS)
			{
				break;
			}
		}
		else if (result < 0)
		{
			break;
		}
	}

	return result;
}
