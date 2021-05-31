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
