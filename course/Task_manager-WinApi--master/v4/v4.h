#pragma once
#pragma comment(lib, "wtsapi32.lib")
#include <windows.h>
#include <tlhelp32.h>
static HWND hList;
static HWND hList1;
static HWND hprocessorlist;
static HWND hgplist;
static HWND hdriverlist;
static HWND huserlist;
static HANDLE h; 

#define OnMenuCllicked1 5001
#define OnMenuCllicked2 5002
#define OnMenuCllicked3 5003
#define OnMenuCllicked4 5004
#define OnMenuCllicked5 5005
#define OnMenuCllicked6 5006


# define ID_BUTTON_V_P 3000
# define ID_BUTTON_Z_P 3001
# define ID_BUTTON_O_S 3002
# define ID_BUTTON_KILL 3003
# define ID_BUTTON_TURN_OFF 3004
# define ID_BUTTON_LOG_OFF 3005
# define ID_BUTTON_REBOOT 3006
# define ID_EDIT 2000
# define ID_LIST 1000
# define ID_COMBO 1500
#define TIMER_SEC 1
#define IDC_COUNT 1010 

void ListUsers(HWND hwnd);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void EnumerateDeviceDrivers(HWND hdriverlist);
void GetVideoCardInfo(HWND hdriverlist);
void GetProcessorInfo(HWND hwnd);


int ListProcesses(HWND hwnd);
int ProcessesInfo(HWND hwnd, DWORD dwProcessID);
void Terminate(DWORD ID);
void KillProcess(DWORD ID);
void SetListPrioryties(HWND hwnd);
inline BOOL GetDebugPriority(void);
BOOL SetCurrentPrivilege(LPCTSTR Privilege, BOOL bEnablePrivilege);
WNDCLASSEX NewWindowClass(UINT style,
    WNDPROC     lpfnWndProc,
    HINSTANCE   hInstance,
    HICON       hIcon,
    HCURSOR     hCursor,
    HBRUSH      hbrBackground,
    LPCWSTR     lpszMenuName,
    LPCWSTR     lpszClassName);