#include <Windows.h>
#include <stdio.h>


int main() {
    const char* dllPath = "D:\\InjectDll.dll";

    DWORD pid = 4932;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (hProcess)
    {
        LPVOID remoteString = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);

        if (remoteString)
        {
            WriteProcessMemory(hProcess, remoteString, dllPath, strlen(dllPath) + 1, 0);

            HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, remoteString, 0, 0);

            if (hThread)
            {
                WaitForSingleObject(hThread, INFINITE);
                CloseHandle(hThread);
            }

            VirtualFreeEx(hProcess, remoteString, 0, MEM_RELEASE);
        }

        CloseHandle(hProcess);
    }

	return 0;
}