// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "field.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "cli_graphics.h"

const int FIELD_BORDER = 1, SAVE_ZONE = 3;

void craeteField(field* f) {
	f->data = (unsigned char**) malloc((f->height + 2 * FIELD_BORDER) * sizeof(unsigned char*));
	if (f->data == NULL) {
		exit(-1);
	}

	for (int i = 0; i < f->width + 2 * FIELD_BORDER; i++) {
		f->data[i] = (unsigned char*) calloc((f->width + 2 * FIELD_BORDER), sizeof(unsigned char));
		if (f->data[i] == NULL) {
			exit(-1);
		}

		if (i == 0 || i == f->height + FIELD_BORDER) {
			memset(f->data[i], BORDER_TOP_DOWN, f->width + FIELD_BORDER * 2);
		} else {
			memset(f->data[i], EMPTY, f->width + FIELD_BORDER * 2);
		}

		f->data[i][0] = BORDER_LEFT_RIGTH;
		f->data[i][f->width + FIELD_BORDER] = BORDER_LEFT_RIGTH;
	}
	f->data[0][0] = BORDER_TL;
	f->data[f->height + FIELD_BORDER][0] = BORDER_DL;
	f->data[0][f->width + FIELD_BORDER] = BORDER_DR;
	f->data[f->height + FIELD_BORDER][f->width + FIELD_BORDER] = BORDER_TR;

	const int max_h = f->height - SAVE_ZONE, max_w = f->width - SAVE_ZONE;

	//заполняем поле препятсвиями
	for (int i = 0; i < (f->height * f->width * 0.02); i++) {
		f->data[FIELD_BORDER + SAVE_ZONE + rand() % max_h][FIELD_BORDER + SAVE_ZONE + rand() % max_w] = WALL;
	}
}

void deleteField(field* f) {
	if (f == NULL) {
		return;
	}

	for (int i = 0; i < f->height + 2 * FIELD_BORDER; i++) {
		free(f->data[i]);
	}

	free(f->data);
}

void printField(field* f) {
	for (int i = 0; i < f->height + FIELD_BORDER * 2; i++) {
		for (int j = 0; j < f->height + FIELD_BORDER * 2; j++) {
			if (f->food.x == i && f->food.y == j) {
				setColor(0, FOREGROUND_GREEN);
			} else if (f->data[i][j] == WALL) {
				setColor(0, FOREGROUND_RED);
			}

			printf("%c", f->data[i][j]);
			setColor(0, FOREGROUND_WHITE);
		}
		printf("\n");
	}
}

int getCellXY(field* f, int x, int y) {
	if (x < -1 || y < -1 || x > f->height || y > f->width) {
		return -1;
	}
	return f->data[x + FIELD_BORDER][y + FIELD_BORDER];
}

void removeFood(field* f, int x, int y) {
	f->data[x + FIELD_BORDER][y + FIELD_BORDER] = EMPTY;
}
