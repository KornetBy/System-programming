#include <Windows.h>
#include <math.h>

void DrawStamp(HDC hdc, int width, int height) {
	SaveDC(hdc);

	Rectangle(hdc, 0, 0, width, height);

	int cX = 300;
	int cY = 300;
	int r3 = 240;
	int r2 = 190;
	int r1 = 140;
	double  angel = 45;

	LPCSTR lpString = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
	int len = 38;

	Ellipse(hdc, 50, 50, 550, 550);

	Ellipse(hdc, 100, 100, 500, 500);

	Ellipse(hdc, 150, 150, 450, 450);

	Ellipse(hdc, 200, 200, 400, 400);

	Ellipse(hdc, 250, 250, 350, 350);

	Ellipse(hdc, 290, 290, 310, 310);

	double changeAngel = 360.0 / len;

	for (int i = len-1; i > -1; i--) {
		HFONT hFont = CreateFont(24, 0, -(angel + 90)*10, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
		SelectObject(hdc, hFont);

		double radAngel = 3.14 * angel / 180.0;

		int x = cX + r1 * cos(radAngel);
		int y = cY + r1 * sin(radAngel);

		TextOut(hdc, x, y, &lpString[i * 2], 1);

		x = cX + r2 * cos(radAngel);
		y = cY + r2 * sin(radAngel);

		TextOut(hdc, x, y, &lpString[i * 2], 1);

		x = cX + r3 * cos(radAngel);
		y = cY + r3 * sin(radAngel);

		TextOut(hdc, x, y, &lpString[i * 2], 1);

		angel -= changeAngel;

		DeleteObject(hFont);
	}

	RestoreDC(hdc, -1);
}
