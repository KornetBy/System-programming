
#include <psapi.h>
#include <tchar.h>
#include <string>
#include<windows.h>
#include <stdio.h>
#include <tchar.h>

#include <vector>
#include <tlhelp32.h>

// Глобальные переменные
HWND hwndList;
HINSTANCE hInstance;

// Отображение текущих процессов
// ...

class Task_Manager
{
    HWND hList;
    HWND hBox;
    void Update();

public:
    Task_Manager();
    //~Task_Manager();
    static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
    static Task_Manager* ptr;
    void OnInitDialog(HWND h);
    void OnCommand(WPARAM wp, LPARAM lp);
    TCHAR str[255];
    PROCESS_INFORMATION pinfo;
};

Task_Manager* Task_Manager::ptr = nullptr;




Task_Manager::Task_Manager(void) {
	ptr = this;
}

void Task_Manager::OnInitDialog(HWND h) {
	hList = GetDlgItem(h, IDC_LIST1);
	hBox = GetDlgItem(h, IDC_EDIT2);

	// Инициализируем список процессов и устанавливаем таймер
	Update();
	SetTimer(h, 1, 5000, nullptr); // Запускаем таймер каждые 5 секунд (5000 миллисекунд)
}

void Task_Manager::Update() {
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h == INVALID_HANDLE_VALUE) {
		// Обработка ошибки при создании снимка процессов
		return;
	}

	// Очищаем текущий список процессов
	SendMessage(hList, LB_RESETCONTENT, 0, 0);

	PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };
	BOOL flag = Process32First(h, &process);
	while (flag) {
		wstring processInfo = L"Name: " + wstring(process.szExeFile) + L" | PID: " + to_wstring(process.th32ProcessID);

		// Получение дополнительной информации о памяти процесса
		PROCESS_MEMORY_COUNTERS pmc;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process.th32ProcessID);
		if (hProcess != NULL && GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
			processInfo += L" | Memory Usage: " + to_wstring(pmc.WorkingSetSize / (1024 * 1024)) + L" MB";
		}

		int index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(processInfo.c_str()));
		SendMessage(hList, LB_SETITEMDATA, index, LPARAM(process.th32ProcessID));

		if (hProcess != NULL) {
			CloseHandle(hProcess);
		}

		flag = Process32Next(h, &process);
	}

	CloseHandle(h);
}

void Task_Manager::OnCommand(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))
	{
	case IDC_BUTTON1:
		Update();
		break;
	case IDC_BUTTON2:
	{
		STARTUPINFO startin = { sizeof(startin) };
		TCHAR pathname[MAX_PATH];
		GetWindowText(hBox, pathname, MAX_PATH);
		CreateProcess(nullptr, pathname, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startin, &pinfo);
		Update();
	}
	break;
	case IDC_BUTTON3:
	{
		auto n = SendMessage(hList, LB_GETCURSEL, 0, 0);
		DWORD NumberID = SendMessage(hList, LB_GETITEMDATA, n, 0);
		HANDLE Process = OpenProcess(PROCESS_ALL_ACCESS, false, NumberID);
		TerminateProcess(Process, 0);
		CloseHandle(Process);
		Update();
	}
	break;
	}
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	Task_Manager dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Task_Manager::DlgProc);
}