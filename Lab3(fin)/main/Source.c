#include <Windows.h>
#include <stdio.h>

typedef void(__cdecl* PROC1)(char*, char*);

int main() {
	char str[] = "text2";

	printf(str);
	printf("\n");

	HINSTANCE hLib = LoadLibrary(TEXT("change.dll"));

	PROC1 Proc = (PROC1)GetProcAddress(hLib, "ReplaceString");
	if (Proc != NULL) {
		Proc(str, "text1");
	}

	FreeLibrary(hLib);

	printf(str);

	scanf_s(str);

	return 0;
}

