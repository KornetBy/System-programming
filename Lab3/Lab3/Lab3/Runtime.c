#include <stdio.h>
#include<Windows.h>

#define a 10
#define b 3


typedef void(__cdecl* PROC_MATH)(int , int);
typedef void(__cdecl* PROC_RES)();

int main() {
    HINSTANCE hLib = LoadLibrary(TEXT("MathDll.dll"));

    PROC_MATH ProcMath1 = (PROC_MATH)GetProcAddress(hLib, "Addition");
    if (ProcMath1 != NULL) {
        ProcMath1(a, b);
    }

    PROC_MATH ProcMath2 = (PROC_MATH)GetProcAddress(hLib, "Subtraction");
    if (ProcMath2 != NULL) {
        ProcMath2(a, b);
    }
    
    PROC_RES ProcRes = (PROC_RES)GetProcAddress(hLib, "PrintResult");
    if (ProcRes != NULL) {
        ProcRes();
    }


    FreeLibrary(hLib);

    int c = 0;
    scanf_s("%d", c);
    return 0;
}
