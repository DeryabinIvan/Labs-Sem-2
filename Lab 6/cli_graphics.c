#include "cli_graphics.h"

void setCursorPosistion(POINT* p) {
	HANDLE cli = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coord;
	coord.X = p->x;
	coord.Y = p->y;

	SetConsoleCursorPosition(cli, coord);
}

POINT getCursorPosistion() {
	POINT p;
	CONSOLE_SCREEN_BUFFER_INFO cbi;

	HANDLE cli = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(cli, &cbi);

	p.x = cbi.dwCursorPosition.X;
	p.y = cbi.dwCursorPosition.Y;

	return p;
}

void setColor(int background, int foreground) {
	HANDLE cli = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(cli, background | foreground);
}

void hideCursor() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void showCursor() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = 1;
	SetConsoleCursorInfo(out, &cursorInfo);
}
