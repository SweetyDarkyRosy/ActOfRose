/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CPreProcessor.h
	@Created:		12.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of preprocessor for extracting predefined values from a cache file and from a command line. */

#ifndef __ACT_OF_ROSE_PREPROCESSOR_CLASS_H__
#define __ACT_OF_ROSE_PREPROCESSOR_CLASS_H__

#include <filesystem>


namespace ActOfRose
{
	// Class of preprocessor for extracting predefined values from a cache file and from a command line
	class CPreProcessor
	{
	public:
		// Constructor
		CPreProcessor() {}

	public:
		// Processes a command line stream
		int ProcessCommandLine(int argCount, char** args);


		// Returns a pointer to the string with custom path to a root script
		std::filesystem::path* GetCustomRootScriptPath() { return &_mRootScriptCustomPath; }

	private:
		std::filesystem::path _mRootScriptCustomPath;

	};

} // !namespace ActOfRose


extern ActOfRose::CPreProcessor			gPreprocessor;				// Global instance of executor

#endif // !__ACT_OF_ROSE_PREPROCESSOR_CLASS_H__
