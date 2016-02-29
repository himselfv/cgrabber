#include <stdio.h>
#include <tchar.h>
#include "windows.h"
#include "util.h"

#define REDIRECTIO
// Else only wait for finish

//#define THREADEDOUTPUT
// Spawn separate reader thread for every pipe, even output and error



#ifdef REDIRECTIO
void makeInheritablePipe(HANDLE* in, HANDLE* out) {
	SECURITY_ATTRIBUTES sa;
	memset(&sa, 0, sizeof(sa));
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(out, in, &sa, 0))
		ThrowLastOsError(L"Cannot create pipe");
}

// "in" is inheritable, "out" stays with us
void makeInputPipe(HANDLE* in, HANDLE* out) {
	// For each handle: create inheritable pair, duplicate our end as non-inheritable, close inheritable version.
	HANDLE tmp;
	makeInheritablePipe(in, &tmp);

	HANDLE proc = GetCurrentProcess();
	if (!DuplicateHandle(proc, tmp, proc, out, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
		DWORD err = GetLastError();
		CloseHandle(in);
		CloseHandle(tmp);
		ThrowOsError(err, L"Cannot configure pipe");
	}
	CloseHandle(tmp);
}

// "in" stays with us, "out" is inheritable
void makeOutputPipe(HANDLE* in, HANDLE* out) {
	HANDLE tmp;
	makeInheritablePipe(&tmp, out);

	HANDLE proc = GetCurrentProcess();
	if (!DuplicateHandle(proc, tmp, proc, in, 0, FALSE, DUPLICATE_SAME_ACCESS))
	{
		DWORD err = GetLastError();
		CloseHandle(out);
		CloseHandle(tmp);
		ThrowOsError(err, L"Cannot configure pipe");
	}
	CloseHandle(tmp);
}


typedef struct {
	HANDLE hFrom;
	HANDLE hTo;
} TRANSLATION_PARAMS, *PTRANSLATION_PARAMS;

DWORD WINAPI TranslationThread(_In_ PTRANSLATION_PARAMS params) {
	char buf[1024];
	unsigned long bread;
	if (!ReadFile(params->hFrom, buf, 1, &bread, NULL)) {
		delete params;
		return -1;
	}
	while (bread > 0) {
		if (!WriteFile(params->hTo, buf, bread, &bread, NULL)
			|| !ReadFile(params->hFrom, buf, 1, &bread, NULL)) {
			delete params;
			return -1;
		}
	}
	return 0;
}

HANDLE RunTranslationThread(HANDLE hFrom, HANDLE hTo) {
	PTRANSLATION_PARAMS params = new TRANSLATION_PARAMS;
	params->hFrom = hFrom;
	params->hTo = hTo;
	return CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(&TranslationThread), params, 0, NULL);
}




void handlePipeError() {
	int err = GetLastError();
	if (err = ERROR_BROKEN_PIPE)
		return; //closed from another end
	ThrowOsError(err);
}

void TryTranslateOutput(HANDLE hFromPipe, HANDLE hTo) {
	char buf[1024];
	unsigned long bread;   //bytes read

	if (!PeekNamedPipe(hFromPipe, NULL, 0, NULL, &bread, NULL)) {
		handlePipeError();
		return;
	}
	while (bread > 0) {
		memset(buf, 0, sizeof(buf));
		if (bread > sizeof(buf)) {
			if (!ReadFile(hFromPipe, buf, sizeof(buf), &bread, NULL)) {
				handlePipeError();
				return;
			}

		}
		else {
			if (!ReadFile(hFromPipe, buf, bread, &bread, NULL)) {
				handlePipeError();
				return;
			}
		}
		if (!WriteFile(hTo, buf, bread, &bread, NULL))
			ThrowLastOsError();
		if (!PeekNamedPipe(hFromPipe, NULL, 0, NULL, &bread, NULL)) {
			handlePipeError();
			return;
		}
	}
}
#endif



PROCESS_INFORMATION pi;

//We want to kill the child process when we are killed, or it'll linger in the background forever
BOOL WINAPI ConsoleCtrlHandler(_In_ DWORD dwCtrlType) {
	if (pi.hProcess != 0)
		TerminateProcess(pi.hProcess, -1);
	return FALSE; //we don't handle it completely
}

int mainproc() {
	memset(&pi, 0, sizeof(pi));

	SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;

#ifndef REDIRECTIO
	//Windows does this automatically but since we have a separate app and all, let's make sure.
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
#else
	HANDLE hWriteStdIn, hReadStdOut, hReadStdErr;
	makeOutputPipe(&hWriteStdIn, &si.hStdInput);
	makeInputPipe(&si.hStdOutput, &hReadStdOut);
	makeInputPipe(&si.hStdError, &hReadStdErr);
#endif
	TCHAR* moduleFilename = GetModuleFilenameStr(0);
	TCHAR* appName = replaceFilename(moduleFilename, L"Grabber.exe");
	TCHAR* cmdLine = buildCommandLine(GetCommandLine(), L" -c");

	if (!CreateProcess(appName, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
		ThrowLastOsError(L"Cannot run the core application.");
	CloseHandle(pi.hThread);

	delete cmdLine;
	delete appName;
	delete moduleFilename;

#ifndef REDIRECTIO
	DWORD res = WaitForSingleObject(pi.hProcess, INFINITE);
#else
	//These went to children process
	CloseHandle(si.hStdInput);
	CloseHandle(si.hStdError);
	CloseHandle(si.hStdOutput);

	HANDLE hInputThread = RunTranslationThread(GetStdHandle(STD_INPUT_HANDLE), hWriteStdIn);
#ifdef THREADEDOUTPUT
	HANDLE hOutputThread = RunTranslationThread(hReadStdOut, GetStdHandle(STD_OUTPUT_HANDLE));
	HANDLE hErrorThread = RunTranslationThread(hReadStdErr, GetStdHandle(STD_ERROR_HANDLE));
#else
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hStdError = GetStdHandle(STD_ERROR_HANDLE);
#endif

	DWORD res = 0;
	do {
#ifndef THREADEDOUTPUT
		TryTranslateOutput(hReadStdOut, hStdOutput);
		TryTranslateOutput(hReadStdErr, hStdError);
#endif
		res = WaitForSingleObject(pi.hProcess, 5);
	} while (res == WAIT_TIMEOUT);


#ifndef THREADEDOUTPUT
	//final part
	TryTranslateOutput(hReadStdOut, hStdOutput);
	TryTranslateOutput(hReadStdErr, hStdError);
#endif

	TerminateThread(hInputThread, -1); //the process ended, no point in even writing there
	CloseHandle(hInputThread);

#ifdef THREADEDOUTPUT
	WaitForSingleObject(hOutputThread, INFINITE); //wait until it reads everything there is
	CloseHandle(hOutputThread);
	WaitForSingleObject(hErrorThread, INFINITE);
	CloseHandle(hErrorThread);
#endif

	CloseHandle(hWriteStdIn);
	CloseHandle(hReadStdErr);
	CloseHandle(hReadStdOut);
#endif

	if (res != WAIT_OBJECT_0)
		ThrowLastOsError(L"Cannot wait on the main process");

	DWORD ExitCode = 0;
	if (!GetExitCodeProcess(pi.hProcess, &ExitCode))
		ThrowLastOsError(L"Cannot retrieve execution result");
	CloseHandle(pi.hProcess);
	return ExitCode;
}


int _tmain(int argc, _TCHAR* argv[])
{
	try {
		return mainproc();
	}
	catch (Win32Error err) {
		fprintf(stderr, "Error %d.", err);
		return -1;
	}
}

