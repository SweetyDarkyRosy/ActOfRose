/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CPreProcessor.h
	@Created:		12.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Class of preprocessor for extracting predefined values from a cache file and from a command line. */

#ifndef __ACT_OF_ROSE_PREPROCESSOR_CLASS_H__
#define __ACT_OF_ROSE_PREPROCESSOR_CLASS_H__

#include <string>
#include <map>
#include <filesystem>


namespace ActOfRose
{
	namespace Value
	{
		class CValue;						// Abstract class of value / data type
	} // !namespace Value

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
		// Returns a pointer to a predefined value associated with a specified name pointed to by elName if possible
		bool GetPredefinedValue(ActOfRose::Value::CValue** valueHolder, const char* elName);

	private:
		// Processes a predefined value
		int ProcessPredefinedValue(const char* elName, const char* valueStr);

		// Checks if the string with a value contains a number
		bool CheckIfPredefinedValueIsNumber(const char* valueStr);

	private:
		// String with custom path to a file with a script
		std::filesystem::path _mRootScriptCustomPath;
		// Map of associations between identifiers of potentially declared variables/constants and predefined values
		std::map<const std::string, ActOfRose::Value::CValue*> _mPredefValueMap;

	};

} // !namespace ActOfRose


extern ActOfRose::CPreProcessor			gPreprocessor;				// Global instance of executor

#endif // !__ACT_OF_ROSE_PREPROCESSOR_CLASS_H__
