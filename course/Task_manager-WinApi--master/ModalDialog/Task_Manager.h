#pragma once
#include "header.h"
#include <vector>
#include <tlhelp32.h>

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
