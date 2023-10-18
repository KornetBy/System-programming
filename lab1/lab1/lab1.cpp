#include <Windows.h>

HWND g_hwnd;
HGDIOBJ g_Brush;
int squareLeft = 300;
int squareTop = 300;
int squareSize = 100;
bool speedBoost = false;
bool isMoving = false;
int velocityX = 0;
int velocityY = 0;
const int g_deceleration = 1;
HACCEL hAccelTable;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void OnCreate(HWND hwnd);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);


LRESULT CALLBACK WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
    switch (nMsg) {
    case WM_CREATE:
        OnCreate(hwnd);
        break;
    case WM_PAINT:
        OnPaint(hwnd);
        break;
    case WM_DESTROY:
        OnDestroy(hwnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1:
            speedBoost = !speedBoost;
        }
        break;
    case WM_KEYDOWN:
    {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        switch (wParam) {
        case VK_LEFT:
            if (squareLeft > 0) {
                velocityX = -(speedBoost ? 40 : 20);
            }
            break;
        case VK_RIGHT:
            if (squareLeft + squareSize < clientRect.right) {
                velocityX = (speedBoost ? 40 : 20);
            }
            break;
        case VK_UP:
            if (squareTop > 0) {
                velocityY = -(speedBoost ? 40 : 20);
            }
            break;
        case VK_DOWN:
            if (squareTop + squareSize < clientRect.bottom) {
                velocityY = (speedBoost ? 40 : 20);
            }
            break;
        case 'V':
            // Проверяем, была ли нажата клавиша Ctrl в момент нажатия 'V'
            if (GetKeyState(VK_CONTROL) & 0x8000) {
                speedBoost = true;
            }
            break;
        }

        if (velocityX != 0 || velocityY != 0) {
            isMoving = true;
            SetTimer(hwnd, 1, 25, NULL);
        }
        break;
    }
    case WM_KEYUP:
    {
        // Обработка отпускания клавиши Ctrl
        if (wParam == 'V') {
            speedBoost = false;
        }
        break;
    }

  
    case WM_TIMER:
    {
        if (isMoving) {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);


            squareLeft += velocityX;
            squareTop += velocityY;


            if (velocityX > 0) {
                velocityX -= g_deceleration;
                if (velocityX < 0)
                    velocityX = 0;
            }
            else if (velocityX < 0) {
                velocityX += g_deceleration;
                if (velocityX > 0) 
                    velocityX = 0;
            }

            if (velocityY > 0) {
                velocityY -= g_deceleration;
                if (velocityY < 0) velocityY = 0;
            }
            else if (velocityY < 0) {
                velocityY += g_deceleration;
                if (velocityY > 0) velocityY = 0;
            }


            if (squareLeft < 0) {
                squareLeft = 0;
                velocityX = -velocityX;
            }
            if (squareTop < 0) {
                squareTop = 0;
                velocityY = -velocityY;
            }
            if (squareLeft + squareSize > clientRect.right) {
                squareLeft = clientRect.right - squareSize;
                velocityX = -velocityX;
            }
            if (squareTop + squareSize > clientRect.bottom) {
                squareTop = clientRect.bottom - squareSize;
                velocityY = -velocityY;
            }

            InvalidateRect(hwnd, NULL, TRUE);
        }
        else {
            KillTimer(hwnd, 1);
        }
        break;
    }

    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        if (GetKeyState(VK_SHIFT) & 0x8000) {
            if (delta > 0) {
                if (squareLeft > 0) {
                   squareLeft -= 10;
                }
                else {
                    squareLeft = 25;
                }
            }
            else if (delta < 0) {
                if (squareLeft + squareSize < clientRect.right) {
                    squareLeft += 10;
                }
                else {
                    squareLeft = clientRect.right - squareSize - 25;
                }
            }
        }
        else {
            if (delta > 0) {
                if (squareTop > 0) {
                    squareTop -= 10;
                }
                else {
                    squareTop = 25;
                }
            }
            else if (delta < 0) {
                if (squareTop + squareSize < clientRect.bottom) {
                    squareTop += 10;
                }
                else {
                    squareTop = clientRect.bottom - squareSize - 25;
                }
            }
        }
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    default:
        return DefWindowProc(hwnd, nMsg, wParam, lParam);
    }
    return 0;
}

void OnCreate(HWND hwnd) {
    g_Brush = CreateSolidBrush(RGB(200, 255, 50));
}

void OnPaint(HWND hwnd) {
    HDC hdc;
    PAINTSTRUCT ps;
    HGDIOBJ old;
    RECT clientRect;
    RECT squareRect;

    hdc = BeginPaint(hwnd, &ps);
    old = SelectObject(hdc, g_Brush);
    GetClientRect(hwnd, &clientRect);
    FillRect(hdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
    squareRect = {
        squareLeft,
        squareTop,
        squareLeft + squareSize,
        squareTop + squareSize
    };
    FillRect(hdc, &squareRect, (HBRUSH)g_Brush);

    SelectObject(hdc, old);
    EndPaint(hwnd, &ps);
}

void OnDestroy(HWND hwnd) {
    DeleteObject(g_Brush);
    PostQuitMessage(0);
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    LPSTR lpCmdLine = GetCommandLineA();
    int nCmdShow = SW_SHOWDEFAULT;
    hAccelTable = LoadAccelerators(hInstance, LPCWSTR(101));
    WNDCLASSEX wc{
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        NULL,
        LoadCursor(NULL, IDC_ARROW),
        NULL,
        NULL,
        L"window",
        NULL
    };
    RegisterClassEx(&wc);

    g_hwnd = CreateWindowEx(0, L"window", L"win32samp", WS_OVERLAPPEDWINDOW, 10, 10, 1000, 800, NULL, NULL, wc.hInstance, NULL);

    MSG msg;
    if (g_hwnd != INVALID_HANDLE_VALUE) {
        ShowWindow(g_hwnd, SW_SHOWDEFAULT);
        UpdateWindow(g_hwnd);

        while (GetMessage(&msg, NULL, 0, 0)) {
            if (!TranslateAccelerator(g_hwnd, hAccelTable, &msg)) {
                //TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    UnregisterClass(L"window", wc.hInstance);
    return (int)msg.wParam;
}
