#include <Windows.h>

class Win32Error {
public:
	DWORD errorCode = 0;
	TCHAR* msg = NULL;
	Win32Error(DWORD errorCode, TCHAR* msg = NULL) {
		this->errorCode = errorCode;
		this->msg = msg;
	};
};

void ThrowOsError(DWORD errorCode, TCHAR* msg = NULL);
void ThrowLastOsError(TCHAR* msg = NULL);



// Allocated strings have to be freed with delete[]

TCHAR* GetModuleFilenameStr(HMODULE hModule);
TCHAR* replaceFilename(TCHAR* path, TCHAR* fname); // replaces filename part of a path
TCHAR* buildCommandLine(TCHAR* base, TCHAR* additionalArgs); //builds command line out of existing command line and additional args