/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Script.h
	@Created:		12.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of script contained in a file. */

#ifndef __ACT_OF_ROSE_SCRIPT_CLASS_H__
#define __ACT_OF_ROSE_SCRIPT_CLASS_H__

#include <fstream>
#include <filesystem>
#include <stack>

#include "CScope.h"


namespace ActOfRose
{
	namespace Script
	{
		// Class of script contained in a file
		class CScript
		{
		public:
			// Constructor that takes a path contained in the null-terminated UTF-8-encoded byte string
			CScript(const char* scriptFilePath);
			// Constructor that takes a path contained in the null-terminated UTF-16BE-encoded wide string
			CScript(const wchar_t* scriptFilePath);
			// Constructor that takes an STL's path
			CScript(std::filesystem::path* scriptFilePath);

			// Destructor
			~CScript();
		public:
			// Processes and executes a script contained in the file
			int Execute();

			// Returns the state of readiness of the file containing a script
			inline bool IsLoaded() const { return _mScriptFile.is_open(); }

		private:
			std::ifstream _mScriptFile;						// File with a script
			std::filesystem::path _mScriptPath;				// Path to a file with a script

		};

		// Class of script context
		class CScriptContext
		{
		public:
			// Constructor
			CScriptContext(ActOfRose::Script::CScript* relatedScript);

		private:
			ActOfRose::Script::CScript* _pRelatedScript;			// Pointer to a related script
			std::stack<ActOfRose::CScope> _mLocalScopes;			// Local scopes in a context of related script

		};
	
	} // !namespace Script
} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_SCRIPT_CLASS_H__
