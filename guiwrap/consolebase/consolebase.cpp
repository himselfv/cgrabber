#include <tchar.h>
#include <stdio.h>
#include "Windows.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (GetConsoleWindow() == 0) {
		//GUI mode
		while (true) {
			if (MessageBox(0, L"Exit?", L"Console base app", MB_YESNO) == IDYES)
				break;
		}
	}
	else {
		//Console mode
		while (true) {
			printf("Hey yo! ");
			Sleep(250);
		}
	}


	return 0;
}

