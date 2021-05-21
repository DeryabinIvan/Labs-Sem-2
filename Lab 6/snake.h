#pragma once

#include <stdlib.h>

#include "field.h"

enum DIRECTION { UP, DOWN, LEFT, RIGTH, NONE };

enum SNAKE_PARTS { HEAD = '@', BODY = 's' };

typedef struct p{
	int x, y;
} point;

typedef struct sp{
	point coord;
	unsigned char sym;
	int direction;
} snake_part;

typedef struct s {
	int length;
	snake_part* parts;
} snake;

void createSnake(snake* s, field* f);

void deleteSnake(snake* s);

void addPart(snake* s);

int moveSnake(snake* s, field* f, int dir);

void printSnake(snake* s);