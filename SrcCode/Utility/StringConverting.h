/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of utility set source code.
	
	@Name:			StringConverting.h
	@Created:		13.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Functions for convering null-terminated strings between encoding standards. */

#ifndef __ACT_OF_ROSE_UTILITY_C_STYLE_STRING_CONVERTING_H__
#define __ACT_OF_ROSE_UTILITY_C_STYLE_STRING_CONVERTING_H__

#include <string>


// Converts the given UTF-8-encoded null-terminated byte string to a UTF-16BE-encoded null-terminated wide string and returns a result
int ConvertStringUTF8ToUTF16BE(std::wstring* destUTF16BEStr, const char* srcUTF8Str, std::size_t srcLength);

// Converts the given UTF-8-encoded null-terminated byte string to a UTF-16BE-encoded null-terminated wide string and returns a result
int ConvertStringUTF8ToUTF16BE(std::wstring* destUTF16BEStr, const std::string* srcUTF8Str);

// Converts the given UTF-16BE-encoded null-terminated wide string to a UTF-8-encoded null-terminated byte string and returns a result
int ConvertStringUTF16BEToUTF8(std::string* destUTF8Str, const wchar_t* srcUTF16BEStr, std::size_t srcLength);

// Converts the given UTF-16BE-encoded null-terminated wide string to a UTF-8-encoded null-terminated byte string and returns a result
int ConvertStringUTF16BEToUTF8(std::string* destUTF8Str, const std::wstring* srcUTF16BEStr);

#endif // !__ACT_OF_ROSE_UTILITY_C_STYLE_STRING_CONVERTING_H__
