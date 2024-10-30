// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "windows.h"
#define EXPORT __declspec(dllexport)

int EXPORT Sum(int i, int j);
int EXPORT Sub(int i, int j);
int EXPORT Mult(int i, int j);
int EXPORT Div(int i, int j);
int EXPORT Part(int i, int j);


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

EXPORT int Sum(int i, int j)
{
    return i + j;
}

EXPORT int Sub(int i, int j)
{
    return i - j;
}

EXPORT int Mult(int i, int j)
{
    return i * j;
}

EXPORT int Div(int i, int j)
{
    if (!j)
        return j;
    return i / j;
}

EXPORT int Part(int i, int j)
{
    if (!j)
        return j;
    return i % j;
}