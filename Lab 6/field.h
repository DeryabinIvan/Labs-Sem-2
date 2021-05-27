#pragma once

#include <stdlib.h>

typedef struct f{
	unsigned char** data;
	int height, width;
} field;

enum FIELD_CELLS {
	//границы поля
	BORDER_TOP_DOWN = 205,
	BORDER_LEFT_RIGTH = 186,
	BORDER_TR = 188,
	BORDER_DR = 187,
	BORDER_TL = 201,
	BORDER_DL = 200,

	//стенка
	WALL = 4,

	//еда
	FOOD = 15,

	//пустая клетка
	EMPTY = 32,
};

//создать игровое поле
void craeteField(field* f);

//удалить игровое поле и очистить память
void deleteField(field* f);

//добавить еду на поле
void addFood(field* f);

//вывести поле на экран
void printField(field* f);

//полуить тип клетки
int getXYType(field* f, int x, int y);

//убрать еду с поля
void removeFood(field* f, int x, int y);