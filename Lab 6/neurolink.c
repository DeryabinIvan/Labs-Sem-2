#include "neurolink.h"

#include <stdio.h>

int** imaginaryNumbersSpace;

int max_h, max_w;

void fill_space(int x, int y, int len) {
	if (imaginaryNumbersSpace[x][y] == -2 || 
		x + 1 >= max_h || y + 1 >= max_w ||
		x - 1 < 0 || y - 1 < 0) {
		return;
	}

	if (imaginaryNumbersSpace[x][y] == -1) {
		imaginaryNumbersSpace[x][y] = len;
		return;
	}

	if (imaginaryNumbersSpace[x + 1][y] == 0) {
		imaginaryNumbersSpace[x + 1][y] = len;
		fill_space(x + 1, y, len+1);
	} else if (imaginaryNumbersSpace[x - 1][y] == 0) {
		imaginaryNumbersSpace[x - 1][y] = len;
		fill_space(x - 1, y, len + 1);
	} else if (imaginaryNumbersSpace[x][y + 1] == 0) {
		imaginaryNumbersSpace[x][y + 1] = len;
		fill_space(x, y + 1, len + 1);
	} else if (imaginaryNumbersSpace[x][y - 1] == 0) {
		imaginaryNumbersSpace[x][y] = len;
		fill_space(x, y - 1, len + 1);
	}
}

int* calculatePath(field* f, snake* s, POINT start, POINT finish, int* path_len) {
	//инициализация волнового алгоритма
	imaginaryNumbersSpace = (int**) malloc(f->height * sizeof(int*));
	if (imaginaryNumbersSpace == NULL) {
		exit(-1);
	}

	for (int i = 0; i < f->width; i++) {
		imaginaryNumbersSpace[i] = (int*) malloc(f->width * sizeof(int));
		if (imaginaryNumbersSpace[i] == NULL) {
			exit(-1);
		}
	}

	max_h = f->height;
	max_w = f->width;

	for (int i = 0; i < f->width; i++) {
		for (int j = 0; j < f->width; j++) {
			if (f->data[i + 1][j + 1] == WALL || (existInParts(s, i, j) && i != start.x && j != start.y)) {
				//стена или часть змейки это недосягаемые области
				imaginaryNumbersSpace[i][j] = -2;
			} else {
				imaginaryNumbersSpace[i][j] = 0;
			}
		}
	}
	imaginaryNumbersSpace[finish.x][finish.y] = -1;

	//начало волнового алгоритма
	//заполняем поле
	fill_space(start.x, start.y, 0);

	for (int i = 0; i < f->height; i++) {
		for (int j = 0; j < f->width; j++) {
			printf("%d ", imaginaryNumbersSpace[i][j]);
		}
		printf("\n");
	}

	int x = finish.x, y = finish.y;
	*path_len = imaginaryNumbersSpace[x][y];
	
	int* path = (int*) malloc(*path_len * sizeof(int));

	//развертка пути от финиша к старту
	for (int i = 0; i < *path_len; i++) {
		if (imaginaryNumbersSpace[x + 1][y] < imaginaryNumbersSpace[x][y]) {
			path[i] = UP;
		} else if (imaginaryNumbersSpace[x - 1][y] < imaginaryNumbersSpace[x][y]) {
			path[i] = DOWN;
		} else if (imaginaryNumbersSpace[x][y + 1] < imaginaryNumbersSpace[x][y]) {
			path[i] = LEFT;
		} else if (imaginaryNumbersSpace[x][y - 1] < imaginaryNumbersSpace[x][y]) {
			path[i] = RIGTH;
		} else {
			path[i] = NONE;
		}
	}

	for (int i = 0; i < f->height; i++) {
		free(imaginaryNumbersSpace[i]);
	}
	free(imaginaryNumbersSpace);

	return path;
}
