// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <stdio.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

int addition = 0;

__declspec(dllexport) void Addition(int a, int b) {
    addition = a + b;
}

int subtraction = 0;

__declspec(dllexport) void Subtraction(int a, int b) {
    subtraction = a - b;
}

__declspec(dllexport) void PrintResult() {
    if (0 != addition) {
        printf("addition = %d \n", addition);
    }
    if (0 != subtraction) {
        printf("subtraction = %d \n", subtraction);
    }
}


