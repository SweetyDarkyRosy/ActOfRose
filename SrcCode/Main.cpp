/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Main.cpp
	@Created:		12.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	High-level entry point. */

#include <map>
#include <filesystem>

#include "ReturnCodes.h"
#include "Log.h"
#include "Element.h"
#include "CScript.h"
#include "CPreProcessor.h"
#include "CExecutor.h"
#include "Utility/StringMisc.h"


#define BUILD_ROOT_SCRIPT_DEFAULT_NAME						"ActionScript.aor"
#define BUILD_ROOT_SCRIPT_DEFAULT_NAME_PREF					PREF_STRING("ActionScript.aor")


/*[
	Global variables
 ]*/

ActOfRose::CExecutor							gExecutor;			// Global instance of executor
ActOfRose::CPreProcessor						gPreprocessor;		// Global instance of preprocessor
std::map<std::string, ActOfRose::SElement>		gIdentifierMap;		// Map of associations between identifiers and elements
std::filesystem::path							gRootScriptPath;	// Path to a file with a root script


// High-level entry point
int main(int argc, char* argv[])
{
	// ----- Reading of predefined values from arguments -----

	if (argc != 1)
	{
		int result = gPreprocessor.ProcessCommandLine(argc, argv);
		if (result != AOR_SUCCESS)
		{
			return result;
		}
	}


	// ----- Loading of a script -----

	if (gPreprocessor.GetCustomRootScriptPath()->empty() == true)
	{
		gRootScriptPath = BUILD_ROOT_SCRIPT_DEFAULT_NAME_PREF;
	}
	else
	{
		gRootScriptPath = *(gPreprocessor.GetCustomRootScriptPath());
	}
	
	ActOfRose::CScript rootScript(&gRootScriptPath);
	if (rootScript.IsLoaded() == false)
	{
	#if defined (WIN32) || defined (_WIN32)
		std::wstring errMsg = L"Could not open the \"";
		errMsg += gRootScriptPath.wstring().c_str();
		errMsg += L"\" script file";
	#elif defined (__linux__)
		std::string errMsg = "Could not open the \"";
		errMsg += gRootScriptPath.string();
		errMsg += "\" script file";
	#endif

		ActOfRose::WriteLog(errMsg.c_str(), errMsg.length(), ActOfRose::ELogLevel::ELL_Error);
		
		return AOR_ERROR_SCRIPT_FILE_NOT_LOADED;
	}


	// ----- Execution -----

	return rootScript.Execute();
}
