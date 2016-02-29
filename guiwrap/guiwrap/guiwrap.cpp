#include <stdio.h>
#include <tchar.h>
#include "windows.h"
#include "util.h"

int __stdcall wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(pi));

	TCHAR* moduleFilename = GetModuleFilenameStr(0);
	TCHAR* appName = replaceFilename(moduleFilename, L"consolebase.exe");
	TCHAR* cmdLine = buildCommandLine(GetCommandLine(), L" -c");

	if (!CreateProcess(appName, cmdLine, NULL, NULL, TRUE, DETACHED_PROCESS, NULL, NULL, &si, &pi))
		return -1;
	CloseHandle(pi.hThread);

	delete cmdLine;
	delete appName;
	delete moduleFilename;


	int res = 0;
	do {
		res = WaitForSingleObject(pi.hProcess, 50);
	} while (res == WAIT_TIMEOUT);

	DWORD ExitCode = 0;
	if (!GetExitCodeProcess(pi.hProcess, &ExitCode))
		return -1;
	CloseHandle(pi.hProcess);
	return ExitCode;
}
