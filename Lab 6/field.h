#pragma once

#include <stdlib.h>
#include <windows.h>

typedef struct f{
	int height, width;
	unsigned char** data;

	POINT food;
} field;

enum FIELD_CELLS {
	//границы поля
	BORDER_TOP_DOWN = 205,
	BORDER_LEFT_RIGTH = 186,

	//угловые границы
	BORDER_TR = 188,
	BORDER_DR = 187,
	BORDER_TL = 201,
	BORDER_DL = 200,

	//стенка
	WALL = 219,

	//еда
	FOOD = 15,

	//пустая клетка
	EMPTY = 32,
};

//создать игровое поле
void createField(field* f, int difficulty);

//удалить игровое поле и очистить память
void deleteField(field* f);

//вывести поле на экран
void printField(field* f);

//полуить тип клетки
int getCellXY(field* f, int x, int y);

//убрать еду с поля
void removeFood(field* f);