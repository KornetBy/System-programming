#include <iostream>
#include <vector>
#include <Windows.h>
#include <tlhelp32.h>

#define _CRT_NON_CONFORMING_SWPRINTFS
#include <tchar.h>
char* __stdcall GetProcessCommandLine(
    _In_ HANDLE ProcessHandle,
    _Out_ long* status
);

char* __stdcall GetProcessCommandLineByPid(
    _In_ int pid,
    _Out_ long* status
);
// Объявление функций для работы с ListView
int ListView_AddString(HWND hWnd, LPCTSTR lpText);
HWND ListView_Create(HWND hWndParent, LPCTSTR lpCaption, DWORD dwStyle, int x, int y, int cx, int cy, HWND hWndInsertAfter, HMENU hMenu, HINSTANCE hInstance);
DWORD LVS_REPORT = 0x00000004;
DWORD LVS_SHOWSELALWAYS = 0x00010000;
// Функция для получения списка всех запущенных процессов
std::vector<PROCESSENTRY32> GetProcessList() {
    // Объявление переменной типа PROCESSENTRY32
    PROCESSENTRY32 entry = {};

    // Создание снимка процессов
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return std::vector<PROCESSENTRY32>();
    }

    // Цикл, перебирающий все процессы
    std::vector<PROCESSENTRY32> processList;
    while (Process32Next(hProcessSnap, &entry)) {
        // Добавляем процесс в список
        processList.push_back(entry);
    }

    // Закрытие снимка процессов
    CloseHandle(hProcessSnap);

    // Возвращаем список процессов
    return processList;
}

// Функция для отображения списка процессов в окне
void DisplayProcessList(HWND hWnd, const std::vector<PROCESSENTRY32>& processList) {
    // Создание таблицы для отображения списка процессов
    ListView_Create(hWnd, L"ProcessList", WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS,
        0, 0, 0, 0, NULL, NULL, NULL);

    // Цикл, перебирающий все процессы
    /*for (const PROCESSENTRY32& entry : processList) {
        // Создание строк для отображения информации о процессе
        WCHAR str1[MAX_PATH] = {};
        swprintf(str1, L"%s", entry.szExeFile);

        WCHAR str2[MAX_PATH] = {};
        swprintf(str2, L" (PID: %d)", entry.th32ProcessID);

        // Добавляем строки в таблицу
        ListView_AddString(hWnd, str1);
        ListView_AddString(hWnd, str2);
    }*/
}

// Функция для завершения процесса
void TerminateProcess(DWORD pid) {
    // Получаем дескриптор процесса
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess != NULL) {
        // Завершаем процесс
        TerminateProcess(hProcess, 0);
        // Закрываем дескриптор процесса
        CloseHandle(hProcess);
    }
}

// Основная функция
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Создаем главное окно
    HWND hWnd = CreateWindow(L"TaskManager", L"TaskManager", WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        return 0;
    }

    // Получаем список процессов
    std::vector<PROCESSENTRY32> processList = GetProcessList();

    // Отображаем список процессов в окне
    DisplayProcessList(hWnd, processList);

    // Обработчик сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}