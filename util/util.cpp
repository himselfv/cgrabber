#include "util.h"
#include <tchar.h>


void ThrowOsError(DWORD errorCode, TCHAR* msg) {
	throw new Win32Error(errorCode);
}

void ThrowLastOsError(TCHAR* msg) {
	throw new Win32Error(GetLastError(), msg);
}


TCHAR* GetModuleFilenameStr(HMODULE hModule) {
	size_t bufsize = MAX_PATH;
	TCHAR* buf = new TCHAR[bufsize];

	DWORD res = GetModuleFileName(hModule, buf, bufsize);
	while (res == bufsize) {
		bufsize = bufsize + MAX_PATH; //expand
		delete buf;
		buf = new TCHAR[bufsize];
		res = GetModuleFileName(hModule, buf, bufsize);
	}
	if (res == 0)
		ThrowLastOsError(L"Cannot get module filename");

	return buf;
}

TCHAR* replaceFilename(TCHAR* path, TCHAR* fname) {
	size_t plen = wcslen(path);
	size_t oldLen;
	TCHAR* res;

	TCHAR *dir = wcsrchr(path, L'\\');

	if (dir == NULL)
		oldLen = plen;
	else
		oldLen = wcslen(dir + 1);

	size_t newLen = wcslen(fname);
	res = new TCHAR[plen - oldLen + newLen + 1];
	if (plen > oldLen)
		memcpy(res, path, (plen - oldLen)*sizeof(TCHAR));
	if (newLen > 0)
		memcpy(&res[plen - oldLen], fname, newLen*sizeof(TCHAR));
	res[plen - oldLen + newLen] = '\0';

	return res;
}

TCHAR* buildCommandLine(TCHAR* base, TCHAR* additionalArgs) {
	size_t baseLen = _tcslen(base);
	size_t argsLen = _tcslen(additionalArgs);

	TCHAR* cmdLine = new TCHAR[baseLen + argsLen + 1];
	TCHAR* ptr = cmdLine;
	memcpy(ptr, base, baseLen*sizeof(TCHAR));
	ptr += baseLen;
	memcpy(ptr, additionalArgs, argsLen*sizeof(TCHAR));
	ptr += argsLen;
	*ptr = '\0';
	return cmdLine;
}