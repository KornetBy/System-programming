#define _CRT_SECURE_NO_WARNINGS
#include "Task_Manager.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	Task_Manager dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Task_Manager::DlgProc);
}