#pragma once

#include <stdlib.h>

#include "field.h"

enum DIRECTION { UP, DOWN, NONE , LEFT, RIGTH};

enum SNAKE_PARTS {
	HEAD = '@',
	BODY = 's',

	BODY_U_OR_D = 179,
	BODY_L_OR_R = 196,
};

//структура точки
typedef struct p{
	int x, y;
} point;

//часть змейки
typedef struct sp{
	point coord;
	unsigned char sym;
	int direction;
} snake_part;

//структура змейки
typedef struct s {
	int length;
	snake_part* parts;
} snake;

//создание змейки
void createSnake(snake* s, field* f);

//удаление змейки
void deleteSnake(snake* s);

//удлиннение змейки
void addPart(snake* s);

//получить направление движения
int getHeadDirection(snake* s);

//проверяет существует ли часть змейки с данными координатами
int existInParts(snake* s, int x, int y);

//"тик" движения
int moveSnake(snake* s, field* f, int dir);

//вывести змейку на экран
void printSnake(snake* s);