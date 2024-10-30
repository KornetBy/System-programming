#include <windows.h>
#include <time.h>
#include <string.h> 
#include <windowsx.h>
#include <tchar.h>
#include <wchar.h>
#include <tlhelp32.h>
#include <stdlib.h>
#include <shellapi.h>
#include <process.h> 
#include "v5.h"
#define WINVER 0x0501 
#include <wtsapi32.h>
#include <lmcons.h>
#include <d3d9.h> 
#include <Psapi.h>
#include <strsafe.h>
#pragma comment(lib, "d3d9.lib")
static HWND hCombo, hwndDlg;

static HWND hPr;
static HWND hEdit;

int num = 0;
int kursor = 0;
int x11 = 950;
int y11 = 600;
bool par = false;


MEMORYSTATUS ms;
int Count;
int i;
HICON hIcon;
STARTUPINFO si = { sizeof(si) };
PROCESS_INFORMATION pi;
static DWORD ID;
static DWORD Prior;

HINSTANCE hInstance;
int g_ScrollPos = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wndclass = NewWindowClass(CS_HREDRAW | CS_VREDRAW, WndProc, hInstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), L"IDR_MENU1", L"T_M");

    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(L"T_M", L"Task Manager", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 1600, 700, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    SetTimer(hwnd, TIMER_SEC, 10000, NULL);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)

{

    static HWND hButton_V_P;
    static HWND hButton_Z_P;
    static HWND hButton_O_S;
    static HWND hButton_KILL;
    static HWND hButton_TURN_OFF;
    static HWND hButton_LOG_OFF;
    static HWND hButton_REBOOT;



    HBRUSH hBrush;
    char chBuff[3];
    int list_num;
    char buf1[3], buf2[3], buf3[3];
    int kX, kY;
    static int cxClient, cyClient;
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    switch (iMsg) {

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        return 0;

    case WM_CREATE:

        //создание кнопки
        hButton_V_P = CreateWindow(L"button", L"End process", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, 470, 250, 40, hwnd, (HMENU)ID_BUTTON_V_P, hInstance, NULL);
        hButton_Z_P = CreateWindow(L"button", L"Set the priority", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, 420, 250, 40, hwnd, (HMENU)ID_BUTTON_Z_P, hInstance, NULL);
        hButton_O_S = CreateWindow(L"button", L"Update the list", WS_CHILD | BS_DEFPUSHBUTTON, 300, 420, 250, 40, hwnd, (HMENU)ID_BUTTON_O_S, hInstance, NULL);
        hButton_KILL = CreateWindow(L"button", L"Unload the process", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, 520, 250, 40, hwnd, (HMENU)ID_BUTTON_KILL, hInstance, NULL);
        hButton_TURN_OFF = CreateWindow(L"button", L"Turning off the computer", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 650, 420, 250, 40, hwnd, (HMENU)ID_BUTTON_TURN_OFF, hInstance, NULL);
        hButton_LOG_OFF = CreateWindow(L"button", L"Changing the profile", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 650, 470, 250, 40, hwnd, (HMENU)ID_BUTTON_LOG_OFF, hInstance, NULL);
        hButton_REBOOT = CreateWindow(L"button", L"Restarting the computer", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 650, 520, 250, 40, hwnd, (HMENU)ID_BUTTON_REBOOT, hInstance, NULL);

        //создание текстового поля
        hEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 200, 470, 30, 21, hwnd, (HMENU)ID_EDIT, hInstance, NULL);
        //создание listbox
        hList = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 500, 40, 400, 100, hwnd, (HMENU)ID_LIST, hInstance, NULL);
        hList1 = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 500, 200, 400, 180, hwnd, (HMENU)ID_LIST, hInstance, NULL);
        hPr = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 10, 430, 100, 20, hwnd, (HMENU)ID_LIST, hInstance, NULL);
        huserlist = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 925, 40, 675, 70, hwnd, (HMENU)ID_LIST, hInstance, NULL);

        hdriverlist = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 925, 200, 675, 180, hwnd, (HMENU)ID_LIST, hInstance, NULL);
        hgplist = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT | LBS_USETABSTOPS | LBS_HASSTRINGS,
            925, 420, 675, 70, hwnd, (HMENU)ID_LIST, hInstance, NULL);

        hprocessorlist = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT | LBS_USETABSTOPS, 925, 530, 675, 170, hwnd, (HMENU)ID_LIST, hInstance, NULL);
        //создание комбо бокса
        hCombo = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 10, 40, 450, 350, hwnd, (HMENU)ID_COMBO, hInstance, NULL);
        GetProcessorInfo(hprocessorlist);
        SetListPrioryties(hPr);
        num = 0;
        ListUsers(huserlist);
        ListProcesses(hCombo);
        EnumerateDeviceDrivers(hdriverlist);
        GetVideoCardInfo(hgplist);
        SendMessage(hEdit, EM_SETSEL, 0, 6);

        char buf[20]; // Выделение памяти для строки из 20 символов

        _itoa_s(num, buf, sizeof(buf), 10); // Убедитесь, что буфер завершается нулевым символом



        SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)buf);

        par = true;

        return 0;


        //изображение меню

    case WM_PAINT:

        hdc = BeginPaint(hwnd, &ps);

        //DeleteObject(hBrush); 
        TextOut(hdc, 10, 20, L"List of processes", 16);
        TextOut(hdc, 10, 470, L"Number of processes", 20);
        TextOut(hdc, 500, 20, L"Process Parameters", 18);
        TextOut(hdc, 500, 170, L"Process Modules", 15);
        TextOut(hdc, 10, 410, L"Setting the priority", 18);
        TextOut(hdc, 925, 20, L"List of users", 18);
        TextOut(hdc, 925, 170, L"List of drivers", 18);
        TextOut(hdc, 925, 400, L"GP", 16);
        TextOut(hdc, 925, 510, L"Processor", 16);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_TIMER:

        switch (wParam) {
        case TIMER_SEC:
            num = 0;
            kursor = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
            ListProcesses(hCombo);
            SendMessage(hCombo, LB_SETCURSEL, kursor, 0);
            SendMessage(hEdit, EM_SETSEL, 0, 6);
            char* buf;
            buf = new char;
            _itoa_s(num, buf, sizeof(buf) / sizeof(buf[0]), 10);
            SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)buf);
            break;
        }
        return 0;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {

        case ID_COMBO://Действия при выборе процесса в списке
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                //////////////////////////информация о процессе/////////////////////
                i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
                ID = (DWORD)SendMessage(hCombo, LB_GETITEMDATA, (WPARAM)i, 0);

                ProcessesInfo(hList, ID);//Получить информацию
                //о выбранном  процессе
            }
            break;

        case ID_BUTTON_V_P:
            i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
            ID = (DWORD)SendMessage(hCombo, LB_GETITEMDATA, (WPARAM)i, 0);
            if (MessageBox(hwnd, TEXT("Are you sure"),
                TEXT("Attention"), MB_YESNO | MB_ICONWARNING) == IDYES)
                Terminate(ID);//Завершение процесса 

            /////////////////////обновление  информации в списке/////////////////////////
            num = 0;
            kursor = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
            ListProcesses(hCombo);
            SendMessage(hCombo, LB_SETCURSEL, kursor, 0);
            SendMessage(hEdit, EM_SETSEL, 0, 6);
            // char* buf;
           //buf = new char;
            _itoa_s(num, buf, sizeof(buf) / sizeof(buf[0]), 10);
            SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)buf);
            ////////////////////////////////////////////////////
            break;
            //MessageBox(hwnd,"Button pressed","Message from button",MB_OK);
            return 0;

        case ID_BUTTON_Z_P://задание приоритета процесса 

            i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
            ID = (DWORD)SendMessage(hCombo, LB_GETITEMDATA, (WPARAM)i, 0);

            i = SendMessage(hPr, LB_GETCURSEL, 0, 0);
            Prior = (DWORD)SendMessage(hPr, LB_GETITEMDATA, (WPARAM)i, 0);

            //Получение  прав на смену приоритета

            h = OpenProcess(PROCESS_SET_INFORMATION, 0, ID);
            if (h == INVALID_HANDLE_VALUE) {

                MessageBox(hwnd, TEXT("Ошибка:INVALID_HANDLE_VALUE!"),

                    TEXT("Error"), MB_OK | MB_ICONERROR);

                return FALSE;

            }

            if (!SetPriorityClass(h, Prior))
                MessageBox(hwnd, TEXT("Priority cannot be set!"), TEXT("Error"), MB_OK | MB_ICONERROR);
            else
                MessageBox(hwnd, TEXT("A new basic priority has been set!"), TEXT("Внимание"), MB_OK | MB_ICONWARNING);
            CloseHandle(h);

            /////////////////////обновление  информации в списке/////////////////////////

            num = 0;
            kursor = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
            ListProcesses(hCombo);
            SendMessage(hCombo, LB_SETCURSEL, kursor, 0);
            SendMessage(hEdit, EM_SETSEL, 0, 6);

            // char* buf;

            //buf = new char;

            _itoa_s(num, buf, sizeof(buf) / sizeof(buf[0]), 10);

            SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)buf);

            //////////////////////////////////////////////////// 
              //////////////////////////информация о процессе/////////////////////

            i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);

            ID = (DWORD)SendMessage(hCombo, LB_GETITEMDATA, (WPARAM)i, 0);

            //MessageBox(hwnd," ","Error",MB_OK);

            ProcessesInfo(hList, ID);//Получить информацию

            //о выбранном  процессе 

            return 0;
        case ID_BUTTON_O_S:

            /////////////////////обновление  информации в списке/////////////////////////

            num = 0;

            kursor = SendMessage(hCombo, LB_GETCURSEL, 0, 0);

            ListProcesses(hCombo);

            SendMessage(hCombo, LB_SETCURSEL, kursor, 0);

            SendMessage(hEdit, EM_SETSEL, 0, 6);

            char* buf;

            buf = new char;

            _itoa_s(num, buf, sizeof(buf) / sizeof(buf[0]), 10);

            SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)buf);

            //////////////////////////////////////////////////// 


            return 0;

        case ID_BUTTON_KILL:

            i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);

            ID = (DWORD)SendMessage(hCombo, LB_GETITEMDATA,

                (WPARAM)i, 0);

            if (MessageBox(hwnd, TEXT("Are you sure"), TEXT("Attention"), MB_YESNO | MB_ICONWARNING) == IDYES)

                KillProcess(ID);//завершение процесса!!!! 

            /////////////////////обновление  информации в списке/////////////////////////

            num = 0;

            kursor = SendMessage(hCombo, LB_GETCURSEL, 0, 0);

            ListProcesses(hCombo);

            SendMessage(hCombo, LB_SETCURSEL, kursor, 0);

            SendMessage(hEdit, EM_SETSEL, 0, 6);

            // char* buf;

            buf = new char;

            _itoa_s(num, buf, sizeof(buf) / sizeof(buf[0]), 10);

            SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)buf);

            //////////////////////////////////////////////////// 
            return 0;

        case ID_BUTTON_TURN_OFF:

            if (MessageBox(hwnd, TEXT("Are you sure you want to turn off your computer?"),

                TEXT("Attention"), MB_YESNO | MB_ICONWARNING) == IDYES)
            {
                if (!SetCurrentPrivilege(SE_SHUTDOWN_NAME, TRUE))
                {
                    ::MessageBox(0, L"Not enough privileges!", L"", MB_OK);
                }
                ExitWindowsEx(EWX_POWEROFF, 0);
            }

            return 0;

        case ID_BUTTON_LOG_OFF:
            if (MessageBox(hwnd, TEXT("Are you sure you want to log out?"),
                TEXT("Attention"), MB_YESNO | MB_ICONWARNING) == IDYES)
            {
                if (!SetCurrentPrivilege(SE_SHUTDOWN_NAME, TRUE))

                {
                    ::MessageBox(0, L"Not enough privileges!", L"", MB_OK);
                }
                ExitWindowsEx(EWX_LOGOFF, 0);
            }
            return 0;

        case ID_BUTTON_REBOOT:
            if (MessageBox(hwnd, TEXT("Are you sure you want to restart your computer?"), TEXT("Внимание"), MB_YESNO | MB_ICONWARNING) == IDYES)
            {
                if (!SetCurrentPrivilege(SE_SHUTDOWN_NAME, TRUE))
                {
                    ::MessageBox(0, L"Not enough privileges!", L"", MB_OK);
                }
                ExitWindowsEx(EWX_REBOOT, 0);
            }
            return 0;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

//Отображение  раскрыв. списка процессов
int ListProcesses(HWND hwnd)
{
    PROCESSENTRY32 pe32 = { 0 };
    HANDLE myhandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    int i = 0;

    SendMessage(hwnd, LB_RESETCONTENT, 0, 0);

    if (myhandle == INVALID_HANDLE_VALUE)
        return false;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Поиск процессов в системе
    if (Process32First(myhandle, &pe32))
    {
        do {
            num++;
            i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)((LPSTR)pe32.szExeFile));
            SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)pe32.th32ProcessID);
        } while (Process32Next(myhandle, &pe32));
    }

    // Устанавливаем положение вертикальной полосы прокрутки


    SendMessage(hwnd, LB_SETCURSEL, 0, 0);
    FORWARD_WM_COMMAND(hwndDlg, ID_COMBO, hCombo, CBN_SELCHANGE, SendMessage);

    i = SendMessage(hwnd, LB_GETCOUNT, 0, 0);

    TCHAR str[100];

    // Отображение найденных процессов в списке
    SetDlgItemText(hwndDlg, IDC_COUNT, (LPCTSTR)_ltow_s(i, str, 10));

    CloseHandle(myhandle);
    return true;
}


//Получение  информации о процессе.



//Отображение  элементов списка приоритетов

void SetListPrioryties(HWND hwnd)
{
    int i;
    i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)((LPSTR)L"Real time"));
    SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)REALTIME_PRIORITY_CLASS);

    i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)((LPSTR)L"High"));
    SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)HIGH_PRIORITY_CLASS);

    i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)((LPSTR)L"Normal"));
    SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)NORMAL_PRIORITY_CLASS);

    i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)((LPSTR)L"Low"));
    SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)IDLE_PRIORITY_CLASS);
}
//Завершение  процесса
void Terminate(DWORD ID)
{
    //Получение  прав на завершение
    h = OpenProcess(PROCESS_TERMINATE, 0, ID);
    if (h == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, TEXT("Error:INVALID_HANDLE_VALUE!"),
            TEXT("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    int i = 0;
    //Завершение
    TerminateProcess(h, i);

    DWORD dw = WaitForSingleObject(h, 5000);

    switch (dw) {

    case WAIT_OBJECT_0:

        MessageBox(hwndDlg, TEXT("The process is completed!"),

            TEXT("ok"), MB_OK | MB_ICONINFORMATION);

        break;

    case WAIT_TIMEOUT:

        MessageBox(hwndDlg, TEXT("Process completion error!"),

            TEXT("Error"), MB_OK | MB_ICONERROR);

        break;

    case WAIT_FAILED:

        MessageBox(hwndDlg, TEXT("The process is completed"),

            TEXT("Everything is OK"), MB_OK | MB_ICONINFORMATION);

        break;

    }

    CloseHandle(h);
}

//Уничтожение процесса

void KillProcess(DWORD ID) {

    DWORD dwThId;

    HANDLE hProc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |

        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, ID);

    HANDLE hTh = CreateRemoteThread(hProc, NULL, 0, NULL, NULL, CREATE_SUSPENDED, &dwThId);
    if (!hProc || !hTh) {

        MessageBox(NULL, TEXT("Some error:may be access denide?"), TEXT("!"), 0);

        return;

    }

    CONTEXT thcon;

    thcon.ContextFlags = CONTEXT_CONTROL;

    GetThreadContext(hTh, &thcon);

    thcon.Eip = 0xffffffff;

    SetThreadContext(hTh, &thcon);

    ResumeThread(hTh);
    CloseHandle(hTh);
    CloseHandle(hProc);

}

BOOL SetCurrentPrivilege(LPCTSTR Privilege, BOOL bEnablePrivilege)
{
    HANDLE hToken;

    LUID luid;

    TOKEN_PRIVILEGES tp, tpPrevious;

    DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);

    BOOL bSuccess = FALSE;
    if (!LookupPrivilegeValue(NULL, Privilege, &luid))

        return FALSE;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
        return FALSE;

    tp.PrivilegeCount = 1;

    tp.Privileges[0].Luid = luid;

    tp.Privileges[0].Attributes = 0;

    AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious);

    if (GetLastError() == ERROR_SUCCESS)
    {
        tpPrevious.PrivilegeCount = 1;

        tpPrevious.Privileges[0].Luid = luid;

        if (bEnablePrivilege)

            tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
        else
            tpPrevious.Privileges[0].Attributes &= ~(SE_PRIVILEGE_ENABLED);
        AdjustTokenPrivileges(hToken, FALSE, &tpPrevious, cbPrevious, NULL, NULL);
        if (GetLastError() == ERROR_SUCCESS)
            bSuccess = TRUE;
    }
    CloseHandle(hToken);
    return bSuccess;
}

void ListUsers(HWND huserlist) {
    WTS_SESSION_INFO* pSessions = NULL;
    DWORD dwCount = 0;

    if (WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessions, &dwCount)) {
        for (DWORD i = 0; i < dwCount; ++i) {
            WTS_SESSION_INFO sessionInfo = pSessions[i];

            TCHAR szUserName[UNLEN + 1];
            TCHAR szDomainName[DNLEN + 1];
            TCHAR szState[256];

            DWORD dwUserNameSize = UNLEN + 1;
            DWORD dwDomainNameSize = DNLEN + 1;
            DWORD dwStateSize = sizeof(szState) / sizeof(szState[0]);

            LPWSTR lpUserName = szUserName;
            LPWSTR lpDomainName = szDomainName;
            LPWSTR lpState = szState;

            if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, sessionInfo.SessionId, WTSUserName, &lpUserName, &dwUserNameSize) &&
                WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, sessionInfo.SessionId, WTSDomainName, &lpDomainName, &dwDomainNameSize) &&
                WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, sessionInfo.SessionId, WTSConnectState, &lpState, &dwStateSize)) {

                // Получение состояния сеанса
                int sessionState = *(int*)lpState;

                // Преобразование числового состояния в строку
                const TCHAR* stateString = nullptr;
                switch (sessionState) {
                case WTSActive:
                    stateString = _T("Active");
                    break;
                case WTSConnected:
                    stateString = _T("Connected");
                    break;
                case WTSConnectQuery:
                    stateString = _T("Connect Query");
                    break;
                    // Другие возможные состояния...
                default:
                    stateString = _T("Unknown");
                    break;
                }
                TCHAR szComputerName[MAX_COMPUTERNAME_LENGTH + 1];
                DWORD dwComputerNameSize = MAX_COMPUTERNAME_LENGTH + 1;
                TCHAR szInfo[256];
                if (GetComputerName(szComputerName, &dwComputerNameSize))
                    _stprintf_s(szInfo, _T("User: %s\\%s, Session ID: %d, State: %s, Computer: % s"), lpUserName, lpDomainName, sessionInfo.SessionId, stateString, szComputerName);
                SendMessage(huserlist, LB_ADDSTRING, 0, (LPARAM)szInfo);
            }
        }

        WTSFreeMemory(pSessions);
    }
}

void EnumerateDeviceDrivers(HWND hdriverlist) {
    DWORD cbNeeded, cDrivers;
    LPVOID drivers[1024];

    if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers)) {
        cDrivers = cbNeeded / sizeof(drivers[0]);
        for (DWORD i = 0; i < cDrivers; ++i) {
            TCHAR driverName[MAX_PATH];
            TCHAR driverPath[MAX_PATH];

            if (GetDeviceDriverBaseName(drivers[i], driverName, sizeof(driverName) / sizeof(driverName[0])) &&
                GetDeviceDriverFileName(drivers[i], driverPath, sizeof(driverPath) / sizeof(driverPath[0]))) {

                // Теперь у вас есть имя и путь к файлу для каждого драйвера
                // Вместо вывода в консоль, добавьте информацию в ваш список (hdriverlist)
                // Например:
                TCHAR szInfo[MAX_PATH * 2];
                _stprintf_s(szInfo, _T("Driver Name: %s, Driver Path: %s"), driverName, driverPath);
                SendMessage(hdriverlist, LB_ADDSTRING, 0, (LPARAM)szInfo);
            }
        }
    }
}

void GetVideoCardInfo(HWND hwnd) {
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (pD3D == nullptr) {
        MessageBox(hwnd, _T("Failed to create Direct3D object"), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    D3DADAPTER_IDENTIFIER9 adapterInfo;
    if (SUCCEEDED(pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapterInfo))) {
        TCHAR szInfo[512];
        _stprintf_s(szInfo, _T("Video Card: %s"),
            adapterInfo.Description);
        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szInfo);
        _stprintf_s(szInfo, _T("Device Name: %s"), adapterInfo.DeviceName);


        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szInfo);
        _stprintf_s(szInfo, _T("Driver Version: %d.%d.%d.%d"),
            HIWORD(adapterInfo.DriverVersion.HighPart),
            LOWORD(adapterInfo.DriverVersion.HighPart),
            HIWORD(adapterInfo.DriverVersion.LowPart),
            LOWORD(adapterInfo.DriverVersion.LowPart));
        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szInfo);
    }

    pD3D->Release();
}
void GetProcessorInfo(HWND hwnd) {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    TCHAR szInfo[256];
    StringCbPrintf(szInfo, sizeof(szInfo), _T("Processor Architecture: %d\n"), sysInfo.wProcessorArchitecture);
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szInfo);

    StringCbPrintf(szInfo, sizeof(szInfo), _T("Number of Processors: %d\n"), sysInfo.dwNumberOfProcessors);
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szInfo);

    StringCbPrintf(szInfo, sizeof(szInfo), _T("Page Size: %d\n"), sysInfo.dwPageSize);
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szInfo);

    StringCbPrintf(szInfo, sizeof(szInfo), _T("Processor Type: %d\n"), sysInfo.dwProcessorType);
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szInfo);

    StringCbPrintf(szInfo, sizeof(szInfo), _T("Minimum Application Address: 0x%p\n"), sysInfo.lpMinimumApplicationAddress);
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szInfo);

    StringCbPrintf(szInfo, sizeof(szInfo), _T("Maximum Application Address: 0x%p\n"), sysInfo.lpMaximumApplicationAddress);
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szInfo);

    // Информация о ядрах процессора
    TCHAR szCoresInfo[256];
    StringCbPrintf(szCoresInfo, sizeof(szCoresInfo), _T("Number of Processor Cores: %d"), sysInfo.dwNumberOfProcessors);
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szCoresInfo);

    // Информация о сокетах
    TCHAR szSocketsInfo[256];
    StringCbPrintf(szSocketsInfo, sizeof(szSocketsInfo), _T("Number of Processor Sockets: %d"), 1);  // В Windows API нет прямой поддержки сокетов, но обычно это 1
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szSocketsInfo);

    // Базовая скорость процессора
    TCHAR szBaseSpeedInfo[256];
    StringCbPrintf(szBaseSpeedInfo, sizeof(szBaseSpeedInfo), _T("Processor Base Speed: %d MHz"), sysInfo.dwProcessorType);
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szBaseSpeedInfo);

}