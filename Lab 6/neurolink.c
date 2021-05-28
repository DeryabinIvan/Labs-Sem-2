#include "neurolink.h"

#include <stdio.h>
#include <windows.h>

int** imaginaryNumbersSpace;

int height, width;

#define WAVE_WALL -5

void fill_space(int x, int y, int len) {
	//наткнулись на стену
	if (imaginaryNumbersSpace[x][y] == WAVE_WALL){
		return;
	}

	COORD cli_coord;

	cli_coord.X = 0;
	cli_coord.Y = 0;

	//нашли еду
	if (imaginaryNumbersSpace[x][y] == -1) {
		imaginaryNumbersSpace[x][y] = len;
		return;
	}

	if (x + 1 < height) {
		if (imaginaryNumbersSpace[x + 1][y] <= 0 || imaginaryNumbersSpace[x + 1][y] > len) {
			imaginaryNumbersSpace[x + 1][y] = len;
			fill_space(x + 1, y, len + 1);
		}
	}

	if (y + 1 < height) {
		if (imaginaryNumbersSpace[x][y + 1] <= 0 || imaginaryNumbersSpace[x][y + 1] > len) {
			imaginaryNumbersSpace[x][y + 1] = len;
			fill_space(x, y + 1, len + 1);
		}
	}

	if (x - 1 >= 0) {
		if (imaginaryNumbersSpace[x - 1][y] <= 0 || imaginaryNumbersSpace[x - 1][y] > len) {
			imaginaryNumbersSpace[x - 1][y] = len;
			fill_space(x - 1, y, len + 1);
		}
	}

	if (y - 1 >= 0) {
		if (imaginaryNumbersSpace[x][y - 1] <= 0 || imaginaryNumbersSpace[x][y - 1] > len) {
			imaginaryNumbersSpace[x][y - 1] = len;
			fill_space(x, y - 1, len + 1);
		}
	}
}

int* calculatePath(field* f, snake* s, POINT start, POINT finish, int* path_len) {
	height = f->height + 1;
	width = f->width + 1;

	//инициализация волнового алгоритма
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

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (f->data[i + 1][j + 1] == WALL || existInParts(s, i, j)) {
				//стена или часть змейки это недосягаемые области
				imaginaryNumbersSpace[i][j] = WAVE_WALL;
			} else {
				imaginaryNumbersSpace[i][j] = 0;
			}
		}
	}
	imaginaryNumbersSpace[finish.x][finish.y] = -1;
	imaginaryNumbersSpace[start.x][start.y] = 0;

	//начало волнового алгоритма
	//заполняем поле
	fill_space(start.x, start.y, 1);

	int x = finish.x, y = finish.y;
	*path_len = imaginaryNumbersSpace[x][y];
	
	int* path = (int*) malloc(*path_len * sizeof(int));

	//развертка пути от финиша к старту
	for (int i = 0; i < *path_len; i++) {
		if (imaginaryNumbersSpace[x + 1][y] < imaginaryNumbersSpace[x][y] &&
			imaginaryNumbersSpace[x + 1][y] > 0) {
			path[*path_len - i - 1] = UP;
			x++;
		} else if (imaginaryNumbersSpace[x - 1][y] < imaginaryNumbersSpace[x][y] &&
				   imaginaryNumbersSpace[x - 1][y] > 0) {
			path[*path_len - i - 1] = DOWN;
			x--;
		} else if (imaginaryNumbersSpace[x][y + 1] < imaginaryNumbersSpace[x][y] &&
				   imaginaryNumbersSpace[x][y + 1] > 0) {
			path[*path_len - i - 1] = LEFT;
			y++;
		} else if (imaginaryNumbersSpace[x][y - 1] < imaginaryNumbersSpace[x][y] &&
				   imaginaryNumbersSpace[x][y - 1] > 0) {
			path[*path_len - i - 1] = RIGTH;
			y--;
		}
	}

	for (int i = 0; i < height; i++) {
		free(imaginaryNumbersSpace[i]);
	}
	free(imaginaryNumbersSpace);

	return path;
}
