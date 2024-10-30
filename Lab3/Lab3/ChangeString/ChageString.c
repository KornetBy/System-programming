// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define SIZE_BUFF 1'000'000
char buff[SIZE_BUFF];

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

__declspec(dllexport) void ReplaceString(const char* oldString, const char* newString) {
    HANDLE hProc = GetCurrentProcess();

    int oldStringLen = strlen(oldString);

    SYSTEM_INFO siSysInfo;

    GetSystemInfo(&siSysInfo);

    PSIZE_T minAddr = oldString - SIZE_BUFF*1 - 90;
    PSIZE_T maxAddr = siSysInfo.lpMaximumApplicationAddress;

    char* p = minAddr;
    memset(buff, 0, sizeof(SIZE_BUFF));

    int j = 0;
    while(p< maxAddr) {
        ReadProcessMemory(hProc, p, &buff, SIZE_BUFF, NULL);

        for (int i = 0; i < SIZE_BUFF; i++) {
            if (buff[i] == oldString[j]) {
                j++;
                if (j == oldStringLen) {
                    WriteProcessMemory(hProc, p + i - j + 1, newString, oldStringLen, NULL);
                    CloseHandle(hProc);
                    return;
                }
            }
            else {
                j = 0;
            }
        }
        p += SIZE_BUFF;
    }

    CloseHandle(hProc);

}
