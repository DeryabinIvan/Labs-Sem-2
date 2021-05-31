// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "neurolink.h"

#include <stdio.h>
#include <windows.h>

int** imaginaryNumbersSpace;

int height, width;
int max_value;

#define WAVE_WALL -5

void printINS() {
	HANDLE cli = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cli_coord, saved_coord;

	CONSOLE_SCREEN_BUFFER_INFO cbi;
	GetConsoleScreenBufferInfo(cli, &cbi);
	saved_coord = cbi.dwCursorPosition;

	cli_coord.X = 0;
	cli_coord.Y = 20;

	SetConsoleCursorPosition(cli, cli_coord);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf("%d\t", imaginaryNumbersSpace[i][j]);
		}
		printf("\n");
	}

	//SetConsoleCursorPosition(cli, saved_coord);
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

int* calculatePath(field* f, snake* s, POINT start, POINT finish, int* path_len) {
	height = f->height + 1;
	width = f->width + 1;

	max_value = height * width;
	start.x++;
	start.y++;

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
				imaginaryNumbersSpace[i][j] = max_value;
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
	if (path == NULL) {
		exit(-1);
	}

	//развертка пути от финиша к старту
	for (int i = 0; i < *path_len; i++) {
		if (imaginaryNumbersSpace[x + 1][y] < imaginaryNumbersSpace[x][y] &&
			imaginaryNumbersSpace[x + 1][y] != WAVE_WALL) {
			path[*path_len - i - 1] = UP;
			x++;
		} else if (imaginaryNumbersSpace[x - 1][y] < imaginaryNumbersSpace[x][y] &&
				   imaginaryNumbersSpace[x - 1][y] != WAVE_WALL) {
			path[*path_len - i - 1] = DOWN;
			x--;
		} else if (imaginaryNumbersSpace[x][y + 1] < imaginaryNumbersSpace[x][y] &&
				   imaginaryNumbersSpace[x][y + 1] != WAVE_WALL) {
			path[*path_len - i - 1] = LEFT;
			y++;
		} else if (imaginaryNumbersSpace[x][y - 1] < imaginaryNumbersSpace[x][y] &&
				   imaginaryNumbersSpace[x][y - 1] != WAVE_WALL) {
			path[*path_len - i - 1] = RIGTH;
			y--;
		}
	}

	printINS();

	for (int i = 0; i < height; i++) {
		free(imaginaryNumbersSpace[i]);
	}
	free(imaginaryNumbersSpace);

	return path;
}
