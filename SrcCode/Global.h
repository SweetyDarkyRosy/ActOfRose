/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of utility set source code.
	
	@Name:			Global.h
	@Created:		22.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	External access to system's central global elements. */

#ifndef __ACT_OF_ROSE_GLOBAL_H__
#define __ACT_OF_ROSE_GLOBAL_H__

#include <map>
#include <stack>
#include <filesystem>

#include "Script.h"
#include "Element.h"
#include "CPreProcessor.h"


/*[
	Global variables
 ]*/

extern ActOfRose::CPreProcessor								gPreprocessor;				// Global instance of preprocessor

extern std::filesystem::path								gRootScriptPath;			// Path to a file with a root script
extern std::map<const std::string, ActOfRose::SElement>		gGlobalIdentifierMap;		// Map of associations between identifiers and elements
extern std::stack<ActOfRose::Script::CScriptContext>		gScriptContexts;			// Stack of script contexts



#endif // !__ACT_OF_ROSE_GLOBAL_H__
