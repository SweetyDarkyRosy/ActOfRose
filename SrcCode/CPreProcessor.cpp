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

#include "ReturnCodes.h"
#include "Log.h"
#include "Token.h"
#include "SystemAPI.h"
#include "Utility/StringMisc.h"
#include "Utility/StringConverting.h"
#include "Value/Value.h"


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

// Returns a pointer to a predefined value associated with a specified name pointed to by elName if possible
bool ActOfRose::CPreProcessor::GetPredefinedValue(ActOfRose::Value::CValue** valueHolder, const char* elName)
{
	std::map<const std::string, ActOfRose::Value::CValue*>::iterator predefValueAssocIt = _mPredefValueMap.find(elName);
	if (predefValueAssocIt == _mPredefValueMap.end())
	{
		return false;
	}

	std::pair<const std::string, ActOfRose::Value::CValue*>* predefValuePair = &(*predefValueAssocIt);
	*valueHolder = predefValuePair->second;
	
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

	unsigned int currArgIndex = 1;

	while (currArgIndex < (unsigned int)(utf16BEArguments.size()))
	{
		if (utf16BEArguments[currArgIndex].compare(L"-P") == 0)
		{
			// ----- If a parameter with a custom path to a root script found -----

			if (((int)(utf16BEArguments.size()) - (int)currArgIndex) < 2)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			_mRootScriptCustomPath = utf16BEArguments[currArgIndex + 1];

			currArgIndex += 2;
		}
		else if (utf16BEArguments[currArgIndex].compare(L"-D") == 0)
		{
			// ----- If a parameter with a custom path to a root script found -----

			if (((int)(utf16BEArguments.size()) - (int)currArgIndex) < 3)
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid number of arguments"),
					(sizeof(PREF_STRING("Invalid number of arguments")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INVALID_ARG_NUMBER;
			}

			{
				std::string elName;
				std::string valueStr;

				if (ConvertStringUTF16BEToUTF8(&elName, &(utf16BEArguments[currArgIndex + 1])) != AOR_SUCCESS)
				{
					ActOfRose::WriteLog(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8"),
						(sizeof(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_INTERNAL_ERROR;
				}

				if (ConvertStringUTF16BEToUTF8(&valueStr, &(utf16BEArguments[currArgIndex + 2])) != AOR_SUCCESS)
				{
					ActOfRose::WriteLog(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8"),
						(sizeof(PREF_STRING("Internal error. Could not convert an argument from UTF-16BE to UTF-8")) / sizeof(PChar)),
						ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_INTERNAL_ERROR;
				}

				int procResult = ProcessPredefinedValue(elName.c_str(), valueStr.c_str());
				if (procResult != AOR_SUCCESS)
				{
					return procResult;
				}
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

			_mRootScriptCustomPath = args[currArgIndex + 1];

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

			int procResult = ProcessPredefinedValue(args[currArgIndex + 1], args[currArgIndex + 2]);
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
#endif

	return AOR_SUCCESS;
}

// Processes data from a cache file associated with a 
int ActOfRose::CPreProcessor::ProcessCache(std::ifstream* cacheStream)
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

// Processes a predefined value
int ActOfRose::CPreProcessor::ProcessPredefinedValue(const char* elName, const char* valueStr)
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

	/* REMOVED
	std::map<const std::string, ActOfRose::Value::CValue*>::iterator predefValueAssocIt = _mPredefValueMap.find(elName);
	if (predefValueAssocIt == _mPredefValueMap.end())
	{
		std::pair<std::map<const std::string, ActOfRose::Value::CValue*>::iterator, bool> result = _mPredefValueMap.insert({ elName, valueHolder });
	
		if (result.second == false)
		{
			ActOfRose::WriteLog(PREF_STRING("Internal error. Could not set a predefined value into an association map"),
				(sizeof(PREF_STRING("Internal error. Could not set a predefined value into an association map")) / sizeof(PChar)),
				ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_INTERNAL_ERROR;
		}

	#ifdef _DEBUG
		std::string logMsg = "New predefined value " + valueHolder->ConvertValueToByteString() + " (" + std::string(valueHolder->GetTypeByteString()) +
			") for " + std::string(elName) + "has been set";
		ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Debug);
	#endif
	}
	else
	{
		std::string logMsg = "Predefined value for " + std::string(elName) + "had been already set. Updating with " + valueHolder->ConvertValueToByteString() +
			" (" + std::string(valueHolder->GetTypeByteString()) + ")";
		ActOfRose::WriteLog(logMsg.c_str(), logMsg.size(), ActOfRose::ELogLevel::ELL_Warning);

		delete _mPredefValueMap[elName];
		_mPredefValueMap[elName] = valueHolder;
	}
	*/

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
int ActOfRose::CPreProcessor::ParseCacheFileData(std::map<const std::string, ActOfRose::Value::CValue*>* map, std::ifstream* cacheStream)
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
