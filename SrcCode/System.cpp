/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			System.cpp
	@Created:		22.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	System's central global elements. */

#include <map>
#include <stack>
#include <filesystem>

#include "Script.h"
#include "Element.h"
#include "Function.h"
#include "CPreProcessor.h"
#include "Utility/StringMisc.h"

#include "BuiltIn/BuiltInFuncOS.h"
#include "BuiltIn/BuiltInFuncScript.h"


#define BUILD_ROOT_SCRIPT_DEFAULT_NAME						"ActionScript.aor"
#define BUILD_ROOT_SCRIPT_DEFAULT_NAME_PREF					PREF_STRING("ActionScript.aor")


/*[
	Global variables
 ]*/

// Wrapper of built-in 'ActOfRose::BuiltIn::Print' function
ActOfRose::CPredefinedFunction								gBuiltInFuncPrint(ActOfRose::BuiltIn::Print);
// Wrapper of built-in 'ActOfRose::BuiltIn::Execute' function
ActOfRose::CPredefinedFunction								gBuiltInFuncExecute(ActOfRose::BuiltIn::Execute);
// Wrapper of built-in 'ActOfRose::BuiltIn::Act' function
ActOfRose::CPredefinedFunction								gBuiltInFuncAct(ActOfRose::BuiltIn::Act);
// Wrapper of built-in 'ActOfRose::BuiltIn::GetCurrentScriptPath' function
ActOfRose::CPredefinedFunction								gBuiltInFuncGetCurrentScriptPath(ActOfRose::BuiltIn::GetCurrentScriptPath);
// Wrapper of built-in 'ActOfRose::BuiltIn::GetRootScriptPath' function
ActOfRose::CPredefinedFunction								gBuiltInFuncGetRootScriptPath(ActOfRose::BuiltIn::GetRootScriptPath);
// Wrapper of built-in 'ActOfRose::BuiltIn::IsCurrentOS' function
ActOfRose::CPredefinedFunction								gBuiltInFuncIsCurrentOS(ActOfRose::BuiltIn::IsCurrentOS);
// Wrapper of built-in 'ActOfRose::BuiltIn::GetCurrentDirPath' function
ActOfRose::CPredefinedFunction								gBuiltInFuncGetCurrentDirPath(ActOfRose::BuiltIn::GetCurrentDirPath);


ActOfRose::CPreProcessor									gPreprocessor;											// Global instance of preprocessor

std::filesystem::path										gRootScriptPath(BUILD_ROOT_SCRIPT_DEFAULT_NAME_PREF);	// Path to a file with a root script
std::stack<ActOfRose::Script::CScriptContext>				gScriptContexts;										// Stack of script contexts

// Map of associations between identifiers and elements
std::map<const std::string, ActOfRose::SElement>			gGlobalIdentifierMap = {
	{ "Print", { ActOfRose::EElementType::EET_Function, &gBuiltInFuncPrint } },
	{ "Execute", { ActOfRose::EElementType::EET_Function, &gBuiltInFuncExecute } },
	{ "Act", { ActOfRose::EElementType::EET_Function, &gBuiltInFuncAct } },
	{ "GetCurrentScriptPath", { ActOfRose::EElementType::EET_Function, &gBuiltInFuncGetCurrentScriptPath } },
	{ "GetRootScriptPath", { ActOfRose::EElementType::EET_Function, &gBuiltInFuncGetRootScriptPath } },
	{ "IsCurrentOS", { ActOfRose::EElementType::EET_Function, &gBuiltInFuncIsCurrentOS } },
	{ "GetCurrentDirPath", { ActOfRose::EElementType::EET_Function, &gBuiltInFuncGetCurrentDirPath } },
};
