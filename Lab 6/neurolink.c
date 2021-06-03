// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "neurolink.h"

#include <stdio.h>
#include <windows.h>

#include "cli_graphics.h"

int** imaginaryNumbersSpace;

int height, width;
int max_value;

#define WAVE_WALL -5

void printINS(snake *s, POINT food, POINT head) {
	POINT cli_coord;

	cli_coord.x = 0;
	cli_coord.y = 29;

	setCursorPosistion(&cli_coord);

	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			//Выделяем цветом для удобства отладки
			if (imaginaryNumbersSpace[i][j] == WAVE_WALL) {
				if (existInParts(s, i, j)) {
					setColor(s->color, s->color);
				} else {
					setColor(FOREGROUND_RED, FOREGROUND_RED);
				}
			} else if (i == food.x && j == food.y) {
				setColor(BACKGROUND_GREEN, FOREGROUND_GREEN);
				printf("42\t");
				setColor(0, FOREGROUND_INTENSITY);
			} else if (i == head.x && j == head.y) {
				setColor(BACKGROUND_BLUE, FOREGROUND_BLUE);
				printf("42\t");
				setColor(0, FOREGROUND_INTENSITY);
				continue;
			}

			printf("%d\t", imaginaryNumbersSpace[i][j]);
			setColor(0, FOREGROUND_INTENSITY);
		}
		printf("\n");
		setColor(0, FOREGROUND_INTENSITY);
	}
}

void fill_space(int x, int y, int len) {
	//наткнулись на стену
	if (imaginaryNumbersSpace[x][y] == WAVE_WALL){
		return;
	}

	COORD cli_coord;

	cli_coord.X = 0;
	cli_coord.Y = 0;

	imaginaryNumbersSpace[x][y] = len;

	//нашли еду
	if (imaginaryNumbersSpace[x][y] == -1) {
		return;
	}

	len++;

	if (x + 1 < height) {
		if (imaginaryNumbersSpace[x + 1][y] <= 0 || imaginaryNumbersSpace[x + 1][y] > len) {
			fill_space(x + 1, y, len);
		}
	}

	if (y + 1 < height) {
		if (imaginaryNumbersSpace[x][y + 1] <= 0 || imaginaryNumbersSpace[x][y + 1] > len) {
			fill_space(x, y + 1, len);
		}
	}

	if (x - 1 >= 0) {
		if (imaginaryNumbersSpace[x - 1][y] <= 0 || imaginaryNumbersSpace[x - 1][y] > len) {
			fill_space(x - 1, y, len);
		}
	}

	if (y - 1 >= 0) {
		if (imaginaryNumbersSpace[x][y - 1] <= 0 || imaginaryNumbersSpace[x][y - 1] > len) {
			fill_space(x, y - 1, len);
		}
	}
}

int calculatePath(field* f, snake* s, POINT start, POINT finish, int debug) {
	height = f->height + 2;
	width = f->width + 2;

	max_value = (height - 2) * (width - 2);

	imaginaryNumbersSpace = (int**) malloc(height * sizeof(int*));
	if (imaginaryNumbersSpace == NULL) {
		exit(-1);
	}

	for (int i = 0; i < height; i++) {
		imaginaryNumbersSpace[i] = (int*) malloc(width * sizeof(int));
		if (imaginaryNumbersSpace[i] == NULL) {
			exit(-1);
		}
	}

	//инициализация волнового алгоритма
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (f->data[i][j] == EMPTY && !existInParts(s, i, j)) {
				imaginaryNumbersSpace[i][j] = max_value;
			} else {
				//стена или часть змейки это недосягаемые области
				imaginaryNumbersSpace[i][j] = WAVE_WALL;
			}
		}
	}
	imaginaryNumbersSpace[finish.x][finish.y] = -1;
	imaginaryNumbersSpace[start.x][start.y] = 0;

	//начало волнового алгоритма
	//заполняем поле
	fill_space(start.x, start.y, 1);

	int x = finish.x, y = finish.y;
	int path_len = imaginaryNumbersSpace[x][y];
	if (path_len == -1) {
		return NONE;
	}
	
	int* path = (int*) malloc(path_len * sizeof(int));
	if (path == NULL) {
		exit(-1);
	}

	//развертка пути от финиша к старту
	for (int i = 0; i < path_len; i++) {
		if (imaginaryNumbersSpace[x + 1][y] < imaginaryNumbersSpace[x][y] &&
			imaginaryNumbersSpace[x + 1][y] != WAVE_WALL) {
			path[path_len - i - 1] = UP;
			x++;
		} else if (imaginaryNumbersSpace[x - 1][y] < imaginaryNumbersSpace[x][y] &&
				   imaginaryNumbersSpace[x - 1][y] != WAVE_WALL) {
			path[path_len - i - 1] = DOWN;
			x--;
		} else if (imaginaryNumbersSpace[x][y + 1] < imaginaryNumbersSpace[x][y] &&
				   imaginaryNumbersSpace[x][y + 1] != WAVE_WALL) {
			path[path_len - i - 1] = LEFT;
			y++;
		} else if (imaginaryNumbersSpace[x][y - 1] < imaginaryNumbersSpace[x][y] &&
				   imaginaryNumbersSpace[x][y - 1] != WAVE_WALL) {
			path[path_len - i - 1] = RIGTH;
			y--;
		}
	}

	if(debug != 0)
		printINS(s, finish, start);

	for (int i = 0; i < height; i++) {
		free(imaginaryNumbersSpace[i]);
	}
	free(imaginaryNumbersSpace);

	return path[1];
}

int foodOrEmpty(int cell_type) {
	return cell_type == FOOD || cell_type == EMPTY;
}

int simpleMove(field* f, snake* s) {
	int head_dir = s->parts[0].direction;
	POINT head_coord = s->parts[0].coord;

	//соседние клетки
	int up    = getCellXY(f, head_coord.x-1, head_coord.y),
		down  = getCellXY(f, head_coord.x+1, head_coord.y),
		left  = getCellXY(f, head_coord.x, head_coord.y-1),
		rigth = getCellXY(f, head_coord.x, head_coord.y+1);

	if (foodOrEmpty(up)) {
		return UP;
	} else if (foodOrEmpty(left)) {
		return LEFT;
	} else if (foodOrEmpty(down)) {
		return DOWN;
	} else if (foodOrEmpty(rigth)) {
		return RIGTH;
	}

	return NONE;
}
