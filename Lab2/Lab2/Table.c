#include <Windows.h>

#define TEXT L"TextBox"
#define ROWS 4
#define COLUMNS 3

void DrawTable(HDC hdc, int width, int height) {
	SaveDC(hdc);

	Rectangle(hdc, 0, 0, width, height);

	int tmpWidth = width / COLUMNS;
	int tmpHeight = height / ROWS;

	for (int i = ROWS-1; i > 0; i--) {
		MoveToEx(hdc,0, tmpHeight*i, NULL);
		LineTo(hdc, width, tmpHeight * i, NULL);
	}

	for (int i = COLUMNS-1; i > 0; i--) {
		MoveToEx(hdc, tmpWidth*i, 0, NULL);
		LineTo(hdc, tmpWidth * i, height, NULL);
	}

	RECT* rect = malloc(sizeof(RECT));
	for (int j = (ROWS); j > 0; j--) {
		for (int i = (COLUMNS); i > 0; i--) {
			rect->left = tmpWidth * (i - 1);
			rect->top = tmpHeight * (j - 1);
			rect->right = tmpWidth * (i);
			rect->bottom = tmpHeight * (j);
			DrawText(hdc, TEXT, -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	free(rect);

	RestoreDC(hdc, -1);
}