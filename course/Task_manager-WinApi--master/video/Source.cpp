#include <Windows.h>
#include <Vfw.h>
TCHAR GAppname[] = TEXT("Camera Window");

void err(LPCTSTR x) {
    MessageBox(GetActiveWindow(), x, GAppname, MB_OK | MB_ICONSTOP | MB_SETFOREGROUND | MB_SYSTEMMODAL);
    ExitProcess(0);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//
int GetListCaps()
{
    TCHAR Buffer[1024];
    TCHAR Name[512];
    TCHAR Comment[512];
    int nCount = 0;

    for (int i = 0; i < 5; ++i)
    {
        if (capGetDriverDescription(i, Name, 512, Comment, 512))
        {
            wsprintf(Buffer, TEXT("%d: %s %s"), i, Name, Comment);
            ++nCount;
        }
    }

    wsprintf(Buffer, TEXT("%d devices listed"), nCount);
    return nCount;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (GetListCaps() == 0)
        err(TEXT("No device"));

    // Регистрация класса окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"CameraWindowClass";
    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(
        0,
        L"CameraWindowClass",
        L"Camera Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    // Отображение окна
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    HWND hCapWindow = capCreateCaptureWindow(TEXT("Camera Window"), WS_VISIBLE | WS_CHILD, 0, 0, 640, 480, hwnd, 0);

    if (!hCapWindow) {
        DWORD dwError = GetLastError();
        // Обработка ошибки создания окна видеозахвата
        // Выводите сообщение об ошибке, используя dwError
        MessageBox(GetActiveWindow(), TEXT("Failed to create capture window"), GAppname, MB_OK | MB_ICONERROR);
        return 1;
    }
    else {
        // Окно видеозахвата успешно создано, теперь пытаемся подключиться к камере
        if (capDriverConnect(hCapWindow, 0)) {
            // Успешное подключение к камере
        }
        else {
            DWORD dwError = GetLastError();
            // Обработка ошибки подключения к камере
            // Выводите сообщение об ошибке, используя dwError
            MessageBox(GetActiveWindow(), TEXT("Failed to connect to the camera"), GAppname, MB_OK | MB_ICONERROR);
            capDriverConnect(hCapWindow, 0);
            // Освобождение ресурсов, если необходимо, например, DestroyWindow(hCapWindow)

            return 1;
        }
    }


    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Освобождение ресурсов Video for Windows
    capDriverDisconnect(hCapWindow);
    DestroyWindow(hCapWindow);

    return 0;
}
