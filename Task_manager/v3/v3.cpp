#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

// Функция для получения списка процессов
void ListProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Получаем снимок текущих процессов
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, _T("Error creating process snapshot."), _T("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    // Устанавливаем размер структуры
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Получаем информацию о первом процессе
    if (!Process32First(hProcessSnap, &pe32)) {
        MessageBox(NULL, _T("Error getting process information."), _T("Error"), MB_OK | MB_ICONERROR);
        CloseHandle(hProcessSnap);
        return;
    }

    // Очищаем вывод
    SendMessage(GetDlgItem(hwnd, IDC_PROCESS_LIST), LB_RESETCONTENT, 0, 0);

    // Перебираем процессы и выводим информацию о каждом из них
    do {
        // Добавляем процесс в список
        SendMessage(GetDlgItem(hwnd, IDC_PROCESS_LIST), LB_ADDSTRING, 0, (LPARAM)pe32.szExeFile);
    } while (Process32Next(hProcessSnap, &pe32));

    // Закрываем снимок процессов
    CloseHandle(hProcessSnap);
}

// Процедура обработки сообщений
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
    switch (message) {
    case WM_CREATE:
        // Создаем кнопку
        CreateWindow(_T("BUTTON"), _T("Refresh"), WS_VISIBLE | WS_CHILD, 10, 10, 80, 25, hwnd, (HMENU)IDC_REFRESH_BUTTON, NULL, NULL);

        // Создаем список для отображения процессов
        CreateWindow(_T("LISTBOX"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, 10, 50, 300, 200, hwnd, (HMENU)IDC_PROCESS_LIST, NULL, NULL);

        // Инициализируем список процессов
        ListProcesses();
        break;

    case WM_COMMAND:
        // Обработка команд
        switch (LOWORD(w_param)) {
        case IDC_REFRESH_BUTTON:
            // Обновляем список процессов при нажатии кнопки
            ListProcesses();
            break;
        }
        break;

    case WM_DESTROY:
        // Завершаем приложение при закрытии окна
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, w_param, l_param);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    // Регистрируем класс окна
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = h_instance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = _T("WinAPIApp");
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, _T("Error registering window class."), _T("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    // Создаем окно
    hwnd = CreateWindow(_T("WinAPIApp"), _T("Task Manager Lite"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, h_instance, NULL);

    if (!hwnd) {
        MessageBox(NULL, _T("Error creating window."), _T("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    // Отображаем окно
    ShowWindow(hwnd, n_cmd_show);
    UpdateWindow(hwnd);

    // Обрабатываем сообщения
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
