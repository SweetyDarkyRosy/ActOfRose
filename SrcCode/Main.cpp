/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Main.cpp
	@Created:		12.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	High-level entry point. */

#include <string>
#include <map>

#include "ReturnCodes.h"
#include "Log.h"
#include "CScript.h"
#include "CExecutor.h"
#include "Element.h"
#include "Utility/StringMisc.h"


#define BUILD_ROOT_SCRIPT_DEFAULT_NAME						"ActionScript.aor"
#define BUILD_ROOT_SCRIPT_DEFAULT_NAME_PREF					PREF_STRING("ActionScript.aor")


/*[
	Global variables
 ]*/

ActOfRose::CExecutor							gExecutor;			// Global instance of executor
std::map<std::string, ActOfRose::SElement>		gIdentifierMap;		// Map of associations between identifiers and elements


// High-level entry point
int main(int argc, char* argv[])
{
	// ----- Reading of predefined values from arguments -----

	if (argc != 1)
	{
		
	}


	// ----- Loading of a script -----

	ActOfRose::CScript rootScript(BUILD_ROOT_SCRIPT_DEFAULT_NAME_PREF);
	if (rootScript.IsLoaded() == false)
	{
		ActOfRose::WriteLog(PREF_STRING("Could not open the \"") BUILD_ROOT_SCRIPT_DEFAULT_NAME_PREF PREF_STRING("\" script file"),
			(sizeof(PREF_STRING("Could not open the \"") BUILD_ROOT_SCRIPT_DEFAULT_NAME_PREF PREF_STRING("\" script file")) / sizeof(PChar)),
			ActOfRose::ELogLevel::ELL_Error);
		
		return AOR_ERROR_SCRIPT_FILE_NOT_LOADED;
	}


	// ----- Execution -----

	return rootScript.Execute();
}
