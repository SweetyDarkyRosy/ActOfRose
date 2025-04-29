/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			BuiltInFuncOS.cpp
	@Created:		22.04.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "BuiltInFuncOS.h"

#if defined (WIN32) || defined (_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif defined (__linux__)
	#include <sys/ptrace.h>
	#include <sys/wait.h>

	#include <unistd.h>
	#include <signal.h>
#endif

#include <cstring>

#include <ReturnCodes.h>
#include <Log.h>
#include <Utility/StringMisc.h>
#include <Utility/StringConverting.h>

#include <Value/CIntegerValue.h>
#include <Value/CStringValue.h>


// Prints a desired value to console
int ActOfRose::BuiltIn::Print(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params)
{
	if (params->size() != 1)
	{
		if (params->size() > 1)
		{
			ActOfRose::WriteLog(PREF_STRING("Too many arguments have been passed"),
				(sizeof(PREF_STRING("Too many arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
		}
		else
		{
			ActOfRose::WriteLog(PREF_STRING("Too few arguments have been passed"),
				(sizeof(PREF_STRING("Too few arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
		}
	}

	returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_None;

	if ((*params)[0] != nullptr)
	{
	#if defined (WIN32) || defined (_WIN32)
		std::wstring utf16BEValueStr = (*params)[0]->ConvertValueToWideString();

		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		WriteConsoleW(consoleHandle, utf16BEValueStr.c_str(), (DWORD)(utf16BEValueStr.length()), NULL, NULL);
		WriteConsoleW(consoleHandle, L"\n", 1, NULL, NULL);
	#elif defined (__linux__)
		std::string utf8ValueStr = (*params)[0]->ConvertValueToByteString();

		write(1, (const void*)(utf8ValueStr.c_str()), utf8ValueStr.length());
		write(1, "\n", 1);
	#endif


		// ----- Cleanup -----

		delete (*params)[0];
	}

	return AOR_SUCCESS;
}

// Executes a command line
int ActOfRose::BuiltIn::Execute(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Value::CValue*>* params)
{
	if (params->size() != 1)
	{
		if (params->size() > 1)
		{
			ActOfRose::WriteLog(PREF_STRING("Too many arguments have been passed"),
				(sizeof(PREF_STRING("Too many arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
		}
		else
		{
			ActOfRose::WriteLog(PREF_STRING("Too few arguments have been passed"),
				(sizeof(PREF_STRING("Too few arguments have been passed")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_EXEC_INVALID_ARGUMENT_NUMBER;
		}
	}

	returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_None;

	if ((*params)[0] != nullptr)
	{
		if ((*params)[0]->GetValueType() != ActOfRose::Value::EValueType::EVT_String)
		{
			ActOfRose::WriteLog(PREF_STRING("Invalid parameter. Command line should be passed using a string"),
				(sizeof(PREF_STRING("Invalid parameter. Command line should be passed using a string")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_EXEC_INVALID_PARAMETER;
		}

	#if defined (WIN32) || defined (_WIN32)
		wchar_t* utf16BECmdRaw = nullptr;
		{
			const std::string* utf8Cmd = ((ActOfRose::Value::CStringValue*)((*params)[0]))->GetSTDString();
			std::wstring utf16BECmd;

			if (ConvertStringUTF8ToUTF16BE(&utf16BECmd, utf8Cmd) != AOR_SUCCESS)
			{
				ActOfRose::WriteLog(PREF_STRING("Internal error. Could not convert a UTF-8-encoded string to UTF-16BE-encoded string"),
					(sizeof(PREF_STRING("Internal error. Could not convert a UTF-8-encoded string to UTF-16BE-encoded string")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_EXEC_INVALID_PARAMETER;
			}

			utf16BECmdRaw = (wchar_t*)malloc(sizeof(wchar_t) * (utf16BECmd.size() + 1));
			std::memcpy((void*)utf16BECmdRaw, (const void*)(utf16BECmd.c_str()), (sizeof(wchar_t) * utf16BECmd.size()));
			utf16BECmdRaw[utf16BECmd.size()] = L'\0';
		}


		// ----- Creating a child process and execution -----

		STARTUPINFOW startupInfo;
		PROCESS_INFORMATION processInformation;

		ZeroMemory(&startupInfo, sizeof(STARTUPINFOW));
		startupInfo.cb = sizeof(STARTUPINFOW);
		ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));

		if (CreateProcessW(NULL, utf16BECmdRaw, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInformation) == 0)
		{
			ActOfRose::WriteLog(PREF_STRING("Internal error. Could not create a child process"),
				(sizeof(PREF_STRING("Internal error. Could not create a child process")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			free(utf16BECmdRaw);
			delete (*params)[0];

			return AOR_ERROR_INTERNAL_ERROR;
		}

		// Waits until a child process exits
		WaitForSingleObject(processInformation.hProcess, INFINITE);

		{
			DWORD exitCode;
			if (GetExitCodeProcess(processInformation.hProcess, &exitCode) == 0)
			{
				ActOfRose::WriteLog(PREF_STRING("Internal error. Could not retrieve the return code from a child process"),
					(sizeof(PREF_STRING("Internal error. Could not retrieve the return code from a child process")) / sizeof(PChar)),
					ActOfRose::ELogLevel::ELL_Error);

				free(utf16BECmdRaw);
				delete (*params)[0];

				return AOR_ERROR_INTERNAL_ERROR;
			}

			returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;
			returnValueHolder->value.value = new ActOfRose::Value::CIntegerValue((int)exitCode);
		}

		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);


		// ----- Cleanup -----

		free(utf16BECmdRaw);

	#elif defined (__linux__)
		const std::string* utf8Cmd = ((ActOfRose::Value::CStringValue*)((*params)[0]))->GetSTDString();
		std::vector<std::string> tempArgArr;
		{
			unsigned int utf8CmdWalkerIndex = 0;
			while (utf8CmdWalkerIndex < (unsigned int)(utf8Cmd->size()))
			{
				if ((*utf8Cmd)[utf8CmdWalkerIndex] != ' ')
				{
					std::string arg;

					if ((*utf8Cmd)[utf8CmdWalkerIndex] == '\"')
					{
						utf8CmdWalkerIndex++;

						while (utf8CmdWalkerIndex < (unsigned int)(utf8Cmd->size()))
						{
							if ((*utf8Cmd)[utf8CmdWalkerIndex] == '\"')
							{
								break;
							}
							else if ((*utf8Cmd)[utf8CmdWalkerIndex] == '\\')
							{
								if ((utf8CmdWalkerIndex < (unsigned int)(utf8Cmd->size() - 1)) &&
									((*utf8Cmd)[utf8CmdWalkerIndex + 1] == '\"'))
								{
									arg += (*utf8Cmd)[utf8CmdWalkerIndex];
									arg += (*utf8Cmd)[utf8CmdWalkerIndex + 1];

									utf8CmdWalkerIndex++;
								}
								else
								{
									arg += (*utf8Cmd)[utf8CmdWalkerIndex];
								}
							}
							else
							{
								arg += (*utf8Cmd)[utf8CmdWalkerIndex];
							}

							utf8CmdWalkerIndex++;
						}
					}
					else if ((*utf8Cmd)[utf8CmdWalkerIndex] == '\'')
					{
						utf8CmdWalkerIndex++;

						while (utf8CmdWalkerIndex < (unsigned int)(utf8Cmd->size()))
						{
							if ((*utf8Cmd)[utf8CmdWalkerIndex] == '\'')
							{
								break;
							}
							else if ((*utf8Cmd)[utf8CmdWalkerIndex] == '\\')
							{
								if ((utf8CmdWalkerIndex < (unsigned int)(utf8Cmd->size() - 1)) &&
									((*utf8Cmd)[utf8CmdWalkerIndex + 1] == '\''))
								{
									arg += (*utf8Cmd)[utf8CmdWalkerIndex];
									arg += (*utf8Cmd)[utf8CmdWalkerIndex + 1];

									utf8CmdWalkerIndex++;
								}
								else
								{
									arg += (*utf8Cmd)[utf8CmdWalkerIndex];
								}
							}
							else
							{
								arg += (*utf8Cmd)[utf8CmdWalkerIndex];
							}

							utf8CmdWalkerIndex++;
						}
					}
					else
					{
						while (utf8CmdWalkerIndex < (unsigned int)(utf8Cmd->size()))
						{
							if ((*utf8Cmd)[utf8CmdWalkerIndex] == ' ')
							{
								break;
							}
							else
							{
								arg += (*utf8Cmd)[utf8CmdWalkerIndex];
							}

							utf8CmdWalkerIndex++;
						}
					}

					tempArgArr.push_back(arg);
				}

				utf8CmdWalkerIndex++;
			}
		}

		// Actual array of arguments for creating a child process
		char** actualArgArr = (char**)malloc(sizeof(char*) * (tempArgArr.size() + 1));
		
		for (unsigned int it = 0; it < (unsigned int)(tempArgArr.size()); it++)
		{
			char* arg = (char*)malloc(tempArgArr[it].size() + 1);
			std::memcpy((void*)arg, (const void*)(tempArgArr[it].c_str()), tempArgArr[it].size());
			arg[tempArgArr[it].size()] = '\0';

			actualArgArr[it] = arg;
		}

		actualArgArr[tempArgArr.size()] = nullptr;


		pid_t procID = fork();
		if (procID < 0)
		{
			for (unsigned int it = 0; it < (unsigned int)(tempArgArr.size()); it++)
			{
				free(actualArgArr[it]);
			}

			free(actualArgArr);

			delete (*params)[0];

			ActOfRose::WriteLog(PREF_STRING("Internal error. Could not create a child process"),
				(sizeof(PREF_STRING("Internal error. Could not create a child process")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_INTERNAL_ERROR;
		}
		else if (procID == 0)
		{
			ptrace(PTRACE_TRACEME, 0, NULL, NULL);

			{
				pid_t currChildProcID = getpid();
				kill(currChildProcID, 19);
			}

			execvp(actualArgArr[0], actualArgArr);
			
			ActOfRose::WriteLog(PREF_STRING("Internal error. Could not create a child process"),
				(sizeof(PREF_STRING("Internal error. Could not create a child process")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			exit(1);
		}
		else
		{	
			int status;

			wait4(procID, &status, 0, NULL);
			ptrace(PTRACE_SETOPTIONS, procID, NULL, (1 << 4));
			ptrace(PTRACE_CONT, procID, NULL, NULL);

			wait4(procID, &status, 0, NULL);
			ptrace(PTRACE_CONT, procID, NULL, NULL);

			if ((status >> 8) != (5 | (4 << 8)))
			{
				for (unsigned int it = 0; it < (unsigned int)(tempArgArr.size()); it++)
				{
					free(actualArgArr[it]);
				}

				free(actualArgArr);

				delete (*params)[0];

				return AOR_ERROR_INTERNAL_ERROR;
			}
		}


		// ----- Waiting -----

		{
			int status;

			size_t returnValue = wait4(procID, &status, 0, NULL);
			if (returnValue > 0)
			{
				int childProcReturnValue = (status & 0xFF00) >> 8;

				returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;
				returnValueHolder->value.value = new ActOfRose::Value::CIntegerValue(childProcReturnValue);
			}
			else
			{
				for (unsigned int it = 0; it < (unsigned int)(tempArgArr.size()); it++)
				{
					free(actualArgArr[it]);
				}

				free(actualArgArr);

				delete (*params)[0];

				ActOfRose::WriteLog(PREF_STRING("Internal error while waiting for a child process"),
					(sizeof(PREF_STRING("Internal error while waiting for a child process")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INTERNAL_ERROR;
			}
		}


		// ----- Cleanup -----

		for (unsigned int it = 0; it < (unsigned int)(tempArgArr.size()); it++)
		{
			free(actualArgArr[it]);
		}

		free(actualArgArr);

	#endif


		// ----- Cleanup -----

		delete (*params)[0];
	}

	return AOR_SUCCESS;
}
