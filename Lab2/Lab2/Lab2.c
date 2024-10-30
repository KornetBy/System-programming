#include <Windows.h>
#include <WinDef.h>
#include "Table.h"
#include "Stamp.h"

// Класс игрового окна
const wchar_t* const GameZoneName = L"Form";

HWND	 hwndGameWindow;
HDC		 hdcBackBuffer;
HANDLE thDraw;
int width, height;
HACCEL hAccelTable;
int type = 0;


void DrawToBackBuffer(HDC hdcBackBuffer) {
	if (type) {
		DrawStamp(hdcBackBuffer, width, height);
	}
	else
	{
		DrawTable(hdcBackBuffer, width, height);
	}
}

void MySwapBuffers(HDC hdc, HDC hdcBackBuffer) {
	BitBlt(hdc, 0, 0, width, height, hdcBackBuffer, 0, 0, SRCCOPY);
}


void  Draw() {
	while (1) {
		Sleep(100);
		InvalidateRect(hwndGameWindow, NULL, 0);
		UpdateWindow(hwndGameWindow);
	}
}


LRESULT CALLBACK GameZoneProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		width = ((CREATESTRUCTA*)lParam)->cx;
		height = ((CREATESTRUCTA*)lParam)->cy;

		hdc = BeginPaint(hWnd, &ps);
		hdcBackBuffer = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
		SelectObject(hdcBackBuffer, hBitmap);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
	}
	break;
	case(WM_GETMINMAXINFO): 
	{
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 300;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 150;
	}
	break;
	case(WM_SIZE):
	{
		width = LOWORD(lParam);
		height = HIWORD((lParam));

		hdc = BeginPaint(hWnd, &ps);
		hdcBackBuffer = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
		SelectObject(hdcBackBuffer, hBitmap);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_PAINT:
	{
		DrawToBackBuffer(hdcBackBuffer);

		hdc = BeginPaint(hWnd, &ps);

		MySwapBuffers(hdc, hdcBackBuffer);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case 40001:
			type = 0;
			break;
		case 40002:
			type = 1;
			break;
		}
	}
	break;
	case WM_DESTROY:
	{
		DeleteDC(hdcBackBuffer);
		PostQuitMessage(0);
	}
	break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	WNDCLASSEX wcex;

	// Регестрация оконого класса
	memset(&wcex, 0, sizeof wcex);
	wcex.cbSize = sizeof wcex;
	wcex.style = CS_GLOBALCLASS;
	wcex.lpfnWndProc = GameZoneProc;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);

	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	wcex.lpszClassName = GameZoneName;
	RegisterClassEx(&wcex);

	// Создание окна
	hwndGameWindow = CreateWindowEx(0, GameZoneName, GameZoneName, WS_VISIBLE | WS_OVERLAPPEDWINDOW,0, 0, 1060, 660, 0, 0, 0, NULL);

	hAccelTable = LoadAccelerators(hInstance, 101);

	thDraw = (HANDLE)_beginthread(Draw, 0, 0);

	while (GetMessage(&msg, 0, 0, 0))
	{
		if (!TranslateAccelerator(hwndGameWindow, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CloseHandle(thDraw);

	return msg.wParam;
}
