/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of utility set source code.
	
	@Name:			Log.h
	@Created:		12.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Functions for logging. */

#ifndef __ACT_OF_ROSE_LOGGING_H__
#define __ACT_OF_ROSE_LOGGING_H__

#include <cstddef>


namespace ActOfRose
{
	// Log leves
	enum ELogLevel
	{
		ELL_Info = 0,				// Information
		ELL_Debug = 1,				// Debug
		ELL_Warning = 2,			// Warning
		ELL_Error = 3				// Error
	};

	// Writes a given null-terminated UTF-8-encoded message as a log with the specified log level
	void WriteLog(const char* utf8Msg, std::size_t msgLength, ActOfRose::ELogLevel level);

	// Writes a given null-terminated UTF-16BE-encoded message as a log with the specified log level
	void WriteLog(const wchar_t* utf16BEMsg, std::size_t msgLength, ActOfRose::ELogLevel level);

} // !namespace ActOfRose

#endif // !__ACT_OF_ROSE_LOGGING_H__
