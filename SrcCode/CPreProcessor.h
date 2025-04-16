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
#include <fstream>
#include <vector>
#include <filesystem>


namespace ActOfRose
{
	namespace Value
	{
		class CValue;						// Abstract class of value / data type
	} // !namespace Value

	
	/*[
		Operation-related elements
	 ]*/

	// Types of preprocessor's operations
	enum EPreProcessorOperationTypes
	{
		EPPOT_SetValue,						// Operation of setting a predefined value associated with a specified identifier
		EPPOT_RemoveValue					// Operation of removing a predefined value associated with a specified identifier
	};

	// Base structure of preprocessor's operation declaration
	struct SPreProcessorOperation
	{
		// Constructor
		SPreProcessorOperation(ActOfRose::EPreProcessorOperationTypes _type) :
			type(_type)
		{}

		// Destructor
		virtual ~SPreProcessorOperation() {}


		ActOfRose::EPreProcessorOperationTypes type;			// Operation type
	};

	// Structure of declaration of preprocessor's operation for setting/adding a predefined value
	struct SPreProcessorSetValueOperation : public SPreProcessorOperation
	{
		// Constructor
		SPreProcessorSetValueOperation() :
			SPreProcessorOperation(ActOfRose::EPreProcessorOperationTypes::EPPOT_SetValue)
		{}

		// Destructor
		~SPreProcessorSetValueOperation() {}


		std::string identifier;						// Identifier associated with a value
		std::string value;							// String containing a predefined value
	};

	// Structure of declaration of preprocessor's operation for removing a predefined value
	struct SPreProcessorRemoveValueOperation : public SPreProcessorOperation
	{
		// Constructor
		SPreProcessorRemoveValueOperation() :
			SPreProcessorOperation(ActOfRose::EPreProcessorOperationTypes::EPPOT_RemoveValue)
		{}

		// Destructor
		~SPreProcessorRemoveValueOperation() {}


		std::string identifier;						// Identifier associated with a value
	};


	// Class of preprocessor for extracting predefined values from a cache file and from a command line
	class CPreProcessor
	{
	public:
		// Constructor
		CPreProcessor() {}

	public:
		// Processes a command line stream
		int ProcessCommandLine(int argCount, char** args);

		// Processes data from a cache file associated with a script file
		int ProcessCache(std::ifstream* cacheStream);


		// Returns a pointer to the string with custom path to a root script
		std::filesystem::path* GetCustomRootScriptPath() { return &_mRootScriptCustomPath; }

		// Returns a pointer to a predefined value associated with a specified name pointed to by elName and extracts it from map of associations if possible
		bool ExtractPredefinedValue(ActOfRose::Value::CValue** valueHolder, const char* elName);

	private:
	#if defined (WIN32) || defined (_WIN32)
		// Processes parameters/arguments in the default execution mode for later execution of scripts
		int ProcessParametersInExecMode(int argCount, std::wstring* args, std::vector<ActOfRose::SPreProcessorOperation*>* opDeclArr);
		// Processes parameters/arguments in the cache editing mode
		int ProcessParametersInCacheEditMode(int argCount, std::wstring* args, std::vector<ActOfRose::SPreProcessorOperation*>* opDeclArr);
	#elif defined (__linux__)
	#endif
		// Executes operations from the array of such
		int ExecuteOperations(std::vector<ActOfRose::SPreProcessorOperation*>* opDeclArr);
	
		// Sets a predefined value
		int SetPredefinedValue(const char* elName, const char* valueStr);

		// Checks if the string with a value contains a number
		bool CheckIfPredefinedValueIsNumber(const char* valueStr);

		// Parses cache file data and creates a map of associations between identifiers of potentially declared variables/constants and predefined values
		int ParseCacheFileData(std::map<const std::string, ActOfRose::Value::CValue*>* map, std::ifstream* cacheStream);

	private:
		// String with custom path to a file with a script
		std::filesystem::path _mRootScriptCustomPath;
		// Map of associations between identifiers of potentially declared variables/constants and predefined values
		std::map<const std::string, ActOfRose::Value::CValue*> _mPredefValueMap;

	};

} // !namespace ActOfRose


extern ActOfRose::CPreProcessor			gPreprocessor;				// Global instance of executor

#endif // !__ACT_OF_ROSE_PREPROCESSOR_CLASS_H__
