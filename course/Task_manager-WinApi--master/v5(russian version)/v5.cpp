#include "v5.h"




WNDCLASSEX NewWindowClass(UINT style,
    WNDPROC     lpfnWndProc,
    HINSTANCE   hInstance,
    HICON       hIcon,
    HCURSOR     hCursor,
    HBRUSH      hbrBackground,
    LPCWSTR     lpszMenuName,
    LPCWSTR     lpszClassName) {
    WNDCLASSEX WND = { 0 };
    WND.cbSize = sizeof(WNDCLASSEX);
    WND.style = style;
    WND.lpfnWndProc = lpfnWndProc;
    WND.hInstance = hInstance;
    WND.hIcon = hIcon;
    WND.hCursor = hCursor;
    WND.hbrBackground = hbrBackground;
    WND.lpszMenuName = lpszMenuName;
    WND.lpszClassName = lpszClassName;
    WND.cbClsExtra = 0;
    WND.cbWndExtra = 0;

    return WND;
}

void MainWndAddMenus(HWND hWnd) {
    HMENU RootMunu = CreateMenu();
    AppendMenu(RootMunu, MF_STRING, OnMenuCllicked1, L"Processes");
    AppendMenu(RootMunu, MF_STRING, OnMenuCllicked2, L"Performance");
    AppendMenu(RootMunu, MF_STRING, OnMenuCllicked3, L"Startup apps");
    AppendMenu(RootMunu, MF_STRING, OnMenuCllicked4, L"Users");
    AppendMenu(RootMunu, MF_STRING, OnMenuCllicked5, L"Details");
    AppendMenu(RootMunu, MF_STRING, OnMenuCllicked6, L"Services");
    SetMenu(hWnd, RootMunu);
}

int ProcessesInfo(HWND hwnd, DWORD dwProcessID)

{

    PROCESSENTRY32 pe32 = { 0 };

    HANDLE myhandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    int i = 0;

    h = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwProcessID);

    DWORD minphmem, maxphmem;

    DWORD dw = GetPriorityClass(h);

    TCHAR s[20];

    //Отображание  списка приоритетов  

    i = SendMessage(hList, LB_FINDSTRING, 0, (LPARAM)((LPCTSTR)s));

    SendMessage(hList, LB_SETCURSEL, i, 0);

    SendMessage(hwnd, LB_RESETCONTENT, 0, 0);

    if (myhandle == INVALID_HANDLE_VALUE)

        return false;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    //Поиск  выбранного процесса и получение  информации о нем

    if (Process32First(myhandle, &pe32))

        do {

            if (pe32.th32ProcessID == dwProcessID) break;

        }

    while (i++, Process32Next(myhandle, &pe32));

        TCHAR sz[100];
        //Отображение  информации о процессе в списке
        wsprintf(sz, TEXT("Name: %s"), pe32.szExeFile);

        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);

        wsprintf(sz, TEXT("Process ID: %lu"), pe32.th32ProcessID);

        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);

        wsprintf(sz, TEXT("Number of threads: %d"), pe32.cntThreads);

        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);

        wsprintf(sz, TEXT("Basic priority: %d"), pe32.pcPriClassBase);

        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);

        GetProcessWorkingSetSize(h, &minphmem, &maxphmem);
        wsprintf(sz, TEXT("Min/max physical memory size:%lu MB/ %lu MB")

            , minphmem / 1024, maxphmem / 1024);
        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);

        CloseHandle(h);
        myhandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);

        if (myhandle == INVALID_HANDLE_VALUE)
        {
            DWORD a = GetLastError();
            return false;
        }
        MODULEENTRY32 me32 = { 0 };

        me32.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(myhandle, &me32))

            do {

                SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)me32.szExePath);
            } while (Module32Next(myhandle, &me32));

            CloseHandle(myhandle);

            return true;

}

inline BOOL GetDebugPriority(void)

{

    BOOL fOk = FALSE;

    HANDLE hToken;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {

        TOKEN_PRIVILEGES tp;

        tp.PrivilegeCount = 1;

        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);

        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

        fOk = (GetLastError() == ERROR_SUCCESS);

        CloseHandle(hToken);
    }
    return fOk;
}