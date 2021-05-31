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

void printINS(POINT food, POINT head) {
	POINT cli_coord;

	cli_coord.x = 0;
	cli_coord.y = 29;

	setCursorPosistion(&cli_coord);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (imaginaryNumbersSpace[i][j] == WAVE_WALL) {
				setColor(0, FOREGROUND_RED);
			} else if (i == food.x && j == food.y) {
				setColor(0, FOREGROUND_GREEN);
			} else if (i == head.x && j == head.y) {
				setColor(0, FOREGROUND_BLUE);
			}

			printf("%d\t", imaginaryNumbersSpace[i][j]);
			setColor(0, FOREGROUND_WHITE);
		}
		printf("\n");
		setColor(0, FOREGROUND_WHITE);
	}
}

void fill_space(int x, int y, int len) {
	//���������� �� �����
	if (imaginaryNumbersSpace[x][y] == WAVE_WALL){
		return;
	}

	COORD cli_coord;

	cli_coord.X = 0;
	cli_coord.Y = 0;

	imaginaryNumbersSpace[x][y] = len;

	//����� ���
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

	//������������� ��������� ���������
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (f->data[i][j] == EMPTY && !existInParts(s, i, j)) {
				imaginaryNumbersSpace[i][j] = max_value;
			} else {
				//����� ��� ����� ������ ��� ������������ �������
				imaginaryNumbersSpace[i][j] = WAVE_WALL;
			}
		}
	}
	imaginaryNumbersSpace[finish.x][finish.y] = -1;
	imaginaryNumbersSpace[start.x][start.y] = 0;

	//������ ��������� ���������
	//��������� ����
	fill_space(start.x, start.y, 1);

	int x = finish.x, y = finish.y;
	*path_len = imaginaryNumbersSpace[x][y];
	
	int* path = (int*) malloc(*path_len * sizeof(int));
	if (path == NULL) {
		exit(-1);
	}

	//��������� ���� �� ������ � ������
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

	printINS(finish, start);

	for (int i = 0; i < height; i++) {
		free(imaginaryNumbersSpace[i]);
	}
	free(imaginaryNumbersSpace);

	return path;
}
