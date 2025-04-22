/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			Main.cpp
	@Created:		12.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	High-level entry point. */

#include "ReturnCodes.h"
#include "Log.h"
#include "Global.h"
#include "Utility/StringMisc.h"


// High-level entry point
int main(int argc, char* argv[])
{
	// ----- Reading of predefined values from arguments -----

	if (argc != 1)
	{
		int result = gPreprocessor.ProcessCommandLine(argc, argv);
		switch (result)
		{
			case AOR_SUCCESS:
			{
				break;
			}

			case AOR_PREPROCESSOR_EXIT:
			{
				return 0;
			}

			default:
			{
				return result;
			}
		}
	}


	// ----- Loading of a script -----

	if (gPreprocessor.GetCustomRootScriptPath()->empty() == false)
	{
		gRootScriptPath = *(gPreprocessor.GetCustomRootScriptPath());
	}
	
	ActOfRose::Script::CScript rootScript(&gRootScriptPath);
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


	// ----- Loading of a cache file associated with -----

	{
		std::filesystem::path cacheFilePath = gRootScriptPath.parent_path();
		cacheFilePath += gRootScriptPath.stem();
		cacheFilePath += ".racache";

		std::fstream cacheFile(cacheFilePath, std::ios::in);
		if (cacheFile.is_open() == true)
		{
			ActOfRose::WriteLog(PREF_STRING("Cache file has been found"), (sizeof(PREF_STRING("Cache file has been found")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Info);

			int result = gPreprocessor.ProcessCache(&cacheFile);
			cacheFile.close();

			if (result != AOR_SUCCESS)
			{
				return result;
			}
		}
	}


	// ----- Execution -----

	return rootScript.Execute();
}
