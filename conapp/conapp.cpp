#include <stdio.h>
#include <tchar.h>
#include "windows.h"


int _tmain(int argc, _TCHAR* argv[]) {
	wWinMain(0, 0, NULL, 0);
}

int __stdcall wWinMain(_In_ HINSTANCE hInstance,
              _In_opt_ HINSTANCE hPrevInstance,
              _In_ LPTSTR    lpCmdLine,
              _In_ int       nCmdShow)
{

/*
Neither AttachConsole nor AllocConsole works.

  AllocConsole();
  AttachConsole(ATTACH_PARENT_PROCESS);

C initializes stdout/stderr early before _main, in crtlib.c/CRTDLL_INIT -> ioinit.c/_ioinit. It populates
internal.h / __pioinfo[][], where pioinfo[0][0], [1] and[2] and stdin, out and err.
GetStdHandle(*) is checked for GetFileType() and if console is not attached, it's FILE_TYPE_UNKNOWN
and - 2 is saved instead of handle, after which no writes to stdout succeed.

There's no reliable workaround for this apart from digging into internal structures. freopen(stdout, "CONIN$") always
opens console input, even if it's redirected.
*/

/*
This does not work:

  setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
  setvbuf(stderr, NULL, _IOLBF, BUFSIZ);

In Visual C Runtime, _IOLBF is not implemented, treated as _IOBBF. Console output is just not buffered at all:
*/
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	int tmp;
	while (true) {
		printf("Test Test Test Test Test Test Test Test\n");
		fprintf(stdout, "Out Out Out Out Out Out\n");
		fprintf(stderr, "Err Err Err Err Err Err\n");
		//_flushall();
		//scanf("%d", &tmp);
		Sleep(100);
	}

	return 0;
}
