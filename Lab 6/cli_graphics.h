#pragma once

#include <stdlib.h>
#include <windows.h>

#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define BACKGROUND_WHITE (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)

void setCursorPosistion(POINT* p);

POINT getCursorPosistion();

void setColor(int background, int foreground);

void hideCursor();

void showCursor();