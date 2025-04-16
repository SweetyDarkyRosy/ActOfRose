/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CPreProcessor.cpp
	@Created:		12.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CPreProcessor.h"

#if defined (WIN32) || defined (_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	#include <stdlib.h>

	#include <vector>
#endif

#include <string>
#include <fstream>

#include "ReturnCodes.h"
#include "Log.h"
#include "Token.h"
#include "SystemAPI.h"
#include "Utility/StringMisc.h"
#include "Utility/StringConverting.h"
#include "Value/Value.h"


#define CACHE_FILE_DEFAULT_NAME						"ActionScript.racache"
#define CACHE_FILE_DEFAULT_NAME_PREF				PREF_STRING("ActionScript.racache")


#if defined (WIN32) || defined (_WIN32)
// Checks if the character is a non-printable character
static inline bool IsWhitespaceUTF16BE(wchar_t value)
{
	return ((value == '\t') || (value == ' ') || (value == 0xA) || (value == 0xD));
}
#endif

// Checks if the character is a non-printable character
static inline bool IsWhitespace(char value)
{
	return ((value == '\t') || (value == ' ') || (value == 0xA) || (value == 0xD));
}

// Checks if the character value is an alphabetic character
static inline bool IsAlphabetic(char value)
{
	return ((('A' <= value) && (value <= 'Z')) || (('a' <= value) && (value <= 'z')));
}

// Checks if the character value is a digit
static inline bool IsDigit(char value)
{
	return (('0' <= value) && (value <= '9'));
}


// ----- ActOfRose::CPreProcessor class -----

// Returns a pointer to a predefined value associated with a specified name pointed to by elName and extracts it from map of associations if possible
bool ActOfRose::CPreProcessor::ExtractPredefinedValue(ActOfRose::Value::CValue** valueHolder, const char* elName)
{
	std::map<const std::string, ActOfRose::Value::CValue*>::iterator predefValueAssocIt = _mPredefValueMap.find(elName);
	if (predefValueAssocIt == _mPredefValueMap.end())
	{
		return false;
	}

	std::pair<const std::string, ActOfRose::Value::CValue*>* predefValuePair = &(*predefValueAssocIt);
	*valueHolder = predefValuePair->second;

	_mPredefValueMap.erase(predefValueAssocIt);
	
	return true;
}

// Processes a command line stream
int ActOfRose::CPreProcessor::ProcessCommandLine(int argCount, char** args)
{
#if defined (WIN32) || defined (_WIN32)
	// ----- Windows OS-specific preparation of arguments -----

	std::vector<std::wstring> utf16BEArguments;
	{
		const wchar_t* argWalker = GetCommandLineW();

		while (*argWalker != L'\0')
		{
			if (IsWhitespaceUTF16BE(*argWalker) == false)
			{
				std::wstring arg;

				if (*argWalker == L'"')
				{
					argWalker++;

					while ((*argWalker != L'"') && (*argWalker != L'\0'))
					{
						arg += *argWalker;
						argWalker++;
					}

					if (*argWalker == L'"')
					{
						argWalker++;
					}
				}
				else
				{
					while ((IsWhitespaceUTF16BE(*argWalker) == false) && (*argWalker != L'\0'))
					{
						arg += *argWalker;
						argWalker++;
					}
				}

				utf16BEArguments.push_back(arg);
			}
			else
			{
				argWalker++;
			}
		}
	}


	int result;


	// ----- Processing of arguments/parameters -----

	std::vector<ActOfRose::SPreProcessorOperation*> opDeclArr;			// Array of declarations of operations for deferred execution

	if (utf16BEArguments[1].compare(L"cache") == 0)
	{
		// ----- Cache editing mode -----

		result = ProcessParametersInCacheEditMode(argCount, utf16BEArguments.data(), &opDeclArr);
		if (result == AOR_SUCCESS)
		{
			// ----- Loading of cache file -----

			if (_mRootScriptCustomPath.empty() == false)
			{
				std::filesystem::path processedPath = _mRootScriptCustomPath.parent_path();
				processedPath += "/";
				processedPath += _mRootScriptCustomPath.stem();
				processedPath += ".racache";

				_mRootScriptCustomPath = processedPath;
			}
			else
			{
				_mRootScriptCustomPath = CACHE_FILE_DEFAULT_NAME_PREF;
			}

			std::fstream cacheFile;
			
			if (std::filesystem::exists(_mRootScriptCustomPath) == true)
			{
				cacheFile.open(_mRootScriptCustomPath, std::ios::in);
				if (cacheFile.is_open() == true)
				{
					result = ParseCacheFileData(&_mPredefValueMap, &cacheFile);
				}
				else
				{
					ActOfRose::WriteLog(PREF_STRING("Could not open/create a cache file"),
						(sizeof(PREF_STRING("Could not open/create a cache file")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

					result = AOR_ERROR_INTERNAL_ERROR;
				}
			}

			if (result == AOR_SUCCESS)
			{
				// ----- Execution of operations -----

				result = ExecuteOperations(&opDeclArr);
				if (result == AOR_SUCCESS)
				{
					// ----- Truncating and saving changes -----
					
					cacheFile.close();
					cacheFile.open(_mRootScriptCustomPath, std::ios::out | std::ios::trunc);
					if (cacheFile.is_open() == true)
					{
						SaveCacheData(&cacheFile);
					}
					else
					{
						ActOfRose::WriteLog(PREF_STRING("Could not open/create a cache file for saving changes"),
							(sizeof(PREF_STRING("Could not open/create a cache file for saving changes")) / sizeof(PChar)),
							ActOfRose::ELogLevel::ELL_Error);

						result = AOR_ERROR_INTERNAL_ERROR;
					}
				}
			}
		}

		if (result == AOR_SUCCESS)
		{
			result = AOR_PREPROCESSOR_EXIT;
		}
	}
	else
	{
		// ----- Default (script execution) mode -----
		
		result = ProcessParametersInExecMode(argCount, utf16BEArguments.data(), &opDeclArr);
		if (result == AOR_SUCCESS)
		{
			// ----- Execution of operations -----

			result = ExecuteOperations(&opDeclArr);
		}
	}


	// ----- Cleanup -----

	for (unsigned int opIt = 0; opIt < (unsigned int)(opDeclArr.size()); opIt++)
	{
		delete opDeclArr[opIt];
	}

#elif defined (__linux__)
	unsigned int currArgIndex = 1;

	while (currArgIndex < (unsigned int)argCount)
	{
		if (std::strcmp(args[currArgIndex], "-P") == 0)
		{
			// ----- If a parameter with a custom path to a root script found -----

			if (((unsigned int)argCount - currArgIndex) < 2)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			std::filesystem::path pathDetected = args[currArgIndex + 1];
			if (std::filesystem::exists(pathDetected) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Custom path to a script file is invalid"),
					(sizeof(PREF_STRING("Custom path to a script file is invalid")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}
			else if (std::filesystem::is_regular_file(pathDetected) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Specified path does not refer to a file"),
					(sizeof(PREF_STRING("Specified path does not refer to a file")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}

			_mRootScriptCustomPath = pathDetected;

			currArgIndex += 2;
		}
		else if (std::strcmp(args[currArgIndex], "-D") == 0)
		{
			// ----- If a parameter with a custom path to a root script found -----

			if (((unsigned int)argCount - currArgIndex) < 3)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			int procResult = SetPredefinedValue(args[currArgIndex + 1], args[currArgIndex + 2]);
			if (procResult != AOR_SUCCESS)
			{
				return procResult;
			}

			currArgIndex += 3;
		}
		else
		{
			ActOfRose::WriteLog(PREF_STRING("Invalid parameter"), (sizeof(PREF_STRING("Invalid parameter")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_INVALID_PARAMETER;
		}
	}

	int result = AOR_SUCCESS;
#endif

	return result;
}

// Processes data from a cache file associated with a script file
int ActOfRose::CPreProcessor::ProcessCache(std::fstream* cacheStream)
{
	cacheStream->seekg(0, std::ios_base::beg);

	//std::map<const std::string, ActOfRose::Value::CValue*> assocMap;

	//int result = ParseCacheFileData(&assocMap, cacheStream);
	int result = ParseCacheFileData(&_mPredefValueMap, cacheStream);
	if (result != AOR_SUCCESS)
	{
		return result;
	}

	return AOR_SUCCESS;
}

// Executes operations from the array of such
int ActOfRose::CPreProcessor::ExecuteOperations(std::vector<ActOfRose::SPreProcessorOperation*>* opDeclArr)
{
	for (unsigned int opIt = 0; opIt < (unsigned int)(opDeclArr->size()); opIt++)
	{
		if ((*opDeclArr)[opIt]->type == ActOfRose::EPreProcessorOperationTypes::EPPOT_SetValue)
		{
			ActOfRose::SPreProcessorSetValueOperation* setValueOp = (ActOfRose::SPreProcessorSetValueOperation*)((*opDeclArr)[opIt]);
			int result = SetPredefinedValue(setValueOp->identifier.c_str(), setValueOp->value.c_str());
			if (result != AOR_SUCCESS)
			{
				return result;
			}
		}
	}

	return AOR_SUCCESS;
}

// Processes a predefined value
int ActOfRose::CPreProcessor::SetPredefinedValue(const char* elName, const char* valueStr)
{
	// ----- Checking if the element name contains only latin symbols, numbers and '_' -----

	{
		const char* elNameWalker = elName;
		
		if ((IsAlphabetic(*elNameWalker) == false) && (*elNameWalker != '_'))
		{
			ActOfRose::WriteLog(PREF_STRING("Invalid string with identifier associated with a predefined value"),
				(sizeof(PREF_STRING("Invalid string with identifier associated with a predefined value")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_INVALID_PARAMETER;
		}

		elNameWalker++;

		while (*elNameWalker != '\0')
		{
			if ((IsAlphabetic(*elNameWalker) == false) && (IsDigit(*elNameWalker) == false) && (*elNameWalker != '_'))
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid string with identifier associated with a predefined value"),
				(sizeof(PREF_STRING("Invalid string with identifier associated with a predefined value")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}

			elNameWalker++;
		}
	}


	// ----- Checking of the value type -----

	ActOfRose::Token::SToken token;
	token.value = valueStr;

	if (CheckIfPredefinedValueIsNumber(valueStr) == true)
	{
		token.type = ActOfRose::Token::ETokenType::ETTNumber;
	}
	else
	{
		token.type = ActOfRose::Token::ETokenType::ETTString;
	}

	ActOfRose::Value::CValue* valueHolder;

	int valueCreationResult = CreateValueFromToken(&valueHolder, &token);
	if (valueCreationResult != AOR_SUCCESS)
	{
		return valueCreationResult;
	}

	std::map<const std::string, ActOfRose::Value::CValue*>::iterator predefValueAssocIt = _mPredefValueMap.find(elName);
	if (predefValueAssocIt != _mPredefValueMap.end())
	{
		std::pair<const std::string, ActOfRose::Value::CValue*>* predefValuePair = &(*predefValueAssocIt);
		delete predefValuePair->second;
	}

	_mPredefValueMap[elName] = valueHolder;

#ifdef _DEBUG
	{
		std::string logMsg = "Predefined value detected (in command line). " + std::string(elName) + ": " + valueHolder->ConvertValueToByteString() +
			" (" + valueHolder->GetTypeByteString() + ")";
		ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
	}
#endif

	return AOR_SUCCESS;
}

// Checks if the string with a value contains a number
bool ActOfRose::CPreProcessor::CheckIfPredefinedValueIsNumber(const char* valueStr)
{
	bool isDotFound = false;
	const char* valueWalker = valueStr;

	if ((IsDigit(*valueWalker) == false) && (*valueWalker != '-') && (*valueWalker != '+') &&
		(*valueWalker != '.'))
	{
		return false;
	}

	valueWalker++;

	while (*valueWalker != '\0')
	{
		if ((IsDigit(*valueWalker) == false) && (*valueWalker != '.'))
		{
			return false;
		}

		if (*valueWalker == '.')
		{
			if (isDotFound == false)
			{
				isDotFound = true;
			}
			else
			{
				return false;
			}
		}

		valueWalker++;
	}

	return true;
}

// Parses cache file data and creates a map of associations between identifiers of potentially declared variables/constants and predefined values
int ActOfRose::CPreProcessor::ParseCacheFileData(std::map<const std::string, ActOfRose::Value::CValue*>* map, std::fstream* cacheStream)
{
	while (cacheStream->peek() != EOF)
	{
		char retrievedChar = cacheStream->peek();

		if (IsWhitespace(retrievedChar) == true)
		{
			cacheStream->get();
		}
		else
		{
			std::string identifier;
			std::string value;


			// ----- Retrieving of identifier -----

			if ((IsAlphabetic(retrievedChar) == false) && (retrievedChar != '_'))
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid string with identifier associated with a predefined value"),
					(sizeof(PREF_STRING("Invalid string with identifier associated with a predefined value")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}
			else
			{
				while ((retrievedChar = cacheStream->peek()) != EOF)
				{
					if ((IsAlphabetic(retrievedChar) == true) || (IsDigit(retrievedChar) == true) || (retrievedChar == '_'))
					{
						identifier += cacheStream->get();
					}
					else if ((IsWhitespace(retrievedChar) == true) || (retrievedChar == ':'))
					{
						break;
					}
					else
					{
						ActOfRose::WriteLog(PREF_STRING("Invalid string with identifier associated with a predefined value"),
							(sizeof(PREF_STRING("Invalid string with identifier associated with a predefined value")) / sizeof(PChar)),
							ActOfRose::ELogLevel::ELL_Error);

						return AOR_ERROR_INVALID_PARAMETER;
					}
				}

				if (cacheStream->peek() == EOF)
				{
					ActOfRose::WriteLog(PREF_STRING("Premature end of cache data"),
						(sizeof(PREF_STRING("Premature end of cache data")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_INVALID_PARAMETER;
				}
			}


			// ----- Skipping of whitespaces -----

			while ((retrievedChar = cacheStream->peek()) != EOF)
			{
				if (IsWhitespace(retrievedChar) == true)
				{
					cacheStream->get();
				}
				else
				{
					break;
				}
			}

			if (cacheStream->peek() == EOF)
			{
				ActOfRose::WriteLog(PREF_STRING("Premature end of cache data"),
					(sizeof(PREF_STRING("Premature end of cache data")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}


			// ----- Checking for the colon character -----

			if (cacheStream->get() != ':')
			{
				ActOfRose::WriteLog(PREF_STRING("Expected ':' while retrieving of a predefined value from cache file"),
					(sizeof(PREF_STRING("Expected ':' while retrieving of a predefined value from cache file")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}

			if (cacheStream->peek() == EOF)
			{
				ActOfRose::WriteLog(PREF_STRING("Premature end of cache data"),
					(sizeof(PREF_STRING("Premature end of cache data")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}


			// ----- Skipping of whitespaces -----

			while ((retrievedChar = cacheStream->peek()) != EOF)
			{
				if (IsWhitespace(retrievedChar) == true)
				{
					cacheStream->get();
				}
				else
				{
					break;
				}
			}

			if (cacheStream->peek() == EOF)
			{
				ActOfRose::WriteLog(PREF_STRING("Premature end of cache data"),
					(sizeof(PREF_STRING("Premature end of cache data")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}


			// ----- Retrieving of value -----

			if (retrievedChar == '\"')
			{
				cacheStream->get();
				
				if (cacheStream->peek() == EOF)
				{
					ActOfRose::WriteLog(PREF_STRING("Premature end of cache data"),
						(sizeof(PREF_STRING("Premature end of cache data")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_INVALID_PARAMETER;
				}

				while ((retrievedChar = cacheStream->peek()) != EOF)
				{
					retrievedChar = cacheStream->get();

					if (retrievedChar == '\"')
					{
						break;
					}

					value += retrievedChar;
				}
			}
			else
			{
				while ((retrievedChar = cacheStream->peek()) != EOF)
				{
					if ((IsWhitespace(retrievedChar) == true) || (retrievedChar == ';'))
					{
						break;
					}

					value += cacheStream->get();
				}
			}


			// ----- Skipping of whitespaces -----

			while ((retrievedChar = cacheStream->peek()) != EOF)
			{
				if (IsWhitespace(retrievedChar) == true)
				{
					cacheStream->get();
				}
				else
				{
					break;
				}
			}

			if (cacheStream->peek() == EOF)
			{
				ActOfRose::WriteLog(PREF_STRING("Premature end of cache data"),
					(sizeof(PREF_STRING("Premature end of cache data")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}


			// ----- Checking for the semicolon character -----

			if (cacheStream->get() != ';')
			{
				ActOfRose::WriteLog(PREF_STRING("Expected ';' while retrieving of a predefined value from cache file"),
					(sizeof(PREF_STRING("Expected ';' while retrieving of a predefined value from cache file")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_PARAMETER;
			}


			// ----- Checking if the identifier is already in association map -----

			std::map<const std::string, ActOfRose::Value::CValue*>::iterator predefValueAssocIt = _mPredefValueMap.find(identifier);
			if (predefValueAssocIt == _mPredefValueMap.end())
			{
				// ----- Setting of a predefined value -----

				ActOfRose::Token::SToken token;
				token.value = value;

				if (CheckIfPredefinedValueIsNumber(value.c_str()) == true)
				{
					token.type = ActOfRose::Token::ETokenType::ETTNumber;
				}
				else
				{
					token.type = ActOfRose::Token::ETokenType::ETTString;
				}

				ActOfRose::Value::CValue* valueHolder;

				int valueCreationResult = CreateValueFromToken(&valueHolder, &token);
				if (valueCreationResult != AOR_SUCCESS)
				{
					return valueCreationResult;
				}

				(*map)[identifier] = valueHolder;

			#ifdef _DEBUG
				{
					std::string logMsg = "Predefined value detected (in cache data). " + identifier + ": " + valueHolder->ConvertValueToByteString() +
						" (" + valueHolder->GetTypeByteString() + ")";
					ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
				}
			#endif
			}
		}
	}

	return AOR_SUCCESS;
}

// Saves the data with predefined values to a specified file
int ActOfRose::CPreProcessor::SaveCacheData(std::fstream* cacheStream)
{
	std::map<const std::string, ActOfRose::Value::CValue*>::iterator predefValueIt = _mPredefValueMap.begin();

	for (; predefValueIt != _mPredefValueMap.end(); predefValueIt++)
	{
		std::pair<const std::string, ActOfRose::Value::CValue*>* predefValuePair = &(*predefValueIt);

		(*cacheStream) << predefValuePair->first << ":";
		
		if (predefValuePair->second->GetValueType() == ActOfRose::Value::EValueType::EVT_String)
		{
			(*cacheStream) << "\"" << predefValuePair->second->ConvertValueToByteString() << "\"";
		}
		else
		{
			(*cacheStream) << predefValuePair->second->ConvertValueToByteString();
		}
		
		(*cacheStream)<< ";\n";
	}

	return AOR_SUCCESS;
}
