// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <stdio.h>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    FILE* file;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        fopen_s(&file, "E:\\example.txt", "w");
        DWORD pid = GetCurrentProcessId();
        HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
        fprintf(file, "Process id = %d, Process Handle = %d", pid, h);
        fclose(file);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

