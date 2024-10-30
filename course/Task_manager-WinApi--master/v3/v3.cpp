#include <windows.h>
#include <time.h>
#include <string.h>


#include <windowsx.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <stdlib.h>
#include <shellapi.h>
#include <process.h>

#define _CRT_SECURE_NO_WARNINGS

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
static HWND hPr;
static HWND hCombo, hwndDlg;
static HWND hList;
static HWND hList1;
static HWND hEdit;
int num = 0;
int kursor = 0;
int x11 = 950;
int y11 = 600;
bool par = false;

int ListProcesses(HWND hwnd);
int ProcessesInfo(HWND hwnd, DWORD dwProcessID);
void Terminate(DWORD ID);
void KillProcess(DWORD ID);
void SetListPrioryties(HWND hwnd);
inline BOOL GetDebugPriority(void);
BOOL SetCurrentPrivilege(LPCTSTR Privilege, BOOL bEnablePrivilege);
////////////////////////////////////////////////////////////////////
MEMORYSTATUS ms;
int Count;
int i;
HICON hIcon;
HANDLE h;
STARTUPINFO si = { sizeof(si) };
PROCESS_INFORMATION pi;

static DWORD ID;

static DWORD Prior;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInstance;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	LPCWSTR LszAppName = L"MyAppName";
	static char szAppName[] = "T_M";
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//wndclass.lpszMenuName = "IDR_MENU1";
	wndclass.lpszClassName = LszAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wndclass);
	hwnd = CreateWindow(LszAppName, L"Task Manager", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);


	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	//создание таймера
	SetTimer(hwnd, TIMER_SEC, 10000, NULL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	//определение идентификаторов button и edit
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
	//LPARAM text1[4]="red";
	int kX, kY;

	static int cxClient, cyClient;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	switch (iMsg)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		//MoveWindow(hCombo,10,40,450-x11+cxClient,100-y11+cyClient,par);


		return 0;

	case WM_CREATE:

		MoveWindow(hwnd, 10, 10, 950, 600, true);

		//создание кнопки
		hButton_V_P = CreateWindow(L"button", L"Завершить процесс", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, 470, 250, 40, hwnd, (HMENU)ID_BUTTON_V_P, hInstance, NULL);
		hButton_Z_P = CreateWindow(L"button", L"Задать приоритет", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, 420, 250, 40, hwnd, (HMENU)ID_BUTTON_Z_P, hInstance, NULL);
		hButton_O_S = CreateWindow(L"button", L"Обновить список", WS_CHILD | BS_DEFPUSHBUTTON, 300, 420, 250, 40, hwnd, (HMENU)ID_BUTTON_O_S, hInstance, NULL);
		hButton_KILL = CreateWindow(L"button", L"Выгрузить процесс", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, 520, 250, 40, hwnd, (HMENU)ID_BUTTON_KILL, hInstance, NULL);
		hButton_TURN_OFF = CreateWindow(L"button", L"Выключение компьютера", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 650, 420, 250, 40, hwnd, (HMENU)ID_BUTTON_TURN_OFF, hInstance, NULL);
		hButton_LOG_OFF = CreateWindow(L"button", L"Смена профиля", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 650, 470, 250, 40, hwnd, (HMENU)ID_BUTTON_LOG_OFF, hInstance, NULL);
		hButton_REBOOT = CreateWindow(L"button", L"Перезагрузка компьютера", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 650, 520, 250, 40, hwnd, (HMENU)ID_BUTTON_REBOOT, hInstance, NULL);

		//создание текстового поля
		hEdit = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 200, 470, 30, 21, hwnd, (HMENU)ID_EDIT, hInstance, NULL);

		//создание listbox
		hList = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 500, 40, 400, 100, hwnd, (HMENU)ID_LIST, hInstance, NULL);
		hList1 = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 500, 200, 400, 180, hwnd, (HMENU)ID_LIST, hInstance, NULL);
		hPr = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 10, 430, 100, 20, hwnd, (HMENU)ID_LIST, hInstance, NULL);

		//создание комбо бокса
		hCombo = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 10, 40, 450, 350, hwnd, (HMENU)ID_COMBO, hInstance, NULL);

		//SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM) "Вертикальная");
		SetListPrioryties(hPr);

		//MoveWindow (hwnd,x,y,x1,y1, true);

		num = 0;
		ListProcesses(hCombo);
		SendMessage(hEdit, EM_SETSEL, 0, 6);
		char* buf;
		buf = new char;
		_itoa_s(num, buf, sizeof(buf), 10);
		SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)buf);

		par = true;
		return 0;


		//изображение меню
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//DeleteObject(hBrush);

		TextOut(hdc, 10, 20, L"Список процессов", 16);
		TextOut(hdc, 10, 470, L"Количество процессов", 20);
		TextOut(hdc, 500, 20, L"Параметры процесса", 18);
		TextOut(hdc, 500, 170, L"Модули процесса", 15);
		TextOut(hdc, 10, 410, L"Задание приоритета", 18);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_TIMER:
		switch (wParam)
		{
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
			// нажатие кнопки

		{
		case ID_COMBO://Действия при выборе процесса в списке
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{

				//////////////////////////информация о процессе/////////////////////
				i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
				ID = (DWORD)SendMessage(hCombo, LB_GETITEMDATA, (WPARAM)i, 0);
				//MessageBox(hwnd," ","Error",MB_OK);
				ProcessesInfo(hList, ID);//Получить информацию
				//о выбранном процессе
			}
			break;



		case ID_BUTTON_V_P:


			i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
			ID = (DWORD)SendMessage(hCombo, LB_GETITEMDATA, (WPARAM)i, 0);
			if (MessageBox(hwnd, TEXT("Вы уверены"),
				TEXT("Внимание"), MB_YESNO | MB_ICONWARNING) == IDYES)
				Terminate(ID);//Завершение процесса


			/////////////////////обновление информации в списке/////////////////////////
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
			break;

			//MessageBox(hwnd,"Button pressed","Message from button",MB_OK);
			return 0;

		case ID_BUTTON_Z_P://задание приоритета процесса

			i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
			ID = (DWORD)SendMessage(hCombo, LB_GETITEMDATA,
				(WPARAM)i, 0);
			i = SendMessage(hPr, LB_GETCURSEL, 0, 0);
			Prior = (DWORD)SendMessage(hPr, LB_GETITEMDATA,
				(WPARAM)i, 0);
			//Получение прав на смену приоритета
			h = OpenProcess(PROCESS_SET_INFORMATION, 0, ID);

			if (h == INVALID_HANDLE_VALUE) {
				MessageBox(hwnd, TEXT("Ошибка:INVALID_HANDLE_VALUE!"),
					TEXT("Error"), MB_OK | MB_ICONERROR);
				return FALSE;
			}

			if (!SetPriorityClass(h, Prior))
				MessageBox(hwnd, TEXT("Невозможно установить приоритет!"),
					TEXT("Error"), MB_OK | MB_ICONERROR);
			else
				MessageBox(hwnd, TEXT("Задан новый базовый приоритет!"),
					TEXT("Внимание"), MB_OK | MB_ICONWARNING);

			CloseHandle(h);


			/////////////////////обновление информации в списке/////////////////////////
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

			//////////////////////////информация о процессе/////////////////////
			i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
			ID = (DWORD)SendMessage(hCombo, LB_GETITEMDATA, (WPARAM)i, 0);
			//MessageBox(hwnd," ","Error",MB_OK);
			ProcessesInfo(hList, ID);//Получить информацию
			//о выбранном процессе

			return 0;
		case ID_BUTTON_O_S:


			/////////////////////обновление информации в списке/////////////////////////
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
			if (MessageBox(hwnd, TEXT("Вы уверены"),
				TEXT("Внимание"), MB_YESNO | MB_ICONWARNING) == IDYES)
				KillProcess(ID);//завершение процесса!!!!


			/////////////////////обновление информации в списке/////////////////////////
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
			if (MessageBox(hwnd, TEXT("Вы точно хотите выключить компьютер?"),
				TEXT("Внимание"), MB_YESNO | MB_ICONWARNING) == IDYES)
			{
				if (!SetCurrentPrivilege(SE_SHUTDOWN_NAME, TRUE))
				{
					::MessageBox(0, L"Недостаточно привилегий!", L"", MB_OK);

				}

				ExitWindowsEx(EWX_POWEROFF, 0);
			}
			return 0;

		case ID_BUTTON_LOG_OFF:
			if (MessageBox(hwnd, TEXT("Вы точно отлогиниться?"),
				TEXT("Внимание"), MB_YESNO | MB_ICONWARNING) == IDYES)
			{
				if (!SetCurrentPrivilege(SE_SHUTDOWN_NAME, TRUE))
				{
					::MessageBox(0, L"Недостаточно привилегий!", L"", MB_OK);

				}

				ExitWindowsEx(EWX_LOGOFF, 0);
			}
			return 0;

		case ID_BUTTON_REBOOT:
			if (MessageBox(hwnd, TEXT("Вы точно хотите перезагрузить компьютер?"),
				TEXT("Внимание"), MB_YESNO | MB_ICONWARNING) == IDYES)
			{
				if (!SetCurrentPrivilege(SE_SHUTDOWN_NAME, TRUE))
				{
					::MessageBox(0, L"Недостаточно привилегий!", L" ", MB_OK);

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

//Отображение раскрыв. списка процессов
int ListProcesses(HWND hwnd)
{
	PROCESSENTRY32 pe32 = { 0 };
	HANDLE myhandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	int i = 0;

	SendMessage(hwnd, LB_RESETCONTENT, 0, 0);
	if (myhandle == INVALID_HANDLE_VALUE)
		return false;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	//Поиск процессов в системе
	if (Process32First(myhandle, &pe32))
		do {
			num++;
			i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)((LPSTR)pe32.szExeFile));
			SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)pe32.th32ProcessID);
		} while (Process32Next(myhandle, &pe32));
		//
		SendMessage(hwnd, LB_SETCURSEL, 0, 0);

		FORWARD_WM_COMMAND(hwndDlg, ID_COMBO, hCombo, CBN_SELCHANGE, SendMessage);
		i = SendMessage(hwnd, LB_GETCOUNT, 0, 0);

		TCHAR str[100];
		//Отображение найденных процессов в списке
		SetDlgItemText(hwndDlg, IDC_COUNT, (LPCTSTR)_ltow_s(i, str, 10));
		CloseHandle(myhandle);

		return true;
}


//Получение информации о процессе.
int ProcessesInfo(HWND hwnd, DWORD dwProcessID)
{

	PROCESSENTRY32 pe32 = { 0 };
	HANDLE myhandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	int i = 0;
	h = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwProcessID);
	DWORD minphmem, maxphmem;
	DWORD dw = GetPriorityClass(h);
	TCHAR s[20];
	//Отображание списка приоритетов 

	i = SendMessage(hList, LB_FINDSTRING, 0, (LPARAM)((LPCTSTR)s));
	SendMessage(hList, LB_SETCURSEL, i, 0);
	SendMessage(hwnd, LB_RESETCONTENT, 0, 0);

	if (myhandle == INVALID_HANDLE_VALUE)
		return false;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	//Поиск выбранного процесса и получение информации о нем
	if (Process32First(myhandle, &pe32))
		do {
			if (pe32.th32ProcessID == dwProcessID) break;
		} while (i++, Process32Next(myhandle, &pe32));


		TCHAR sz[100];
		//Отображение информации о процессе в списке
		wsprintf(sz, TEXT("Имя: %s"), pe32.szExeFile);
		SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);

		wsprintf(sz, TEXT("ID процесса: %lu"), pe32.th32ProcessID);
		SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);

		wsprintf(sz, TEXT("Кол-во потоков: %d"), pe32.cntThreads);
		SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);

		wsprintf(sz, TEXT("Базовый приоритет: %d"), pe32.pcPriClassBase);
		SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);
		GetProcessWorkingSetSize(h, &minphmem, &maxphmem);

		wsprintf(sz, TEXT("Мин/макс размер физической памяти:%lu / %lu")
			, minphmem, maxphmem);

		SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)sz);

		CloseHandle(h);

		myhandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
		if (myhandle == INVALID_HANDLE_VALUE)
			return false;
		MODULEENTRY32 me32 = { 0 };
		me32.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(myhandle, &me32))
			do {
				SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)me32.szExePath);

			} while (Module32Next(myhandle, &me32));
			CloseHandle(myhandle);
			return true;

}

//Получение прав отладчика для отображения информации о серверных
//приложениях 
inline BOOL GetDebugPriority(void)
{
	BOOL fOk = FALSE;
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES,
		&hToken)) {
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

//Отображение элементов списка приоритетов
void SetListPrioryties(HWND hwnd)
{
	int i;
	i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)(L"Real time"));
	SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)REALTIME_PRIORITY_CLASS);
	i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)(L"High"));
	SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)HIGH_PRIORITY_CLASS);
	i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)(L"Normal"));
	SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)NORMAL_PRIORITY_CLASS);
	i = SendMessage(hwnd, LB_ADDSTRING, 0, (DWORD)(L"Low"));
	SendMessage(hwnd, LB_SETITEMDATA, i, (DWORD)IDLE_PRIORITY_CLASS);
}

//Завершение процесса
void Terminate(DWORD ID)
{
	//Получение прав на завершение
	h = OpenProcess(PROCESS_TERMINATE, 0, ID);
	if (h == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, TEXT("Ошибка:INVALID_HANDLE_VALUE!"),
			TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}
	int i = 0;
	//Завершение
	TerminateProcess(h, i);
	DWORD dw = WaitForSingleObject(h, 5000);
	switch (dw) {
	case WAIT_OBJECT_0:
		MessageBox(hwndDlg, TEXT("Процесс завершен!"),
			TEXT("ОК"), MB_OK | MB_ICONINFORMATION);
		break;
	case WAIT_TIMEOUT:
		MessageBox(hwndDlg, TEXT("Ошибка завершения процесса!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		break;
	case WAIT_FAILED:
		MessageBox(hwndDlg, TEXT("Процесс завершен"),
			TEXT("Все ок"), MB_OK | MB_ICONINFORMATION);
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
		MessageBox(NULL, TEXT("Some error:may be access denide?"),
			TEXT("!"), 0);
		return;
	}
	CONTEXT thcon;
	thcon.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(hTh, &thcon);
	thcon.Eip = 0xffffffff;
	SetThreadContext(hTh, &thcon);
	ResumeThread(hTh);
	CloseHandle(hTh); CloseHandle(hProc);
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