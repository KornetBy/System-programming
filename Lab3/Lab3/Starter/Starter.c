#include <Windows.h>
#include <stdio.h>

typedef void(__cdecl* PROC1)(char*,char*);

int main() {
	char str[] = "Old text";

	printf(str);
	printf("\n");

	HINSTANCE hLib = LoadLibrary(TEXT("ChangeString.dll"));

	PROC1 Proc = (PROC1)GetProcAddress(hLib, "ReplaceString");
	if (Proc != NULL) {
		Proc(str, "New text");
	}

	FreeLibrary(hLib);

	printf(str);

	scanf_s(str);

	return 0;
}